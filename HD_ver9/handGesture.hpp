#ifndef _HAND_GESTURE_
#define _HAND_GESTURE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
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
	int stateControl;
	int light;
	int biencu;

	MyImage *m;
	vector<Point> contours;
	vector<int> hullI;
	vector<Point> hullP;
	vector<Vec4i> defects;
	Rect bRect;
	int frameNumber;
	
	int nrOfDefects;

	double bRect_width;
	double bRect_height;
	bool detectIfHand();
	

	void initVectors();
	
	string bool2string(bool tf);//
	int fontFace;//
	void checkForOneFinger(MyImage *m);//
	float getAngle(Point s, Point f, Point e);//
	vector<int> fingerNumbers;//
	void analyzeContours();//
	string intToString(int number);//
	void computeFingerNumber();//
	vector<int> numbers2Display;//
	void addFingerNumberToVector();//
	Scalar numberColor;//
	int nrNoFinger;///
	float distanceP2P(Point a, Point b);
	void removeRedundantEndPoints(vector<Vec4i> newDefects);
	void removeRedundantFingerTips();
	void checkFingerPoint();
public: 
	HandGesture();
	vector<Point> fingerTips; //vector toa do diem dau ngon tay
	int mostFrequentFingerNumber=0;
	/*HandGesture(MyImage *msrc,vector<Point> contourShape, vector<int> hullInt, vector<Point> hullPoint,
		vector<Vec4i> defectShape, Rect bRectShape, int frNumber);*/
	void setHandGesture(MyImage *msrc, vector<Point> contourShape, vector<int> hullInt, vector<Point> hullPoint,
		vector<Vec4i> defectShape, Rect bRectShape, int& frNumber);
	bool isHand;
	vector<Point> getFingerTips(MyImage *m);
	bool detectIfHand(MyImage *m);
	void eleminateDefects();
	void getFingerNumber(MyImage *m,int &frNumber);//
	void addNumberToImg(MyImage *m);
	Point getPointCenter();
	vector<Point> getFingerTipsLinux(MyImage *m);
	vector<Point> getFingerTipsWindows(MyImage *m);

};

#endif
