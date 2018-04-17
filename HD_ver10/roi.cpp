#include "myImage.hpp"
#include "roi.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

MyRoi::MyRoi(){
	upperCorner = Point(0, 0);
	lowerCorner = Point(0, 0);
}

MyRoi::MyRoi(Point u_corner, Point l_corner, Mat src){
	upperCorner = u_corner;
	lowerCorner = l_corner;
	color = Scalar(0, 255, 0);
	borderThickness = 2;
	//roiPtr chứa các ảnh mẫu vuông nhỏ
	roiPtr = src(Rect(	u_corner.x, 
						u_corner.y,
						l_corner.x - u_corner.x, 
						l_corner.y - u_corner.y ));
}
void MyRoi::drawRect(Mat src){
	rectangle(src, upperCorner, lowerCorner, color, borderThickness);
}