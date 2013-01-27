#ifndef POSEESTIMATION_H
#define POSEESTIMATION_H

#include "include/opencv2/opencv.hpp"

//! A wrapper class for the pose estimation function from openCV
/*!
    \author Jonas Strubel
*/
class PoseEstimation
{
public:
    PoseEstimation(cv::Mat objPoints, cv::Mat camMatrix, cv::Mat distCoeffs);
    void estimatePose(cv::Mat imagePoints);
    cv::Mat getRotationVector();
    cv::Mat getTranslationVector();

private:
    cv::Mat objectPoints;
    cv::Mat cameraMatrix;
    cv::Mat distortionCoeffs;
    cv::Mat rvec;
    cv::Mat tvec;
};

#endif // POSEESTIMATION_H
