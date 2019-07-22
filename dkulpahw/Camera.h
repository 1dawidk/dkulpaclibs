#ifndef MORTFIRMWARE_CAMERA_H
#define MORTFIRMWARE_CAMERA_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <highgui.h>
#include <opencv2/videoio.hpp>

class Camera {
public:
    Camera();
    ~Camera();
    void init(unsigned int resX, unsigned int resY);

    int getImage(cv::Mat *buf);

private:
    unsigned int resY;
    unsigned int resX;

#ifdef BOARD_TYPE_RASPBERRYPI
    RaspiCam_Cv *cam_h;
#endif
};


#endif //MORTFIRMWARE_CAMERA_H
