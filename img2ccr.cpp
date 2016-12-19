#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

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

        CvSize winSize = CvSize(8, 8);
        CvSize blockSize = CvSize(4, 4);
        CvSize strideSize = CvSize(4, 4);
        CvSize cellSize = CvSize(2, 2);
        int bins = 9;
        CvSize winShiftSize = CvSize(4, 4);
        CvSize paddingSize = CvSize(2, 2);
        HOGDescriptor hog(winSize, blockSize, strideSize, cellSize, bins);

        Mat hogVector;
        hogVector.create(clrStats.rows, bins, CV_32FC1);
        for (int i = 0; i < clrStats.rows; ++i) {
            vector<float> descriptors;
            Mat srcRoi = grayImg(Rect(clrStats.at<int>(i, CC_STAT_LEFT), clrStats.at<int>(i, CC_STAT_TOP), clrStats.at<int>(i, CC_STAT_WIDTH), clrStats.at<int>(i, CC_STAT_HEIGHT))).clone();
            hog.compute(srcRoi, descriptors, winShiftSize, paddingSize);   

            float result[bins] = {0};
            for (int j = 0; j < descriptors.size(); ++j) {
                result[j % bins] += descriptors[j];
            }

            for (int j = 0; j < bins; ++j) {                
                result[j] = result[j] / (descriptors.size() / bins);
            }

            for (int j = 0; j < bins; ++j) {
                hogVector.at<float>(i, j) = result[j]; 
            }
        }

        Mat svmAns;
        Ptr<SVM> svm = SVM::load("ccx.xml");
        svm->predict(hogVector, svmAns);

        Mat svmStats;
        for (int i = 0; i < svmAns.rows; ++i) {
            if (svmAns.at<float>(i) == 1) {
                svmStats.push_back(clrStats.row(i));
            } else {
                continue;
            }
        }

        for (int i = 0; i < svmStats.rows; ++i) {
            if (svmStats.at<int>(i, CC_STAT_AREA) == 0) {
                continue;
            }

            int left   = svmStats.at<int>(i, CC_STAT_LEFT);
            int right  = svmStats.at<int>(i, CC_STAT_LEFT) + svmStats.at<int>(i, CC_STAT_WIDTH);
            int top    = svmStats.at<int>(i, CC_STAT_TOP);
            int bottom = svmStats.at<int>(i, CC_STAT_TOP) + svmStats.at<int>(i, CC_STAT_HEIGHT);

            for (int j = 0; j < svmStats.rows; ++j) {
                if (svmStats.at<int>(j, CC_STAT_AREA) == 0) {
                    continue;
                }

                int width = abs(svmStats.at<int>(i, CC_STAT_LEFT) + svmStats.at<int>(i, CC_STAT_WIDTH) / 2
                    - svmStats.at<int>(j, CC_STAT_LEFT) - svmStats.at<int>(j, CC_STAT_WIDTH) / 2);
                int height = abs(svmStats.at<int>(i, CC_STAT_TOP) + svmStats.at<int>(i, CC_STAT_HEIGHT) / 2
                    - svmStats.at<int>(j, CC_STAT_TOP) - svmStats.at<int>(j, CC_STAT_HEIGHT) / 2);
                int distance = width * width + height * height;

                if (distance < 2025) {
                    if (svmStats.at<int>(j, CC_STAT_LEFT) < left) {
                        left = svmStats.at<int>(j, CC_STAT_LEFT);
                    }
                    if (svmStats.at<int>(j, CC_STAT_LEFT) + svmStats.at<int>(j, CC_STAT_WIDTH) > right) {
                        right = (svmStats.at<int>(j, CC_STAT_LEFT) + svmStats.at<int>(j, CC_STAT_WIDTH));
                    }
                    if (svmStats.at<int>(j, CC_STAT_TOP) < top) {
                        top = svmStats.at<int>(j, CC_STAT_TOP);
                    }
                    if (svmStats.at<int>(j, CC_STAT_TOP) + svmStats.at<int>(j, CC_STAT_HEIGHT) > bottom) {
                        bottom = (svmStats.at<int>(j, CC_STAT_TOP) + svmStats.at<int>(j, CC_STAT_HEIGHT));
                    }
                    svmStats.at<int>(j, CC_STAT_AREA) = 0;
                    continue;
                }
            }

            svmStats.at<int>(i, CC_STAT_LEFT) = left;
            svmStats.at<int>(i, CC_STAT_WIDTH) = abs(right - left);
            svmStats.at<int>(i, CC_STAT_TOP) = top;
            svmStats.at<int>(i, CC_STAT_HEIGHT) = abs(bottom - top);
            svmStats.at<int>(i, CC_STAT_AREA) = abs(right - left) * abs(bottom - top);
        }

        Mat grpStats;
        for (int i = 0; i < svmStats.rows; ++i) {
            if (svmStats.at<int>(i, CC_STAT_AREA)) {
                grpStats.push_back(svmStats.row(i));
            }
        }

        for (int i = 0; i < grpStats.rows; ++i) {
            ++count;
            string ccName;
            if (dirName.back() == '/') {
                ccName = dirName + "ccr" + to_string(count) + ".jpg";
            } else {
                ccName = dirName + "/ccr" + to_string(count) + ".jpg";
            }
            Mat srcRoi = grayImg(Rect(grpStats.at<int>(i, CC_STAT_LEFT), grpStats.at<int>(i, CC_STAT_TOP), grpStats.at<int>(i, CC_STAT_WIDTH), grpStats.at<int>(i, CC_STAT_HEIGHT))).clone();
            imwrite(ccName, srcRoi);
        }
    }

    return 0;
}