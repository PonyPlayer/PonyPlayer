#include "include/interpolate.h"

Interpolater::Interpolater(int length_, int width_, int framesInBetween_) : length(length_), width(width_),
                                                                            framesInBetween(framesInBetween_) {
    cv::Mat xTrans_(width, length, CV_32FC1);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            xTrans_.at<float>(i, j) = static_cast<float>(j);
        }
    }
    cv::Mat yTrans_(width, length, CV_32FC1);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            yTrans_.at<float>(i, j) = static_cast<float>(i);
        }
    }
    xTrans = xTrans_.getUMat(cv::ACCESS_READ);
    yTrans = yTrans_.getUMat(cv::ACCESS_READ);
}

std::vector<cv::UMat> Interpolater::getInterplatedFrames(const cv::Mat &framePrev, const cv::Mat &frameNext) {
    std::vector<cv::UMat> frames;
    cv::UMat framePrevGray, frameNextGray, flow;
    cvtColor(framePrev, framePrevGray, cv::COLOR_BGR2GRAY);
    cvtColor(frameNext, frameNextGray, cv::COLOR_BGR2GRAY);
    calcOpticalFlowFarneback(framePrevGray, frameNextGray, flow, 0.5, 1, 30, 3, 5, 1.2, 0);
    cv::UMat flowX, flowY;
    extractChannel(flow, flowX, 0);
    extractChannel(flow, flowY, 1);

    cv::divide(flowX, framesInBetween + 1, flowX);
    cv::divide(flowY, framesInBetween + 1, flowY);

    cv::UMat mapX, mapY, currFlowX, currFlowY,image;
    for (int i = 1; i <= framesInBetween; i++) {
        cv::multiply(flowX, i, currFlowX);
        cv::multiply(flowY, i, currFlowY);
        subtract(xTrans, currFlowX, mapX);
        subtract(yTrans, currFlowY, mapY);
        remap(framePrev, image, mapX, mapY, cv::INTER_LINEAR);
        frames.push_back(image);
    }
    return frames;
}
