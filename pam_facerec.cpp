#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include "seeta_recognizer.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <fstream>
#include "config.hpp"

using namespace cv;
using namespace std;

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    int ret = 0;
    char *pass = nullptr;

    Config config;

    const char* username;
    ret = pam_get_user(pamh, &username, "Username");
    if (ret != PAM_SUCCESS) {
        return PAM_SYSTEM_ERR;
    }
    struct pam_conv *conv = nullptr;
    ret = pam_get_item(pamh, PAM_CONV, (const void **)&conv);
    if (ret != PAM_SUCCESS || conv == nullptr) {
        return PAM_SYSTEM_ERR;
    }
    string tip = "Authenticating...";

    struct pam_message msg{};
    const struct pam_message *pmsg;
    struct pam_response *presp;
    msg.msg_style = PAM_TEXT_INFO;
    msg.msg = tip.c_str();
    pmsg = &msg;

    ret = conv->conv(1, &pmsg, &presp, conv->appdata_ptr);
    if (ret != PAM_SUCCESS || presp == nullptr) {
        return PAM_CONV_ERR;
    }
    
    VideoCapture cap(0);
    Mat img;
    if (!cap.read(img)) {
        return PAM_AUTH_ERR;
    }
    auto *recognizer = new Recognizer("/data/lsk/face_recognition/faces", config.getModelPaths());
    vector<pair<string, float>> result = recognizer->recognize(img);
    pair<string, float> similarist = result[0];
    if (similarist.first != username || similarist.second < 0.6) {
        return PAM_AUTH_ERR;
    }
    
    return PAM_SUCCESS;
} 
