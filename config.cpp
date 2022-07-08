#include "config.hpp"

Config::Config() {
    this->loadProperties();
    if (!this->loadSucceed) {
        this->logFp.open("/data/facepam/facepam.log", ios::out);
        this->logFp << "Load properties failed! Using defaults..." << endl;
        this->facePath = "/data/facepam/faces/";
        this->logPath = "/data/facepam/facepam.log";
        this->confidenceGate = 0.6;
    }
    this->initModelPaths();
}

Config::~Config() {
    this->logFp.close();
}

void Config::initModelPaths() {
    this->faceDetectorModel = this->modelPath;
    this->faceDetectorModel.append("/face_detector.seta");
    this->faceLandmarkerModel = this->modelPath;
    this->faceLandmarkerModel.append("/face_landmarker_pts5.csta");
    this->faceRecognizerModel = this->modelPath;
    this->faceRecognizerModel.append("/face_recognizer.csta"); 
}

void Config::loadProperties() {
    try {
        ptree ptr, tag;
        ini_parser::read_ini("/etc/facepam.conf", ptr);
        tag = ptr.get_child("General");
        this->modelPath = tag.get<string>("ModelsPath");
        this->facePath = tag.get<string>("FacesPath");
        this->logPath = tag.get<string>("LogFile");
        this->confidenceGate = tag.get<float>("ConfidenceGate");
        this->loadSucceed = true;
        this->logFp.open(this->logPath, ios::out);
    } catch (exception e) {
        this->loadSucceed = false;
        cout << e.what()  << endl;
    }
}

void Config::writeToLog(const string message) {
    this->logFp << message << endl;
}
