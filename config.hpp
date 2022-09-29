#ifndef _FACEPAM_CONFIG_H_
#define _FACEPAM_CONFIG_H_

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using namespace boost::property_tree; 
using namespace std;

class Config {
    public:
        Config();
        ~Config();
        string faceDetectorModel;
        string faceLandmarkerModel;
        string faceRecognizerModel;
        string facePath;
        string logPath;
        bool   loadSucceed;
        float  confidenceGate;
        void   writeToLog(const string);
        std::vector<string> getModelPaths();
    private:
        void   loadProperties();
        string modelPath = "/usr/share/seetaface/models";
        void   initModelPaths();
        ofstream logFp;
};
#endif

