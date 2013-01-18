#include "poseestimation.h"

#include <iostream>

PoseEstimation::PoseEstimation(cv::Mat objPoints, cv::Mat camMatrix, cv::Mat distCoeffs)
{
    objectPoints = objPoints;
    cameraMatrix = camMatrix;
    distortionCoeffs = distCoeffs;
}

void PoseEstimation::estimatePose(cv::Mat imagePoints){
    cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distortionCoeffs, rvec, tvec);
}

cv::Mat PoseEstimation::getRotationVector(){
    return rvec;
}

cv::Mat PoseEstimation::getTranslationVector(){
    return tvec;
}
