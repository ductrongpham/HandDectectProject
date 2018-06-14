#ifndef _HAND_GESTURE_
#define _HAND_GESTURE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "myImage.hpp"
#define PI 3.14159265358979323846 //su dung de tinh goc 

using namespace cv;
using namespace std;

class HandGesture{

	int giatricu;
	int giatrimoi;
	int stateLight[5];
	int fontFace;//
	int frameNumber;
	int nrNoFinger;///

	Rect bRect;
	Point pointHighest;
	Scalar numberColor;//

	vector<int> numbers2Display;//

	vector<int> hullI;
	vector<Point> hullP;

	vector<int> fingerNumbers;//

	double bRect_width;
	double bRect_height;

	string intToString(int number);//
	string bool2string(bool tf);//

	float getAngle(Point s, Point f, Point e);//
	float distanceP2P(Point a, Point b);

	void initVectors();
	void checkForOneFinger(Mat src);//
	void analyzeContours();//
	void computeFingerNumber();//
	void addFingerNumberToVector();//
	void removeRedundantEndPoints(vector<Vec4i> newDefects);
	void removeRedundantFingerTips();
	void checkFingerPoint();

public:
	vector<Vec4i> defects;
	vector<Point> contours;
	HandGesture();
	Point getPointCenter();
	Point getPointHighest();
	vector<Point> getFingerTips(Mat src);
	vector<Point> fingerTips; //vector toa do diem dau ngon tay
	bool isHand;
	int mostFrequentFingerNumber = 0;
	void setHandGesture(vector<Point> contourShape,
		vector<int> hullInt, vector<Point> hullPoint,
		vector<Vec4i> defectShape, Rect bRectShape, int& frNumber);
	bool detectIfHand(Mat src);
	void eliminateDefects();
	void getFingerNumber(Mat src, int &frNumber);//
	void addNumberToImg(Mat src);
};

#endif
