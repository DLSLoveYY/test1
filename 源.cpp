#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Scalar check(RotatedRect rect, Mat frame);
Scalar color;
vector<int>nums;

int main()
{
    VideoCapture cap("kaohe.mp4");
    Mat src;
    int frameCount = cap.get(CAP_PROP_FRAME_COUNT);
    while (cap.read(src))
    {
        Mat hsv;
        cvtColor(src, hsv, COLOR_BGR2HSV);
        Scalar low = Scalar(20, 150, 150);
        Scalar up = Scalar(30, 255, 255);
        Mat mask;
        inRange(hsv, low, up, mask);
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        morphologyEx(mask, mask, MORPH_OPEN, kernel);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

         for (size_t i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            if (area > 20000)
            {
                RotatedRect rect = minAreaRect(contours[i]);
                color = check(rect, src);
                vector<vector<Point>> contours_1;
                contours_1.push_back(std::vector<Point>());
                Point2f vert[4];
                rect.points(vert);
                contours_1[0].assign(vert, vert + 4);
                drawContours(src, contours_1, 0, color, 4);
                imshow("result", src);
            }
        }
        if (waitKey(1) == 27)
            break;
    }
    cap.release();
    waitKey(0);
    return 0;
}
Scalar check(RotatedRect rect, Mat frame)
{
    Rect box = rect.boundingRect();
    Mat roiFrame = frame(box);
    vector<Mat> vecMat;
    split(roiFrame, vecMat);
    Mat SplitImg = vecMat[2];
    Mat ThresholdImg;
    threshold(SplitImg, ThresholdImg, 128, 255, THRESH_BINARY_INV);
    Mat OpenImg;
    Mat openSE = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
    morphologyEx(ThresholdImg, OpenImg, MORPH_OPEN, openSE);
    Mat CloseImg;
    Mat closeSE = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
    morphologyEx(OpenImg, CloseImg, MORPH_CLOSE, closeSE);
    Mat Labels;
    int ConnectionNum;
    nums.push_back(ConnectionNum);
    {
        ConnectionNum = connectedComponents(CloseImg, Labels, 4, CV_32S);
        nums.push_back(ConnectionNum);
        int n = nums.size();
        if (n <= 11)
            return Scalar(0, 255, 0);
        else if (nums[n - 3] == nums[n - 5] && nums[n - 5] == nums[n - 7])
        {
            int x = nums[n - 3];
            if (x == 11 && x == nums[n - 9])
                color = Scalar(0, 255, 0);
            else if (x == 6 && x == nums[n - 9] && nums[n - 21] != 5)
                color = Scalar(0, 0, 255);
            else if (x == 5)
                color = Scalar(0, 0, 0);
        }
    }
    return color;
}
