#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
    if (argc < 6) {
        cout << "Usage is -s <output> -l <label> <images ...>" << endl;
        return 0;
    }

    int label;    
    string fileName;
    for (int i = 1; i < 5; ++i) {
        if (strcmp(argv[i], "-s") == 0) {
            if (argv[i + 1] != NULL) {
                fileName = argv[i + 1];
                ++i;
            } else {
                cout << "Usage is -s <output> -l <label> <images ...>" << endl;
                return 0;
            }
        } else if (strcmp(argv[i], "-l") == 0) {
            if (argv[i + 1] != NULL) {
                label = atoi(argv[i + 1]);
                ++i;
            } else {
                cout << "Usage is -s <output> -l <label> <images ...>" << endl;
                return 0;
            }
        }
    }

    Mat hog;
    for (int i = 5; i < argc; ++i) {
        string file = argv[i];
        Mat srcImg = imread(file, IMREAD_COLOR);

        if (srcImg.empty()) {
            return -1;
        }

        Mat grayImg;
        cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);

        CvSize winSize = CvSize(8, 8);
        CvSize blockSize = CvSize(4, 4);
        CvSize strideSize = CvSize(4, 4);
        CvSize cellSize = CvSize(2, 2);
        int bins = 9;
        CvSize winShiftSize = CvSize(4, 4);
        CvSize paddingSize = CvSize(2, 2);
        HOGDescriptor hogd(winSize, blockSize, strideSize, cellSize, bins);

        vector<float> descriptors;
        hogd.compute(grayImg, descriptors, winShiftSize, paddingSize);   

        float result[bins] = {0};
        for (int j = 0; j < descriptors.size(); ++j) {
            result[j % bins] += descriptors[j];
        }

        for (int j = 0; j < bins; ++j) {                
            result[j] = result[j] / (descriptors.size() / bins);
        }

        Mat hogVector;
        hogVector.create(1, bins, CV_32FC1);
        for (int j = 0; j < bins; ++j) {
            hogVector.at<float>(0, j) = result[j]; 
        }
        
        hog.push_back(hogVector);
    }

    FileStorage fs(fileName, FileStorage::WRITE);
    fs << "label" << label;
    fs << "hog" << hog;
    fs.release();
    cout << fileName <<" Write Done. There are " << hog.rows << " rows data have been created." << endl;

    return 0;
}