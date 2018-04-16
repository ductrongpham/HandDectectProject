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
	frNumber2 = 0;

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




