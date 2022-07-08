#include "config.hpp"

Config::Config() {
    this->loadProperties();
    if (!this->loadSucceed) {
        ofstream logFile;
        logFile.open("/data/facepam.log", ios::out);
        logFile << "Load properties failed! Using defaults..." << endl;
        logFile.close();
        this->faceDetectorModel = this->defaultModelPath;
        this->faceDetectorModel.append("/face_detector.seta");
        this->faceLandmarkerModel = this->defaultModelPath;
        this->faceLandmarkerModel.append("/face_landmarker_pts5.csta");
        this->faceRecognizerModel = this->defaultModelPath;
        this->faceRecognizerModel.append("/face_recognizer.csta");
        this
    }

}
