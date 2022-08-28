#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>
#include "vec4.hpp"
#include "ray.hpp"

class Camera {
public:
    vec4 eyePoint;
    vec4 aimPoint;
    vec4 up;

    double panAngle;
    double tiltAngle;

    vec4 uAxis;
    vec4 vAxis;
    vec4 nAxis;
    
    double left;
    double right;
    double top;
    double bottom;
    double near;

    int width;
    int height;

    double pixelWidth;
    double pixelHeight;

    Camera() {
        eyePoint = vec4(-10, 0, 1, 1);
        aimPoint = vec4(0, 0, 1, 1);
        up = vec4(0, 0, 1, 0);

        panAngle = 0;
        tiltAngle = 0;

        left = -1;
        right = 1;
        top = 1;
        bottom = -1;
        near = 1;

        width = 512;
        height = 512;
        pixelWidth = (double)(right - left)/(double)(width);
        pixelHeight = (double)(top - bottom)/(double)(height);

        setUVN();
    }

    Camera(vec4 eye, double pan, double tilt, int w, int h, double near, double fovy, double aspect): eyePoint(eye), panAngle(pan), tiltAngle(tilt), width(w), height(h) {
        setLookDirection(pan, tilt);
        rayPerspective(fovy, aspect, near);

        pixelWidth = (double)(right - left)/(double)(width);
        pixelHeight = (double)(top - bottom)/(double)(height);
    }

    void setEyePosition(vec4 pos);
    void setLookDirection(double pan, double tilt);

    //define the absolute size of the image plane and its distance from the aperture
    void rayFrustum(double _left, double _right, double _top, double _bottom, double _near);
    void rayPerspective(double fovy, double aspect, double near);

    //return the ray from the camera's aperture to (xPos, yPos) on the near/image plane
    //NB: in the image plane's coordinate system, each pixel is one unit wide by one unit tall
    ray getEyeRay(double xPos, double yPos);

    void setUVN();
};

#endif