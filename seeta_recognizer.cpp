#include "seeta_recognizer.hpp"

Recognizer::Recognizer(const string knownFacesDir, vector<string> models) {
    loadModels(models);
    loadKnownFeatures(this->fd, this->fl, this->fr, knownFacesDir, &this->knownFeatures);
}

void Recognizer::convertImage(Mat cvImage, SeetaImageData *seetaImage) {
    seetaImage->height = cvImage.rows;
    seetaImage->width = cvImage.cols;
    seetaImage->channels = cvImage.channels();
    seetaImage->data = cvImage.data;
}

void Recognizer::loadModels(vector<string> models) {
    ModelSetting fd_settings;
    fd_settings.set_device(SEETA_DEVICE_AUTO);
    fd_settings.append(models.at(0));
    this->fd = new FaceDetector(fd_settings);

    ModelSetting fl_settings;
    fl_settings.set_device(SEETA_DEVICE_AUTO);
    fl_settings.append(models.at(1));
    this->fl = new FaceLandmarker(fl_settings);

    ModelSetting fr_settings;
    fr_settings.set_device(SEETA_DEVICE_AUTO);
    fr_settings.append(models.at(2));
    this->fr = new FaceRecognizer(fr_settings);
}

void Recognizer::loadKnownFeatures(const FaceDetector *fd,
                       const FaceLandmarker *fl,
                       const FaceRecognizer *fr,
		       const string knownFacesDir,
                       map<string, shared_ptr<float>> *features) {
    DIR* dp = opendir(knownFacesDir.c_str());
    if (!dp) {
        cout << "open faces dir failed!" << endl;
        return;
    }
    for (dirent* entry = readdir(dp); entry != NULL; entry = readdir(dp)) {
        string fname = entry->d_name;
        if (strcmp(".", fname.c_str()) == 0 || strcmp("..", fname.c_str()) == 0) {
            continue;
        }
        string fpath = knownFacesDir;
        fpath.append("/");
        fpath.append(fname.c_str());
        shared_ptr<float> feature(new float[fr->GetExtractFeatureSize()]);
        Mat image = imread(fpath);
        extract_feature(fd, fl, fr, image, feature.get());
        string name = fname.replace(fname.find(".jpg"), 4, "");
        features->insert(pair<string, shared_ptr<float>>(name, feature));
    }
}

void Recognizer::extract_feature(const FaceDetector *FD,
                     const FaceLandmarker *FL,
                     const FaceRecognizer *FR,
                     Mat image,
                     float *feature){
    SeetaImageData img;
    convertImage(image, &img);
    auto faces = FD->detect(img);

    if (faces.size <= 0) {
        cout << "no face detected" << endl;
        return;
    }
    
    SeetaPointF points[5];
    FL->mark(img, faces.data[0].pos, points);
    FR->Extract(img, points, feature);
}

bool Recognizer::cmp(const pair<string, float> p1,
                    const pair<string, float> p2) {
   return p1.second > p2.second; 
}

vector<pair<string, float>> Recognizer::recognize(Mat img) {
    shared_ptr<float> inpFeature(new float[fr->GetExtractFeatureSize()]);
    extract_feature(this->fd, this->fl, this->fr, img, inpFeature.get());
    vector<pair<string, float>> similarities;
    map<string, shared_ptr<float>>::iterator knownIt;
    for (knownIt = knownFeatures.begin(); knownIt != knownFeatures.end(); knownIt++) {
        string name = knownIt->first;
        shared_ptr<float> feature = knownIt->second;
        float sim = this->fr->CalculateSimilarity(inpFeature.get(), feature.get());
        similarities.push_back(make_pair(name, sim));
    }
    sort(similarities.begin(), similarities.end(), cmp);
    return similarities;
}
