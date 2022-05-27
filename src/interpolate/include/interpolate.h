#include <opencv2/opencv.hpp>
#include <vector>

class Interpolater {
private:
    int length, width, framesInBetween;
    cv::UMat xTrans, yTrans;

public:
    Interpolater(int length_, int width_, int framesInBetween_);

    std::vector<cv::UMat> getInterplatedFrames(const cv::Mat& framePrev, const cv::Mat& frameNext);
};