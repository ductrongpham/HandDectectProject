#include "myImage.hpp"
#include "roi.hpp"
#include "handGesture.hpp"
#include "main.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

int fontFace = FONT_HERSHEY_PLAIN;
int square_len;
int avgColor[NSAMPLES][3]; // chưa lần lượt bộ 3 tham số HLS của từng ô mẫu màu
int c_lower[NSAMPLES][3];
int c_upper[NSAMPLES][3];
VideoWriter out;
vector <MyRoi> roi;
//chờ nhận mẫu màu từ lòng bàn tay
void waitForPalmCover(MyImage* m);
//tính toán màu thu thập đc
void average(MyImage *m);
//tạo ra thanh chỉnh màu
void initWindows();
void initTrackbars();
//tạo ảnh nhị phân hiển thị màu cùng với màu da
void produceBinaries(MyImage *m);
//tìm chỉ số của Contour lớn nhất và nhỏ 
int* findBiggestContour(vector<vector<Point> > contours);
//tính toán giá trị median
int getMedian(vector<int> val);
//Hiển thị text
void printText(Mat src, string text);
//laấy ra giá trị trung bình của HLS
void getAvgColor(MyRoi roi, int avg[3]);
//tiêu chuẩn màu sắc
void normalizeColors();
//Hiển thị cửa sổ
void showWindows(MyImage m);
//vẽ
void myDrawContours(MyImage *m, HandGesture *hg);
//tạo đường bao
void makeContours(MyImage *m, HandGesture *h);

int main(){
	MyImage m(0);
	HandGesture hg;
	square_len = 20;
	m.cap >> m.src; // chuyen anh tu cam qua anh nguon
	namedWindow("HandDetect", CV_WINDOW_KEEPRATIO);
	
	//chờ nhận mẫu màu từ lòng bàn tay
	waitForPalmCover(&m);
	
	//tính toán
	average(&m);

	//tat cua so
	destroyWindow("HandDetect");

	//tạo ra thanh chỉnh màu
	initWindows();
	initTrackbars();
	for (;;){
		m.cap >> m.src;
		flip(m.src, m.src, 1); //Lật ảnh

		pyrDown(m.src, m.srcLR);// giam anh xuống 1 nửa
		
		//làm mở anh 1 nửa
		blur(m.srcLR, m.srcLR, Size(3, 3));
		//chuyển đổi màu của ảnh đó sang HLS
		cvtColor(m.srcLR, m.srcLR, ORIGCOL2COL);

		//tạo ảnh nhị phân hiển thị màu cùng với màu da
		produceBinaries(&m);

		//chuyển lại ảnh 1 nửa thành màu RGB lại
		cvtColor(m.srcLR, m.srcLR, COL2ORIGCOL);

		makeContours(&m, &hg);
		//hg.getFingerNumber(&m);
		showWindows(m);

		if (cv::waitKey(30) == char('q')) break;
	}
	destroyAllWindows();
	m.cap.release();
	return 0;
}

void waitForPalmCover(MyImage *m){

	m->cap >> m->src; // truyen anh tu camera vao;
	flip(m->src, m->src, 1); // sử dụng để lật hình với trục y(1)

	//MyRoi(Point upperCorner,Point lowerCorner,Mat src);
	//Vẽ 7 khung ảnh nhỏ ddeeer nhận mẫu
	roi.push_back(MyRoi(Point(m->src.cols / 3, m->src.rows / 6), Point(m->src.cols / 3 + square_len, m->src.rows / 6 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 4, m->src.rows / 2), Point(m->src.cols / 4 + square_len, m->src.rows / 2 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 3, m->src.rows / 1.5), Point(m->src.cols / 3 + square_len, m->src.rows / 1.5 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2, m->src.rows / 2), Point(m->src.cols / 2 + square_len, m->src.rows / 2 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2.5, m->src.rows / 2.5), Point(m->src.cols / 2.5 + square_len, m->src.rows / 2.5 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2, m->src.rows / 1.5), Point(m->src.cols / 2 + square_len, m->src.rows / 1.5 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2.5, m->src.rows / 1.8), Point(m->src.cols / 2.5 + square_len, m->src.rows / 1.8 + square_len), m->src));

	// thuc hien cho nhan với i=50 
	for (int i = 0; i<50; i++){
		m->cap >> m->src;
		flip(m->src, m->src, 1);
		for (int j = 0; j<NSAMPLES; j++){
			roi[j].drawRect(m->src);
		}
		//hiển thị thông tin
		string imgText = string("Cover rectangles with palm");
		printText(m->src, imgText);
		cout << "chuan bi hien anh" << endl;
		imshow("HandDetect", m->src);
		if (cv::waitKey(30) >= 0) break;
	}
}

int getMedian(vector<int> val){
	int median;
	size_t size = val.size();
	sort(val.begin(), val.end());
	if (size % 2 == 0)  {
		median = val[size / 2 - 1];
	}
	else{
		median = val[size / 2];
	}
	return median;
}

//laấy ra giá trị trung bình của HLS
void getAvgColor(MyRoi roi, int avg[3]){
	Mat r;
	roi.roiPtr.copyTo(r);
	vector<int>hm;
	vector<int>sm;
	vector<int>lm;
	// generate vectors
	// truyền lần lượt các giá trị của H L S vào 
	for (int i = 2; i<r.rows - 2; i++){
		for (int j = 2; j<r.cols - 2; j++){
			hm.push_back(r.data[r.channels()*(r.cols*i + j) + 0]);
			sm.push_back(r.data[r.channels()*(r.cols*i + j) + 1]);
			lm.push_back(r.data[r.channels()*(r.cols*i + j) + 2]);
		}
	}
	avg[0] = getMedian(hm);
	avg[1] = getMedian(sm);
	avg[2] = getMedian(lm);
}

// tính toán ra giá trị H L S của 7 ô đc lưu trong avgColor
// khi tính toán xong 7 ô sẽ chuyển sang màu trắng
void average(MyImage *m){	
	m->cap >> m->src;
	flip(m->src, m->src, 1);
	for (int i = 0; i<30; i++){
		m->cap >> m->src;
		flip(m->src, m->src, 1);
		cvtColor(m->src, m->src, ORIGCOL2COL);// chuyển từ RGB sang HLS để tách ra tham số HLS
		for (int j = 0; j<NSAMPLES; j++){
			getAvgColor(roi[j], avgColor[j]);
			roi[j].drawRect(m->src);
		}
		cvtColor(m->src, m->src, COL2ORIGCOL);// cho doi mau cua o tu xanh la thanh trang
		string imgText = string("Finding average color of hand");
		printText(m->src, imgText);
		imshow("HandDetect", m->src);
		if (cv::waitKey(30) >= 0) break;
	}
}

void initWindows(){
	namedWindow("trackbars", CV_WINDOW_KEEPRATIO);
	namedWindow("HandDetect", CV_WINDOW_FULLSCREEN);
}

void initTrackbars(){
	for (int i = 0; i<NSAMPLES; i++){
		c_lower[i][0] = 12;
		c_upper[i][0] = 7;
		c_lower[i][1] = 30;
		c_upper[i][1] = 40;
		c_lower[i][2] = 80;
		c_upper[i][2] = 80;
	}
	createTrackbar("lower1", "trackbars", &c_lower[0][0], 255);
	createTrackbar("lower2", "trackbars", &c_lower[0][1], 255);
	createTrackbar("lower3", "trackbars", &c_lower[0][2], 255);
	createTrackbar("upper1", "trackbars", &c_upper[0][0], 255);
	createTrackbar("upper2", "trackbars", &c_upper[0][1], 255);
	createTrackbar("upper3", "trackbars", &c_upper[0][2], 255);
}

//tiêu chuẩn màu sắc
void normalizeColors(){
	// copy all boundaries read from trackbar
	// to all of the different boundaries
	for (int i = 1; i<NSAMPLES; i++){
		for (int j = 0; j<3; j++){
			c_lower[i][j] = c_lower[0][j];
			c_upper[i][j] = c_upper[0][j];
		}
	}
	// normalize all boundaries so that 
	// threshold is within 0-255
	for (int i = 0; i<NSAMPLES; i++){
		if ((avgColor[i][0] - c_lower[i][0]) <0){
			c_lower[i][0] = avgColor[i][0];
		}if ((avgColor[i][1] - c_lower[i][1]) <0){
			c_lower[i][1] = avgColor[i][1];
		}if ((avgColor[i][2] - c_lower[i][2]) <0){
			c_lower[i][2] = avgColor[i][2];
		}if ((avgColor[i][0] + c_upper[i][0]) >255){
			c_upper[i][0] = 255 - avgColor[i][0];
		}if ((avgColor[i][1] + c_upper[i][1]) >255){
			c_upper[i][1] = 255 - avgColor[i][1];
		}if ((avgColor[i][2] + c_upper[i][2]) >255){
			c_upper[i][2] = 255 - avgColor[i][2];
		}
	}
}

void produceBinaries(MyImage *m){
	Scalar lowerBound;
	Scalar upperBound;
	for (int i = 0; i<NSAMPLES; i++){
		normalizeColors();
		lowerBound = Scalar(avgColor[i][0] - c_lower[i][0], avgColor[i][1] - c_lower[i][1], avgColor[i][2] - c_lower[i][2]);
		upperBound = Scalar(avgColor[i][0] + c_upper[i][0], avgColor[i][1] + c_upper[i][1], avgColor[i][2] + c_upper[i][2]);
		
		//chứa các ảnh nhị phân có kích thước bẳng 1 nửa thôi
		m->bwList.push_back(Mat(m->srcLR.rows, m->srcLR.cols, CV_8U));
		
		//tách ra màu cần thiết dựa vào giá trị của trackbar
		inRange(m->srcLR, lowerBound, upperBound, m->bwList[i]);
	}
	//m->bw là tổng của tất cả các bwList
	m->bwList[0].copyTo(m->bw);
	for (int i = 1; i<NSAMPLES; i++){
		m->bw += m->bwList[i];
	}
	medianBlur(m->bw, m->bw, 7);
}

// tim 2 đường viền lớn nhất
int* findBiggestContour(vector<vector<Point> > contours){
	static int index[2];
	int indexOfBiggestContour = -1;
	int indexOfBiggerContour = -1;
	int sizeOfBiggestContour = 0;
	int sizeOfBiggerContour = 0;

	for (int i = 0; i < contours.size(); i++){
		if (contours[i].size() > sizeOfBiggestContour){
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	for (int i = 0; i < contours.size(); i++){
		if (contours[i].size() > sizeOfBiggerContour){
			if (i == indexOfBiggestContour){
				continue;
			}
			else{
				sizeOfBiggerContour = contours[i].size();
				indexOfBiggerContour = i;
			}
		}
	}
	index[0] = indexOfBiggestContour;
	index[1] = indexOfBiggerContour;
	return index;
}
void myDrawContours(MyImage *m, HandGesture *hg){
	/////////////////////////////////////////////////////////////
	//Mat drawing;

	//Mat im_th;
	//m->bw.copyTo(im_th);
	//threshold(im_th, im_th, 150, 255, THRESH_BINARY);
	//// remove noise
	//cv::erode(im_th, im_th, cv::Mat());
	//cv::erode(im_th, im_th, cv::Mat());
	//cv::dilate(im_th, im_th, cv::Mat());
	//cv::dilate(im_th, im_th, cv::Mat());

	//int fontFace = FONT_HERSHEY_PLAIN;

	//cv::Moments mo = cv::moments(hg->hullPoint[hg->cIdx]);
	//Point center = cv::Point(mo.m10 / mo.m00, mo.m01 / mo.m00);
	//putText(drawing, "*", center, fontFace, 1.2f, Scalar(100, 100, 100), 5, 8);
	//putText(m->src, "*", center, fontFace, 1.2f, Scalar(100, 255, 100), 5, 8);

	//cv::Moments mo2 = cv::moments(hg->hullPoint[hg->cIdx2]);
	//Point center2 = cv::Point(mo2.m10 / mo2.m00, mo2.m01 / mo2.m00);
	//putText(drawing, "*", center2, fontFace, 1.2f, Scalar(100, 100, 100), 5, 8);
	//putText(m->src, "*", center2, fontFace, 1.2f, Scalar(100, 255, 100), 5, 8);
	////khac phuc bug

	//// Floodfill from point (0, 0)
	//Mat im_floodfill = im_th.clone();
	//floodFill(im_floodfill, center, Scalar(0));

	//Mat im_floodfill2 = im_th.clone();
	//floodFill(im_floodfill2, center2, Scalar(0));

	//im_floodfill = im_floodfill & im_floodfill2;
	//// Invert floodfilled image
	//Mat im_floodfill_inv;
	//bitwise_not(im_floodfill, im_floodfill_inv);

	//// Combine the two images to get the foreground.
	//drawing = (im_th & im_floodfill_inv);
	////////////////////////////////////////////////////////////

	//ve duong bao ke thang
	drawContours(m->src, hg->hullPoint, hg->cIdx, cv::Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
	drawContours(m->src, hg->hullPoint, hg->cIdx2, cv::Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());

	/*drawContours(drawing, hg->hullPoint, hg->cIdx, cv::Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
	drawContours(drawing, hg->hullPoint, hg->cIdx2, cv::Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
*/
	//ve duong bao
	drawContours(m->src, hg->contours, hg->cIdx, cv::Scalar(0, 200, 0), 2, 8, vector<Vec4i>(), 0, Point());
	drawContours(m->src, hg->contours, hg->cIdx2, cv::Scalar(0, 200, 0), 2, 8, vector<Vec4i>(), 0, Point());

	//drawContours(drawing, hg->contours, hg->cIdx, cv::Scalar(0, 200, 0), 2, 8, vector<Vec4i>(), 0, Point());
	//drawContours(drawing, hg->contours, hg->cIdx2, cv::Scalar(0, 200, 0), 2, 8, vector<Vec4i>(), 0, Point());

	//ve o vuong bao tay
	rectangle(m->src, hg->bRect.tl(), hg->bRect.br(), Scalar(0, 0, 200));
	rectangle(m->src, hg->bRect2.tl(), hg->bRect2.br(), Scalar(0, 0, 200));

	//rectangle(drawing, hg->bRect.tl(), hg->bRect.br(), Scalar(0, 0, 200));
	//rectangle(drawing, hg->bRect2.tl(), hg->bRect2.br(), Scalar(0, 0, 200));

	//imshow("Anh mophong", drawing);
}

void makeContours(MyImage *m, HandGesture* hg){
	Mat aBw;
	vector<Vec4i> hierarchy;
	//zoom anh nhi phan co kich thuoc nhu ban dau
	pyrUp(m->bw, m->bw);
	//tao 1 ban sao cua anh nhi phan la aBw
	m->bw.copyTo(aBw);

	threshold(aBw, aBw, 150, 255, THRESH_BINARY);
	// remove noise
	cv::erode(aBw, aBw, cv::Mat());
	cv::erode(aBw, aBw, cv::Mat());
	cv::dilate(aBw, aBw, cv::Mat());
	cv::dilate(aBw, aBw, cv::Mat());
	//tim duong vien 
	findContours(aBw, hg->contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	int *a;
	a = new int(sizeof(int));
	a = findBiggestContour(hg->contours);

	hg->initVectors();
	hg->cIdx = *a;//tim duong vien lon nhat co the y la ban tay
	hg->cIdx2 = *(a + 1); //ban tay thu 2

	if (hg->cIdx != -1 && hg->cIdx2 != -1){
		//approxPolyDP( Mat(hg->contours[hg->cIdx]), hg->contours[hg->cIdx], 11, true );
		//tao 1 hinh vuong to bao trọn đường viền
		hg->bRect = boundingRect(Mat(hg->contours[hg->cIdx]));
		hg->bRect2 = boundingRect(Mat(hg->contours[hg->cIdx2]));



		/// Find the convex hull object for each contour
		//Tìm phần vỏ lồi của một điểm đặt.
		convexHull(Mat(hg->contours[hg->cIdx]), hg->hullPoint[hg->cIdx], false, true);
		convexHull(Mat(hg->contours[hg->cIdx2]), hg->hullPoint[hg->cIdx2], false, true);

		//Hàm xấp xỉ 1 đa giác từ những đường cong , kín
		approxPolyDP(Mat(hg->hullPoint[hg->cIdx]), hg->hullPoint[hg->cIdx], 18, true);
		approxPolyDP(Mat(hg->hullPoint[hg->cIdx2]), hg->hullPoint[hg->cIdx2], 18, true);

		myDrawContours(m, hg);
	}
}

void showWindows(MyImage m){
	pyrDown(m.bw, m.bw);
	pyrDown(m.bw, m.bw);
	Rect roi(Point(3 * m.src.cols / 4, 0), m.bw.size());
	vector<Mat> channels;
	Mat result;
	for (int i = 0; i<3; i++)
		channels.push_back(m.bw);
	merge(channels, result);
	result.copyTo(m.src(roi));
	imshow("HandDetect", m.src);
}
void printText(Mat src, string text){
	int fontFace = FONT_HERSHEY_PLAIN;
	putText(src, text, Point(src.cols / 2, src.rows / 10), fontFace, 1.2f, Scalar(200, 0, 0), 2);
}
