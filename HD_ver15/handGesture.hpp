﻿#ifndef _HAND_GESTURE_
#define _HAND_GESTURE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "main.hpp"
#include "myImage.hpp"

using namespace cv;
using namespace std;

class HandGesture{

	int giatricu;
	int giatrimoi;
	int stateLight[5];
	int fontFace;//
	int frameNumber;
	int nrNoFinger;///

	MyImage *m;
	Rect bRect;
	Scalar numberColor;//

	vector<int> numbers2Display;//
	vector<Point> contours;
	vector<int> hullI;
	vector<Point> hullP;
	vector<Vec4i> defects;
	vector<int> fingerNumbers;//

	double bRect_width;
	double bRect_height;

	string intToString(int number);//
	string bool2string(bool tf);//

	float getAngle(Point s, Point f, Point e);//
	float distanceP2P(Point a, Point b);

	void initVectors();
	void checkForOneFinger(MyImage *m);//
	void analyzeContours();//
	void computeFingerNumber();//
	void addFingerNumberToVector();//
	void removeRedundantEndPoints(vector<Vec4i> newDefects);
	void removeRedundantFingerTips();
	void checkFingerPoint();

public: 
	HandGesture();
	Point getPointCenter();
	vector<Point> getFingerTips(MyImage *m);
	vector<Point> fingerTips; //vector toa do diem dau ngon tay

	bool isHand;
	int mostFrequentFingerNumber = 0;

	void setHandGesture(	MyImage *msrc, vector<Point> contourShape, 
							vector<int> hullInt, vector<Point> hullPoint,
							vector<Vec4i> defectShape, Rect bRectShape, int& frNumber);
	bool detectIfHand(MyImage *m);
	void eleminateDefects();
	void getFingerNumber(MyImage *m,int &frNumber);//
	void addNumberToImg(MyImage *m);
	/*HandGesture(MyImage *msrc,vector<Point> contourShape, vector<int> hullInt, vector<Point> hullPoint,
	vector<Vec4i> defectShape, Rect bRectShape, int frNumber);*/
};

#endif
