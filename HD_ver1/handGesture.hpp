#ifndef _HAND_GESTURE_
#define _HAND_GESTURE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "main.hpp"
#include "myImage.hpp"

using namespace std;
using namespace cv;

class HandGesture{
	
	int fontFace; //thang mat
	//int prevNrFingerTips;
	int nrNoFinger;

	float getAngle(Point s, Point f, Point e);//Sô đo góc giữa 3 điểm
	float distanceP2P(Point a, Point b); //Khoảng cách giữa 2 điểm

	vector<int> fingerNumbers; //Vector chứa số ngón tay đc xòe
	//vector<int> numbers2Display; //Vector chứa số lượng ngón tay rồi hiển thị
	
	string bool2string(bool tf); //chuyển dạng dữ liệu bool thành string
	string int2String(int number); // chuyển dạng dữ liệu int thành string

	Scalar numberColor; //tham số này đc sử dụng để quy định màu hiển thị

	void analyzeContours(); //phân tích đường viền (kích thích của ô hcn bao quanh) //xxx
	void computeFingerNumber(); //tính toán tần suất xuất hiện của các bàn tay xòe //xxx
	void removeRedundantFingerTips(); //loại bỏ các ngón tay quá gần nhau
	void addFingerNumberToVector(); //thêm ngón tay vào trong vector //xxx
	void checkForOneFinger(MyImage *m); //kiểm tra trong trường hợp 1 ngón tay
	//void drawNewNumber(MyImage *m);
	void addNumberToImg(MyImage *m); //hiển thị số ngón tay
	// Loại bỏ điểm cuối của khiếm khuyết lồi nếu chúng ở cùng ngón tay
	void removeRedundantEndPoints(	vector<Vec4i> newDefects, 
									MyImage *m);// loai bo cac diem du = loc
public:
	HandGesture();
	MyImage m;

	vector<vector<Point>> contours;		//duong vien bao sat tay
	vector<vector<int>> hullInt;		//duong vien vuong bao ngoai tay
	vector<vector<Point> >hullPoint;	//duong vien trong bao ngoai tay
	vector<vector<Vec4i> > defects;		//khiem khuyet
	vector<Point> fingerTips;			//dau ngon tay
	
	//Rect rect;							//hinh vuong de chen vao
	Rect bRect; //hinh chu nhật bao quanh bàn tay
	Rect bRect2;
	//bool isHand;
	//bool detectIfHand();

	int cIdx; //chỉ số của đường viền to nhất bàn tay 1
	int cIdx2; //chỉ số của bàn tay 2
	int frameNumber; //tính toán frame đã đc chụp để tính khung hình đã xuất hiện //xxx
	int mostFrequentFingerNumber; //tần số xuất hiện số lượng ngón tay đó nhiều nhất
	//Number of defects finger
	int nrOfDefects;
	
	//bounding box height, width
	double bRect_width;
	double bRect_height;

	void initVectors();
	void printGestureInfo(Mat src);		//thong tin cua cu chi 
	void getFingerNumber(MyImage* m);	// tính toán số ngón tay trên 20 khung
	void eleminateDefects(MyImage* m);	//thực hiện kiểm tra ngón tay xòe
	void getFingerTips(MyImage* m);		//đầy tọa độ của các đầu ngón tay vào
	void drawFingerTips(MyImage* m);	//ve dau ngon tay


};

#endif
