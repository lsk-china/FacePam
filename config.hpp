#ifndef _FACEPAM_CONFIG_H_
#define _FACEPAM_CONFIG_H_

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>
#include <fstream>
#include <iostream>

using namespace boost::property_tree; 
using namespace std;

class Config {
    public:
        Config();
        string faceDetectorModel;
        string faceLandmarkerModel;
        string faceRecognizerModel;
        string facePath;
        string logPath;
        bool   loadSucceed;
        float  confidenceGate;
    private:
        void   loadProperties();
        string defaultModelPath = "/usr/share/seetaface/models";
};
#endif

