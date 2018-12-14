#include <zconf.h>
#include <fcntl.h>
#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::init(unsigned int resX, unsigned int resY) {

}

int Camera::getImage(cv::Mat *buf) {
    cv::VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 512);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 512);
    capture.read(*buf);

    return 0;
}