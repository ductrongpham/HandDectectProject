#ifndef _ROI_ 
#define _ROI_

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class MyRoi{
public:
	MyRoi();
	MyRoi(	Point upperCorner,
			Point lowerCorner,
			Mat src);
	Point upperCorner; //Tọa độ góc cao hơn 
	Point lowerCorner; //Tọa độ góc thấp hơn
	Mat roiPtr; //Chữ trong ROI
	Scalar color; //mau sac cua ROI
	int borderThickness; //kich thuoc duong ve
	void drawRect(Mat src); //ve ROI tren anh nguon
};

#endif