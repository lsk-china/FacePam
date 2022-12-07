#ifndef _SEETA_RECOGNIZER_H_
#define _SEETA_RECOGNIZER_H_

#include <seeta/FaceDetector.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/FaceRecognizer.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <string>
#include <dirent.h>
#include <map>
#include <iostream>

using namespace seeta;
using namespace cv;
using namespace std;
using namespace filesystem;

class Recognizer {
    public:
        Recognizer(const string&, vector<string>);
        vector<pair<string, float>> recognize(Mat);
    private:
        FaceDetector *fd;
        FaceRecognizer *fr;
        FaceLandmarker *fl;
        map<string, shared_ptr<float>> knownFeatures;
        
        void loadModels(vector<string>);
        void loadKnownFeatures(const FaceDetector *,
                               const FaceLandmarker *,
                               const FaceRecognizer *,
			       const string&,
                               map<string, shared_ptr<float>> *);
        static void convertImage(const Mat& cvImage, SeetaImageData *seetaImage);
        static void extract_feature(const FaceDetector *,
                             const FaceLandmarker *,
                             const FaceRecognizer *,
                             const Mat& image,
                             float *feature);
        static bool cmp(const pair<string, float>&, const pair<string, float>&);
};

#endif
