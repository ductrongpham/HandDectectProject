#include "handGesture.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

HandGesture::HandGesture(){
	frameNumber = 0;
	nrNoFinger = 0;
	fontFace = FONT_HERSHEY_PLAIN;
}
//khoi tao gia tri duong bao Hull
void HandGesture::initVectors(){
	hullInt = vector<vector<int> >(contours.size());
	hullPoint = vector<vector<Point> >(contours.size());
	defects = vector<vector<Vec4i> >(contours.size());
}

void HandGesture::analyzeContours(){
	bRect_height = bRect.height;
	bRect_width = bRect.width;
}

string HandGesture::bool2string(bool tf){
	if (tf)
		return "true";
	else
		return "false";
}

string HandGesture::int2String(int number){
	stringstream ss;
	ss << number;
	string str = ss.str();
	return str;
}

void HandGesture::printGestureInfo(Mat src){
	int fontFace = FONT_HERSHEY_PLAIN;
	Scalar fColor(245, 200, 200);
	int xpos = src.cols / 1.5;
	int ypos = src.rows / 1.6;
	float fontSize = 0.7f;
	int lineChange = 14;
	string info = "Figure info:";
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fColor);
	xpos += lineChange;
	info = string("Number of defects: ") + string(int2String(nrOfDefects));
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fColor);
	xpos += lineChange;
	info = string("bounding box height, width ") + string(int2String(bRect_height)) + string(" , ") + string(int2String(bRect_width));
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fColor);
}

float HandGesture::distanceP2P(Point a, Point b){
	float d = sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
	return d;
}

// remove fingertips that are too close to 
// eachother
void HandGesture::removeRedundantFingerTips(){
	vector<Point> newFingers;
	for (int i = 0; i<fingerTips.size(); i++){
		for (int j = i; j<fingerTips.size(); j++){
			if (distanceP2P(fingerTips[i], fingerTips[j])<10 && i != j){
			}
			else{
				newFingers.push_back(fingerTips[i]);
				break;
			}
		}
	}
	fingerTips.swap(newFingers);
}

void HandGesture::computeFingerNumber(){
	std::sort(fingerNumbers.begin(), fingerNumbers.end());
	int frequentNr;
	int thisNumberFreq = 1;
	int highestFreq = 1;
	frequentNr = fingerNumbers[0];
	for (int i = 1; i<fingerNumbers.size(); i++){
		if (fingerNumbers[i - 1] != fingerNumbers[i]){
			if (thisNumberFreq>highestFreq){
				frequentNr = fingerNumbers[i - 1];
				highestFreq = thisNumberFreq;
			}
			thisNumberFreq = 0;
		}
		thisNumberFreq++;
	}
	if (thisNumberFreq>highestFreq){
		frequentNr = fingerNumbers[fingerNumbers.size() - 1];
	}
	mostFrequentFingerNumber = frequentNr;
}

void HandGesture::addFingerNumberToVector(){
	int i = fingerTips.size();
	fingerNumbers.push_back(i);
}

// add the calculated number of fingers to image m->src
void HandGesture::addNumberToImg(MyImage *m){
	int xPos = 10;
	int yPos = 10;
	int offset = 30;
	float fontSize = 1.5f;
	int fontFace = FONT_HERSHEY_PLAIN;
		rectangle(m->src, Point(xPos, yPos), Point(xPos + offset, yPos + offset), numberColor, 2);
		putText(m->src, int2String(nrOfDefects), Point(xPos + 7, yPos + offset - 3), fontFace, fontSize, numberColor);

}

float HandGesture::getAngle(Point s, Point f, Point e){
	float l1 = distanceP2P(f, s);
	float l2 = distanceP2P(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / PI;
	return angle;
}

void HandGesture::eleminateDefects(MyImage *m){
	int tolerance = bRect_height / 5;
	float angleTol = 95;
	vector<Vec4i> newDefects;
	int startidx, endidx, faridx;
	vector<Vec4i>::iterator d = defects[cIdx].begin();
	while (d != defects[cIdx].end()) {
		Vec4i& v = (*d);
		startidx = v[0]; Point ptStart(contours[cIdx][startidx]);
		endidx = v[1]; Point ptEnd(contours[cIdx][endidx]);
		faridx = v[2]; Point ptFar(contours[cIdx][faridx]);
		if (distanceP2P(ptStart, ptFar) > tolerance && distanceP2P(ptEnd, ptFar) > tolerance && getAngle(ptStart, ptFar, ptEnd) < angleTol){
			if (ptEnd.y > (bRect.y + bRect.height - bRect.height / 4)){
			}
			else if (ptStart.y > (bRect.y + bRect.height - bRect.height / 4)){
			}
			else {
				newDefects.push_back(v);
			}
		}
		d++;
	}
	nrOfDefects = newDefects.size();
	defects[cIdx].swap(newDefects);
	removeRedundantEndPoints(defects[cIdx], m);
}

// remove endpoint of convexity defects if they are at the same fingertip
void HandGesture::removeRedundantEndPoints(vector<Vec4i> newDefects, MyImage *m){
	Vec4i temp;
	float avgX, avgY;
	float tolerance = bRect_width / 6;
	int startidx, endidx, faridx;
	int startidx2, endidx2;
	for (int i = 0; i<newDefects.size(); i++){
		for (int j = i; j<newDefects.size(); j++){
			startidx = newDefects[i][0]; Point ptStart(contours[cIdx][startidx]);
			endidx = newDefects[i][1]; Point ptEnd(contours[cIdx][endidx]);
			startidx2 = newDefects[j][0]; Point ptStart2(contours[cIdx][startidx2]);
			endidx2 = newDefects[j][1]; Point ptEnd2(contours[cIdx][endidx2]);
			if (distanceP2P(ptStart, ptEnd2) < tolerance){
				contours[cIdx][startidx] = ptEnd2;
				break;
			}if (distanceP2P(ptEnd, ptStart2) < tolerance){
				contours[cIdx][startidx2] = ptEnd;
			}
		}
	}
}

// convexity defects does not check for one finger
// so another method has to check when there are no
// convexity defects
void HandGesture::checkForOneFinger(MyImage *m){
	int yTol = bRect.height / 6;
	Point highestP;
	highestP.y = m->src.rows;
	vector<Point>::iterator d = contours[cIdx].begin();
	while (d != contours[cIdx].end()) {
		Point v = (*d);
		if (v.y<highestP.y){
			highestP = v;
			cout << highestP.y << endl;
		}
		d++;
	}int n = 0;
	d = hullPoint[cIdx].begin();
	while (d != hullPoint[cIdx].end()) {
		Point v = (*d);
		cout << "x " << v.x << " y " << v.y << " highestpY " << highestP.y << "ytol " << yTol << endl;
		if (v.y<highestP.y + yTol && v.y != highestP.y && v.x != highestP.x){
			n++;
		}
		d++;
	}if (n == 0){
		fingerTips.push_back(highestP);
	}
}

void HandGesture::drawFingerTips(MyImage *m){
	Point p;
	int k = 0;
	for (int i = 0; i<fingerTips.size(); i++){
		p = fingerTips[i];
		putText(m->src, int2String(i), p - Point(0, 30), fontFace, 1.2f, Scalar(200, 200, 200), 2);
		circle(m->src, p, 5, Scalar(100, 255, 100), 4);
	}
}

void HandGesture::getFingerTips(MyImage *m){
	fingerTips.clear();
	int i = 0;
	vector<Vec4i>::iterator d = defects[cIdx].begin();
	while (d != defects[cIdx].end()) {
		Vec4i& v = (*d);
		int startidx = v[0]; Point ptStart(contours[cIdx][startidx]);
		int endidx = v[1]; Point ptEnd(contours[cIdx][endidx]);
		int faridx = v[2]; Point ptFar(contours[cIdx][faridx]);
		if (i == 0){
			fingerTips.push_back(ptStart);
			i++;
		}
		fingerTips.push_back(ptEnd);
		d++;
		i++;
	}
	if (fingerTips.size() == 0){
		checkForOneFinger(m);
	}
}