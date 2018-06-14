#ifndef _SHAPE_
#define _SHAPE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "myImage.hpp"
using namespace std;
using namespace cv;

class Shape{
public:
	Shape();
	int fontFace;
	vector <Point> fingerTipShape;
	vector <Point> fingerTipShape2;
	vector<vector<Point>> contourShape;		//duong vien bao sat tay
	vector<vector<int>> hullInt;		//duong vien vuong bao ngoai tay
	vector<vector<Point> >hullPoint;	//duong vien trong bao ngoai tay
	vector<vector<Vec4i> > defectShape;		//khiem khuyet
	int frNumber;
	int frNumber2;
	Rect bRectShape; //hinh chu nhật bao quanh bàn tay
	Rect bRectShape2;
	int cIdxShape; //chỉ số của đường viền to nhất bàn tay 1
	int cIdxShape2; //chỉ số của bàn tay 2
	void initVectors();
	void drawFingerTips(Mat src, Shape *sh, int idxSh, vector <Point> fingerTip1);
};

#endif
