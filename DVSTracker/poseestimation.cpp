#include "poseestimation.h"

#include <iostream>

PoseEstimation::PoseEstimation(cv::Mat objPoints, cv::Mat camMatrix, cv::Mat distCoeffs)
{
    objectPoints = objPoints;
    cameraMatrix = camMatrix;
    distortionCoeffs = distCoeffs;
}

cv::Mat rotox(float x)
{
    cv::Mat R = cv::Mat::eye(3,3,CV_32F);
    R.at<float>(1,1) = cos(x);
    R.at<float>(1,2) = -sin(x);
    R.at<float>(2,1) = sin(x);
    R.at<float>(2,2) = cos(x);
    return R;
}

cv::Mat rotoy(float y)
{
    cv::Mat R = cv::Mat::eye(3,3,CV_32F);
    R.at<float>(0,0) = cos(y);
    R.at<float>(0,2) = sin(y);
    R.at<float>(2,0) = -sin(y);
    R.at<float>(2,2) = cos(y);
    return R;
}

cv::Mat rotoz(float z)
{
    cv::Mat R = cv::Mat::eye(3,3,CV_32F);
    R.at<float>(0,0) = cos(z);
    R.at<float>(0,1) = -sin(z);
    R.at<float>(1,0) = sin(z);
    R.at<float>(1,1) = cos(z);
    return R;
}

void PoseEstimation::estimatePose(cv::Mat imagePoints){
    cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distortionCoeffs, rvec, tvec);
}

void PoseEstimation::test(){
    //intrinsics matrix for isight
    cv::Mat K(3,3,CV_32F, cv::Scalar(0));
    K.at<float>(0,0) = 628.80429;
    K.at<float>(1,1) = 656.37384;
    K.at<float>(2,2) = 1;
    K.at<float>(0,2) = 321.738095;
    K.at<float>(1,2) = 239.862776;

    //    // model plane
    //    /*
    //  cv::Mat M(4,4,CV_32F, cv::Scalar(1));
    //  M.at<float>(0,0)=-1;
    //  M.at<float>(1,0)=1;
    //  M.at<float>(2,0)=0;

    //  M.at<float>(0,1)=1;
    //  M.at<float>(1,1)=1;
    //  M.at<float>(2,1)=0;

    //  M.at<float>(0,2)=1;
    //  M.at<float>(1,2)=-1;
    //  M.at<float>(2,2)=0;

    //  M.at<float>(0,3)=-1;
    //  M.at<float>(1,3)=-1;
    //  M.at<float>(2,3)=0;
    //  */

    cv::Mat Rt(3,4,CV_32F,cv::Scalar(0));
    Rt( cv::Range::all(), cv::Range(0,3) ) = rotox(CV_PI/2)*rotoy(CV_PI / 3)*rotoz(3*CV_PI/2);
    Rt.at<float>(0,3) = 1;
    Rt.at<float>(1,3) = 2;
    Rt.at<float>(2,3) = 3;

    cv::Mat P = K*Rt;

    std::vector<cv::Point3f> model_pts;
    model_pts.push_back(cv::Point3f(-1,1,0.0f));
    model_pts.push_back(cv::Point3f(1,1,0.0f));
    model_pts.push_back(cv::Point3f(1,-1,0.0f));
    model_pts.push_back(cv::Point3f(-1,-1,0.0f));
    cv::Mat M(model_pts);

    std::cout << "M"<< std::endl << M << std::endl << "rows: " << M.rows << "\t" << "cols: " << M.cols << "\t" << ", channels "<< M.channels() <<", npoints= "<< M.checkVector(3,CV_32F) <<std::endl;

    cv::Mat M_hom;
    cv::convertPointsToHomogeneous(M, M_hom);

    std::cout << "M_hom"<< std::endl << M_hom << std::endl << "rows: " << M_hom.rows << "\t" << "cols: " << M_hom.cols << "\t" << ", channels "<< M_hom.channels() <<", npoints= "<< M_hom.checkVector(4,CV_32F) <<std::endl;

    cv::Mat M_resh = M_hom.reshape(1).t();
    std::cout << "M_resh"<< std::endl << M_resh << std::endl << "rows: " << M_resh.rows << "\t" << "cols: " << M_resh.cols << "\t" << ", channels "<< M_resh.channels() <<", npoints= "<< M_resh.checkVector(4,CV_32F) << " type: "<< M_resh.type()<< std::endl;

    cv::Mat m = P*M_resh;
    std::cout << "m"<< std::endl << m << std::endl << "rows: " << m.rows << "\t" << "cols: " << m.cols << "\t"<<", channels "<< m.channels() << ", npoints= " << m.checkVector(3,CV_32F) <<" type: "<< m.type()<< std::endl;

    cv::Mat m_non_hom;
    cv::convertPointsFromHomogeneous(m.t(),m_non_hom);
    m_non_hom = m_non_hom.reshape(1);
    std::cout << "m_non_hom"<< std::endl << m_non_hom << std::endl << "rows: " << m_non_hom.rows << "\t" << "cols: " << m_non_hom.cols << "\t"<<", channels "<< m_non_hom.channels() << ", npoints= " << m_non_hom.checkVector(2,CV_32F) << std::endl;

    //cv::solvePnPRansac(M, m_non_hom, K, cv::Mat(1,4,CV_32F,cv::Scalar(0)), rvec, tvec);
    cv::solvePnP(M, m_non_hom, K, cv::Mat(1,4,CV_32F,cv::Scalar(0)), rvec, tvec);
    std::cout << "t: " << tvec << std::endl;

    cv::Mat R_est;
    cv::Rodrigues(rvec, R_est);

    R_est.convertTo(R_est, CV_32F);

    float norm_difference =  cv::norm ( Rt( cv::Range::all(), cv::Range(0,3) ) - R_est );
    std::cout << "difference: " << norm_difference << std::endl;
}

cv::Mat PoseEstimation::getRotationVector(){
    return rvec;
}

cv::Mat PoseEstimation::getTranslationVector(){
    return tvec;
}
