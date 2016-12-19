#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
    if (argc < 4) {
        cout << "Usage is -s <output dir> <images ...>" << endl;        
        return 0;
    }

    string dirName;
    for (int i = 1; i < 3; ++i) {
        if (strcmp(argv[i], "-s") == 0) {
            if (argv[i + 1] != NULL) {
                dirName = argv[i + 1];
                ++i;
            } else {
                cout << "Usage is -s <output dir> <images ...>" << endl;
                return 0;
            }
        }
    }

    string mkdirCommand = "mkdir " + dirName;
    system(mkdirCommand.c_str());

    int count = 0;
    for (int i = 3; i < argc; ++i) {
        string file = argv[i];
        Mat srcImg = imread(file, IMREAD_COLOR);

        if (srcImg.empty()) {
            cout << "Can't open file \"" << file << "\"." << endl; 
            return -1;
        }

        Mat grayImg;
        cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);

        int threshval = 128;
        Mat binaryImg = grayImg < threshval;

        Mat labelImg, stats, centroids;
        int nLabels = connectedComponentsWithStats(binaryImg, labelImg, stats, centroids, 8, CV_32SC1);

        Mat clrStats;
        for (int i = 1; i < nLabels; ++i) {
            if ((stats.at<int>(i, CC_STAT_WIDTH) < 4 && stats.at<int>(i, CC_STAT_HEIGHT) < 4)) {
                continue;
            } else if (stats.at<int>(i, CC_STAT_WIDTH) > stats.at<int>(0, CC_STAT_WIDTH) * 0.1) {
                continue;
            } else if (stats.at<int>(i, CC_STAT_HEIGHT) > stats.at<int>(0, CC_STAT_HEIGHT) * 0.1) {
                continue;
            } else {
                clrStats.push_back(stats.row(i));
            }
        }

        for (int i = 0; i < clrStats.rows; ++i) {
            ++count;
            string ccName;
            if (dirName.back() == '/') {
                ccName = dirName + "cc" + to_string(count) + ".jpg";
            } else {
                ccName = dirName + "/cc" + to_string(count) + ".jpg";
            }
            Mat srcRoi = grayImg(Rect(clrStats.at<int>(i, CC_STAT_LEFT), clrStats.at<int>(i, CC_STAT_TOP), clrStats.at<int>(i, CC_STAT_WIDTH), clrStats.at<int>(i, CC_STAT_HEIGHT))).clone();
            imwrite(ccName, srcRoi);
        }
    }

    return 0;
}