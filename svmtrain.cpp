#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main(int argc, char **argv) {
    string modelName;
    if (argc < 4) {
        cout << "Usage is -s <output> <files ...>" << endl;
        return 0;
    }

    for (int i = 1; i < 3; ++i) {
        if (strcmp(argv[i], "-s") == 0) {
            if (argv[i + 1] != NULL) {
                modelName = argv[i + 1];
                ++i;
            } else {
                cout << "Usage is -s <output> <files ...>" << endl;
                return 0;
            }
        }
    }

    Mat hogData, labelData;
    for (int i = 3; i < argc; ++i) {
        string fileName = argv[i];
        FileStorage fs(fileName, FileStorage::READ);
        if (!fs.isOpened()) {
            cerr << "Failed to open " << fileName << endl;
            return 1;
        }

        Mat hog;
        fs["hog"] >> hog;
        hogData.push_back(hog);

        int label;
        fs["label"] >> label;
        labelData.push_back(Mat::ones(hog.rows, 1, CV_32SC1) * label);
        fs.release();
    }

    Ptr<TrainData> trainData = ml::TrainData::create(hogData, ROW_SAMPLE, labelData);

    Ptr<SVM> svm = SVM::create();
    // svm->setC(1);
    // svm->setGamma(0.01);
    // svm->setP(1e-6);
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::RBF);
    // svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(hogData, ROW_SAMPLE, labelData);
    // svm->trainAuto(trainData);
        // 10,
        // SVM::getDefaultGrid(SVM::C),
        // SVM::getDefaultGrid(SVM::GAMMA),
        // SVM::getDefaultGrid(SVM::P),
        // SVM::getDefaultGrid(SVM::NU),
        // SVM::getDefaultGrid(SVM::COEF),
        // SVM::getDefaultGrid(SVM::DEGREE),
        // false);
    svm->save(modelName);
    
    return 0;
}