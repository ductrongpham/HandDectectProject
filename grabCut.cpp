#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>

#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#define PI 3.14159265358979323846

using namespace cv;
using namespace std;
//hàm tính toán khoảng cách của 2 điểm

Mat returnImage(Mat frameCurrent, Mat framePrev){
	cv::Mat diffImage;
	Mat anhGrab;
	cv::absdiff(framePrev, frameCurrent, diffImage);

	cv::Mat foregroundMask = cv::Mat::zeros(frameCurrent.rows, frameCurrent.cols, CV_8UC1);

	float threshold1 = 30.0f;
	float dist;

	for (int j = 0; j<diffImage.rows; ++j)
	for (int i = 0; i<diffImage.cols; ++i)
	{
		cv::Vec3b pix = diffImage.at<cv::Vec3b>(j, i);

		dist = (pix[0] * pix[0] + pix[1] * pix[1] + pix[2] * pix[2]);
		dist = sqrt(dist);

		if (dist>threshold1)
		{
			foregroundMask.at<unsigned char>(j, i) = 255;
		}
	}
	adaptiveThreshold(foregroundMask, foregroundMask, 255,/* ADAPTIVE_THRESH_MEAN_C*/ ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 11, -1);
	GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);
	threshold(foregroundMask, foregroundMask, 150, 255, THRESH_BINARY);
	cv::erode(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::erode(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	frameCurrent.copyTo(anhGrab, foregroundMask);
	imshow("mask", foregroundMask);
	imshow("aa", anhGrab);

	return anhGrab;
}

int main(int argc, const char** argv)
{
	int n = 0;
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	//declare mats
	cv::Mat frameCurrent, framePrev;
	//prepare Mats
	cap >> frameCurrent;
	flip(frameCurrent, frameCurrent, 1);
	cap >> framePrev;
	flip(framePrev, framePrev, 1);
	
	while (1)
	{
		returnImage(frameCurrent, framePrev);
		if (waitKey(30) == 27)
			break;
		cap >> frameCurrent;
		flip(frameCurrent, frameCurrent, 1);
	}
	return EXIT_SUCCESS;
}