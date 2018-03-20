#include "shape.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Shape::Shape(){
	frNumber = 0;
	int cIdxShape = 0;
	int cIdxShape2 = 0;
	fontFace = FONT_HERSHEY_PLAIN;
}

void Shape::initVectors(){
	hullInt = vector<vector<int> >(contourShape.size());
	hullPoint = vector<vector<Point> >(contourShape.size());
	defectShape = vector<vector<Vec4i> >(contourShape.size());
}

void Shape::drawFingerTips(MyImage *m, Shape *sh, int idxSh, vector <Point> fingerTip1){
	Point p;
	cv::Moments mo;
	mo = cv::moments(sh->contourShape[idxSh]);
	Point center;
	center = cv::Point(mo.m10 / mo.m00, mo.m01 / mo.m00);

	for (int i = 0; i < fingerTip1.size(); i++){
		p = fingerTip1[i];
		putText(m->src, "ON", p - Point(0, 30), fontFace, 1.2f, Scalar(200, 200, 200), 2);
		circle(m->src, p, 5, Scalar(255, 255, 0), 4);
		putText(m->src, "*", center, fontFace, 1.2f, Scalar(255, 100, 100), 5, 8);
		circle(m->src, p, 8, Scalar(100, 0, 255), 5);
		line(m->src, center, p, Scalar(100, 100, 100), 2, 8);
	}
}

//void Shape::drawFingerTips(MyImage *m, Shape *sh, vector <Point> fingerTip1, vector <Point> fingerTip2){
//	Point p;
//	cv::Moments mo;
//	mo = cv::moments(sh->contourShape[sh->cIdxShape]);
//	Point center;
//	center = cv::Point(mo.m10 / mo.m00, mo.m01 / mo.m00);
//
//	for (int i = 0; i < fingerTip1.size(); i++){
//		p = fingerTip1[i];
//		putText(m->src, "ON", p - Point(0, 30), fontFace, 1.2f, Scalar(200, 200, 200), 2);
//		circle(m->src, p, 5, Scalar(255, 255, 0), 4);
//		putText(m->src, "*", center, fontFace, 1.2f, Scalar(255, 100, 100), 5, 8);
//		circle(m->src, p, 8, Scalar(100, 0, 255), 5);
//		line(m->src, center, p, Scalar(100, 100, 100), 2, 8);
//	}
//	mo = cv::moments(sh->contourShape[sh->cIdxShape2]);
//	center = cv::Point(mo.m10 / mo.m00, mo.m01 / mo.m00);
//	for (int i = 0; i < fingerTip2.size(); i++){
//		p = fingerTip2[i];
//		putText(m->src, "ON", p - Point(0, 30), fontFace, 1.2f, Scalar(200, 200, 200), 2);
//		circle(m->src, p, 5, Scalar(255, 255, 0), 4);
//		putText(m->src, "*", center, fontFace, 1.2f, Scalar(255, 100, 100), 5, 8);
//		circle(m->src, p, 8, Scalar(100, 0, 255), 5);
//		line(m->src, center, p, Scalar(100, 100, 100), 2, 8);
//	}
//}

//void Shape::grabCut(MyImage *m, Shape *sh){
//	// Open another image
//	Mat image;
//	m->src.copyTo(image);
//	pyrDown(image, image);
//	pyrDown(image, image);
//
//	cv::Mat result; // segmentation result (4 possible values)
//	cv::Mat bgModel, fgModel; // the models (internally used)
//
//	// GrabCut segmentation
//	cv::grabCut(image,    // input image
//		result,   // segmentation result
//		sh->bRectShape,// rectangle containing foreground 
//		bgModel, fgModel, // models
//		1,        // number of iterations
//		cv::GC_INIT_WITH_RECT); // use rectangle
//	// Get the pixels marked as likely foreground
//	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
//	// Generate output image
//	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(0));
//	image.copyTo(foreground, result); // bg pixels not copied
//
//	// display result
//	cv::namedWindow("Segmented Image");
//	cv::imshow("Segmented Image", foreground);
//}
//
//void Shape::hienThi(MyImage *m, Shape *sh){
//	///////////////////////////////////////////////////////////
//	Mat drawing;
//
//	Mat im_th;
//	m->bw.copyTo(im_th);
//	threshold(im_th, im_th, 150, 255, THRESH_BINARY);
//	// remove noise
//	cv::erode(im_th, im_th, cv::Mat());
//	cv::erode(im_th, im_th, cv::Mat());
//	cv::dilate(im_th, im_th, cv::Mat());
//	cv::dilate(im_th, im_th, cv::Mat());
//
//	int fontFace = FONT_HERSHEY_PLAIN;
//
//	cv::Moments mo = cv::moments(sh->hullPoint[sh->cIdxShape]);
//	Point center = cv::Point(mo.m10 / mo.m00, mo.m01 / mo.m00);
//
//	cv::Moments mo2 = cv::moments(sh->hullPoint[sh->cIdxShape2]);
//	Point center2 = cv::Point(mo2.m10 / mo2.m00, mo2.m01 / mo2.m00);
//
//	// Floodfill from point (0, 0)
//	Mat im_floodfill = im_th.clone();
//	floodFill(im_floodfill, center, Scalar(0));
//
//	//Mat im_floodfill2 = im_th.clone();
//	//floodFill(im_floodfill2, center2, Scalar(0));
//
//	//im_floodfill = im_floodfill & im_floodfill2;
//	//// Invert floodfilled image
//	Mat im_floodfill_inv;
//	bitwise_not(im_floodfill, im_floodfill_inv);
//
//	// Combine the two images to get the foreground.
//	drawing = (im_th & im_floodfill_inv);
//	//////////////////////////////////////////////////////////
//
//	imshow("hienthi",drawing);
//}


