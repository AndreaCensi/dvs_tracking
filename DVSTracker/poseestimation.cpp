#include "poseestimation.h"

#include <iostream>

//! Constructor
/*!
    \param objPoints The feature points of the object.
    \param camMatrix The intrinsic camera parameters.
    \param distCoeffsf The distortion coefficients of the camera.
*/
PoseEstimation::PoseEstimation(cv::Mat objPoints, cv::Mat camMatrix, cv::Mat distCoeffs)
{
    objectPoints = objPoints;
    cameraMatrix = camMatrix;
    distortionCoeffs = distCoeffs;
}

//! Estimates the pose of the object in relation to the camera coordinate system.
/*!
    \param imagePoints The feature points on the camera image.
*/
void PoseEstimation::estimatePose(cv::Mat imagePoints){
    cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distortionCoeffs, rvec, tvec);
}

cv::Mat PoseEstimation::getRotationVector(){
    return rvec;
}

cv::Mat PoseEstimation::getTranslationVector(){
    return tvec;
}
