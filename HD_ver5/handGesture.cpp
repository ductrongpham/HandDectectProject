#include "handGesture.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
HandGesture::HandGesture(){
	frameNumber = 0;
	nrNoFinger = 0;
	fontFace = FONT_HERSHEY_PLAIN;
}

//HandGesture::HandGesture(MyImage *msrc,vector<Point> contourShape, vector<int> hullInt, vector<Point> hullPoint,
//	vector<Vec4i> defectShape, Rect bRectShape, int frNumber){
//	contours = contourShape;
//	initVectors();
//	m = *msrc;	
//	hullI = hullInt;
//	hullP = hullPoint;
//	defects = defectShape;
//	bRect = bRectShape;
//	frameNumber = frNumber;
//	nrNoFinger = 0;
//	fontFace = FONT_HERSHEY_PLAIN;
//	bRect_height = bRect.height;
//	bRect_width = bRect.width;
//}

void HandGesture::setHandGesture(MyImage *msrc, vector<Point> contourShape, vector<int> hullInt, vector<Point> hullPoint,
	vector<Vec4i> defectShape, Rect bRectShape, int frNumber){

	contours = contourShape;
	initVectors();
	m = msrc;
	hullI = hullInt;
	hullP = hullPoint;
	defects = defectShape;
	bRect = bRectShape;
	//frameNumber = frNumber;
	fontFace = FONT_HERSHEY_PLAIN;
	bRect_height = bRect.height;
	bRect_width = bRect.width;
	//detectIfHand();
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void HandGesture::initVectors(){
	hullI = vector<int>(contours.size());
	hullP = vector<Point>(contours.size());
	defects = vector<Vec4i>(contours.size());
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void HandGesture::analyzeContours(){
	bRect_height = bRect.height;
	bRect_width = bRect.width;
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
string HandGesture::bool2string(bool tf){
	if (tf)
		return "true";
	else
		return "false";
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
string HandGesture::intToString(int number){
	stringstream ss;
	ss << number;
	string str = ss.str();
	return str;
}

////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
bool HandGesture::detectIfHand(){
	analyzeContours();
	double h = bRect_height;
	double w = bRect_width;
	isHand = true;
	if (fingerTips.size() > 5){
		isHand = false;
	}
	else if (h == 0 || w == 0){
		isHand = false;
	}
	else if (h / w > 4 || w / h >4){
		isHand = false;
	}//vi sao llai nho hơn 20 ?!?
	else if (bRect.x<20){
		isHand = false;
	}
	return isHand;
}

bool HandGesture::detectIfHand(MyImage *mi){
	analyzeContours();
	double h = bRect_height;
	double w = bRect_width;
	isHand = true;
	if (fingerTips.size() > 5){
	isHand = false;
	}
	else if (4 * w >= 3 * mi->src.rows)
	{
	isHand = false;
	}
	else if (8 * h >= 7 * mi->src.cols)
	{
	isHand = false;
	}
	else if (h == 0 || w == 0){
	isHand = false;
	}
	else if (h < 30 && w < 30){
	isHand = false;
	}
	else if (h / w > 4 || w / h >4){
	isHand = false;
	}
	else if (bRect.x<20){
	isHand = false;
	}
	//if ((fingerTips.size() > 5) ||
	//	(4 * w >= 3 * mi->src.rows) ||
	//	(8 * h >= 7 * mi->src.cols) ||
	//	(h == 0 || w == 0) ||
	//	(h < 30 && w < 30) ||
	//	(h / w > 4 || w / h >4) ||
	//	(bRect.x<20)
	//	){
	//	isHand = false;
	//}

	return isHand;

}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
float HandGesture::distanceP2P(Point a, Point b){
	float d = sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
	return d;
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//thực hiện kiểm tra ngón tay xòe
void HandGesture::eleminateDefects(){
	//so sánh khoang cach
	int tolerance = bRect_height / 5; //chiều dài tối thiểu của 1 ngón tay
	//so sanh goc
	float angleTol = 95;//goc toi da giua nhung ngon tay
	vector<Vec4i> newDefects;// diem nhon moi
	int startidx, endidx, faridx;//diem dau diem cuoi diem giua
	float dept;
	vector<Vec4i>::iterator d = defects.begin(); // chay cac bien o trong ban tay chinh 1
	while (d != defects.end()) {
		Vec4i& v = (*d);
		startidx = v[0];	Point ptStart(contours[startidx]);
		endidx = v[1];		Point ptEnd(contours[endidx]);
		faridx = v[2];		Point ptFar(contours[faridx]);
		//dept = v[3] / 256;
		if (distanceP2P(ptStart, ptFar) > tolerance //xoe ngon
			&& distanceP2P(ptEnd, ptFar) > tolerance //xoe ngon
			&& getAngle(ptStart, ptFar, ptEnd) < angleTol
			/*&& dept>70*/){
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
	nrOfDefects = newDefects.size(); //dem so ngon tay dang dc xoe ra
	defects.swap(newDefects);
	removeRedundantEndPoints(defects);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// remove endpoint of convexity defects if they are at the same fingertip
// Loại bỏ điểm cuối của khiếm khuyết lồi nếu chúng ở cùng ngón tay
void HandGesture::removeRedundantEndPoints(vector<Vec4i> newDefects){
	float tolerance = bRect_width / 6;//khoảng cách giữa 2 ngón tay
	int startidx, endidx, faridx;
	int startidx2, endidx2;
	for (int i = 0; i<newDefects.size(); i++){
		for (int j = i; j<newDefects.size(); j++){
			startidx = newDefects[i][0];	Point ptStart(contours[startidx]);
			endidx = newDefects[i][1];		Point ptEnd(contours[endidx]);
			startidx2 = newDefects[j][0];	Point ptStart2(contours[startidx2]);
			endidx2 = newDefects[j][1];		Point ptEnd2(contours[endidx2]);
			if (distanceP2P(ptStart, ptEnd2) < tolerance){
				contours[startidx] = ptEnd2;
				break;
			}if (distanceP2P(ptEnd, ptStart2) < tolerance){
				contours[startidx2] = ptEnd;
				//break;
			}
		}
	}
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// remove fingertips that are too close to 
// eachother
// loại bỏ các ngón tay quá gần nhau
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

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
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

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// calculate most frequent numbers of fingers 
// over 20 frames
// tính toán số ngón tay trên 20 khung
void HandGesture::getFingerNumber(MyImage *m, int &frNumber){
	removeRedundantFingerTips();
	if (/*bRect.height > m->src.rows / 2 &&*/ nrNoFinger > 12){
		numberColor = Scalar(0, 200, 0);
		addFingerNumberToVector();
		if (frameNumber>12){
			nrNoFinger = 0;
			frameNumber = 0;
			frNumber = 0;
			computeFingerNumber();
			numbers2Display.push_back(mostFrequentFingerNumber);
			fingerNumbers.clear();
		}
		else{
			frameNumber++;
		}
	}
	else{
		nrNoFinger++;
		numberColor = Scalar(255, 255, 0);
	}
	addNumberToImg(m);
}

void HandGesture::addNumberToImg(MyImage *m){
	int xPos = 10;
	int yPos = 10;
	int offset = 30;
	float fontSize = 1.5f;
	int fontFace = FONT_HERSHEY_PLAIN;
	/*for (int i = 0; i<numbers2Display.size(); i++){
	rectangle(m->src, Point(xPos, yPos), Point(xPos + offset, yPos + offset), numberColor, 2);
	putText(m->src, intToString(numbers2Display[i]), Point(xPos + 7, yPos + offset - 3), fontFace, fontSize, numberColor);
	xPos += 40;
	if (xPos>(m->src.cols - m->src.cols / 3.2)){
	yPos += 40;
	xPos = 10;
	}
	}*/
	cv::Moments mo;
	mo = cv::moments(contours);
	Point center;
	center = cv::Point(mo.m10 / mo.m00, mo.m01 / mo.m00);
	putText(m->src, intToString(mostFrequentFingerNumber), Point(center.x, center.y + 30), fontFace, 2.0f, numberColor, 2);

}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
float HandGesture::getAngle(Point s, Point f, Point e){
	float l1 = distanceP2P(f, s);
	float l2 = distanceP2P(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / PI;
	return angle;
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
vector<Point> HandGesture::getFingerTips(MyImage *m){
	fingerTips.clear();
	int i = 0;
	vector<Vec4i>::iterator d = defects.begin();
	while (d != defects.end()) {
		Vec4i& v = (*d);
		int startidx = v[0]; Point ptStart(contours[startidx]);
		int endidx = v[1]; Point ptEnd(contours[endidx]);
		int faridx = v[2]; Point ptFar(contours[faridx]);
		if (i != 0)
			fingerTips.push_back(ptEnd);
		else
			fingerTips.push_back(ptStart);
		d++;
		i++;
	}
	if (fingerTips.size() == 1/* && (fingerTips[0].x == bRect.x + bRect.width)*/){
		fingerTips.clear();
	}

	//nếu ko có ngón nào thì thử kiểm tra trường hợp 1 ngón
	if (fingerTips.size() == 0){
		checkForOneFinger(m);
	}
	return fingerTips;
}
//void HandGesture::checkFingerPoint(){
//	cv::Moments mo;
//	mo = cv::moments(contours);
//	Point center;
//	center = cv::Point(mo.m10 / mo.m00, mo.m01 / mo.m00);
//	if (fingerTips.size() == 1){
//		fingerTips.clear();
//		cout << "size " << fingerTips.size() << endl;
//	}
//	else{
//		for (int i = 0; i<fingerTips.size() - 1; i++){
//			if (getAngle(fingerTips[i], center, fingerTips[i + 1]) > 95){
//				fingerTips.clear();
//				break;
//			}
//		}
//	}
//
//}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// convexity defects does not check for one finger
// so another method has to check when there are no
// convexity defects
// khiếm khuyết lồi không kiểm tra cho một ngón tay để một phương pháp khác phải kiểm tra khi không có khiếm khuyết lồi
void HandGesture::checkForOneFinger(MyImage *m){
	int yTol = bRect.height / 6; //kiểm tra chiều dài 
	Point highestP;
	highestP.y = m->src.cols; //dài nhất là tọa độ của src.rows
	vector<Point>::iterator d = contours.begin();
	while (d != contours.end()) {
		Point& v = (*d);
		if (v.y<highestP.y){
			highestP = v;
		}
		d++;
	}
	int n = 0;
	d = hullP.begin();
	while (d != hullP.end()) {
		Point v = (*d);
		if (v.y<highestP.y + yTol && v.y != highestP.y && v.x != highestP.x){
			n++;
		}
		d++;
	}
	if (n == 0){
		fingerTips.push_back(highestP);
	}
}

