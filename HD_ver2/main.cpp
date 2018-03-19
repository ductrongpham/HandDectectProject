//#include "myImage.hpp"
//#include "roi.hpp"
//#include "handGesture.hpp"
//#include "main.hpp"
//#include "shape.hpp"
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <cmath>
//
//using namespace std;
//using namespace cv;
//
//int fontFace = FONT_HERSHEY_PLAIN;
//int square_len;
//int avgColor[NSAMPLES][3]; // chưa lần lượt bộ 3 tham số HLS của từng ô mẫu màu
//int c_lower[NSAMPLES][3];
//int c_upper[NSAMPLES][3];
//VideoWriter out;
//vector <MyRoi> roi;
////chờ nhận mẫu màu từ lòng bàn tay
//void waitForPalmCover(MyImage* m);
////tính toán màu thu thập đc
//void average(MyImage *m);
////tạo ra thanh chỉnh màu
//void initWindows();
//void initTrackbars();
////tạo ảnh nhị phân hiển thị màu cùng với màu da
//void produceBinaries(MyImage *m);
////tìm chỉ số của Contour lớn nhất và nhỏ 
//int* findBiggestContour(vector<vector<Point> > contours);
////tính toán giá trị median
//int getMedian(vector<int> val);
////Hiển thị text
//void printText(Mat src, string text);
////laấy ra giá trị trung bình của HLS
//void getAvgColor(MyRoi roi, int avg[3]);
////tiêu chuẩn màu sắc
//void normalizeColors();
////Hiển thị cửa sổ
//void showWindows(MyImage m);
////vẽ
//void myDrawContours(MyImage *m, Shape *sh);
////tạo đường bao
//void makeContours(MyImage *m, Shape *sh);
//
//int main(){
//	MyImage m(0);
//	Shape sh;
//	square_len = 20;
//	m.cap >> m.src; // chuyen anh tu cam qua anh nguon
//	namedWindow("HandDetect", CV_WINDOW_KEEPRATIO);
//
//	//chờ nhận mẫu màu từ lòng bàn tay
//	waitForPalmCover(&m);
//
//	//tính toán
//	average(&m);
//
//	//tat cua so
//	destroyWindow("HandDetect");
//
//	//tạo ra thanh chỉnh màu
//	initWindows();
//	initTrackbars();
//	for (;;){
//		sh.frNumber++;
//		m.cap >> m.src;
//		flip(m.src, m.src, 1); //Lật ảnh
//
//		pyrDown(m.src, m.srcLR);// giam anh xuống 1 nửa
//
//		//làm mở anh 1 nửa
//		blur(m.srcLR, m.srcLR, Size(3, 3));
//		//chuyển đổi màu của ảnh đó sang HLS
//		cvtColor(m.srcLR, m.srcLR, ORIGCOL2COL);
//
//		//tạo ảnh nhị phân hiển thị màu cùng với màu da
//		produceBinaries(&m);
//
//		//chuyển lại ảnh 1 nửa thành màu RGB lại
//		cvtColor(m.srcLR, m.srcLR, COL2ORIGCOL);
//
//		makeContours(&m, &sh);
//		//hg.getFingerNumber(&m);
//		showWindows(m);
//
//		if (cv::waitKey(30) == char('q')) break;
//	}
//	destroyAllWindows();
//	m.cap.release();
//	return 0;
//}
//
//void waitForPalmCover(MyImage *m){
//
//	m->cap >> m->src; // truyen anh tu camera vao;
//	flip(m->src, m->src, 1); // sử dụng để lật hình với trục y(1)
//
//	//MyRoi(Point upperCorner,Point lowerCorner,Mat src);
//	//Vẽ 7 khung ảnh nhỏ ddeeer nhận mẫu
//	roi.push_back(MyRoi(Point(m->src.cols / 3, m->src.rows / 6), Point(m->src.cols / 3 + square_len, m->src.rows / 6 + square_len), m->src));
//	roi.push_back(MyRoi(Point(m->src.cols / 4, m->src.rows / 2), Point(m->src.cols / 4 + square_len, m->src.rows / 2 + square_len), m->src));
//	roi.push_back(MyRoi(Point(m->src.cols / 3, m->src.rows / 1.5), Point(m->src.cols / 3 + square_len, m->src.rows / 1.5 + square_len), m->src));
//	roi.push_back(MyRoi(Point(m->src.cols / 2, m->src.rows / 2), Point(m->src.cols / 2 + square_len, m->src.rows / 2 + square_len), m->src));
//	roi.push_back(MyRoi(Point(m->src.cols / 2.5, m->src.rows / 2.5), Point(m->src.cols / 2.5 + square_len, m->src.rows / 2.5 + square_len), m->src));
//	roi.push_back(MyRoi(Point(m->src.cols / 2, m->src.rows / 1.5), Point(m->src.cols / 2 + square_len, m->src.rows / 1.5 + square_len), m->src));
//	roi.push_back(MyRoi(Point(m->src.cols / 2.5, m->src.rows / 1.8), Point(m->src.cols / 2.5 + square_len, m->src.rows / 1.8 + square_len), m->src));
//
//	// thuc hien cho nhan với i=50 
//	for (int i = 0; i < 50; i++){
//		m->cap >> m->src;
//		flip(m->src, m->src, 1);
//		for (int j = 0; j < NSAMPLES; j++){
//			roi[j].drawRect(m->src);
//		}
//		//hiển thị thông tin
//		string imgText = string("Cover rectangles with palm");
//		printText(m->src, imgText);
//		imshow("HandDetect", m->src);
//		if (cv::waitKey(30) >= 0) break;
//	}
//}
//
//int getMedian(vector<int> val){
//	int median;
//	size_t size = val.size();
//	sort(val.begin(), val.end());
//	if (size % 2 == 0)  {
//		median = val[size / 2 - 1];
//	}
//	else{
//		median = val[size / 2];
//	}
//	return median;
//}
//
////laấy ra giá trị trung bình của HLS
//void getAvgColor(MyRoi roi, int avg[3]){
//	Mat r;
//	roi.roiPtr.copyTo(r);
//	vector<int>hm;
//	vector<int>sm;
//	vector<int>lm;
//	// generate vectors
//	// truyền lần lượt các giá trị của H L S vào 
//	for (int i = 2; i < r.rows - 2; i++){
//		for (int j = 2; j < r.cols - 2; j++){
//			hm.push_back(r.data[r.channels()*(r.cols*i + j) + 0]);
//			sm.push_back(r.data[r.channels()*(r.cols*i + j) + 1]);
//			lm.push_back(r.data[r.channels()*(r.cols*i + j) + 2]);
//		}
//	}
//	avg[0] = getMedian(hm);
//	avg[1] = getMedian(sm);
//	avg[2] = getMedian(lm);
//}
//
//// tính toán ra giá trị H L S của 7 ô đc lưu trong avgColor
//// khi tính toán xong 7 ô sẽ chuyển sang màu trắng
//void average(MyImage *m){
//	m->cap >> m->src;
//	flip(m->src, m->src, 1);
//	for (int i = 0; i < 30; i++){
//		m->cap >> m->src;
//		flip(m->src, m->src, 1);
//		cvtColor(m->src, m->src, ORIGCOL2COL);// chuyển từ RGB sang HLS để tách ra tham số HLS
//		for (int j = 0; j < NSAMPLES; j++){
//			getAvgColor(roi[j], avgColor[j]);
//			roi[j].drawRect(m->src);
//		}
//		cvtColor(m->src, m->src, COL2ORIGCOL);// cho doi mau cua o tu xanh la thanh trang
//		string imgText = string("Finding average color of hand");
//		printText(m->src, imgText);
//		imshow("HandDetect", m->src);
//		if (cv::waitKey(30) >= 0) break;
//	}
//}
//
//void initWindows(){
//	namedWindow("trackbars", CV_WINDOW_KEEPRATIO);
//	namedWindow("HandDetect", CV_WINDOW_FULLSCREEN);
//}
//
//void initTrackbars(){
//	for (int i = 0; i < NSAMPLES; i++){
//		c_lower[i][0] = 12;
//		c_upper[i][0] = 7;
//		c_lower[i][1] = 30;
//		c_upper[i][1] = 40;
//		c_lower[i][2] = 80;
//		c_upper[i][2] = 80;
//	}
//	createTrackbar("lower1", "trackbars", &c_lower[0][0], 255);
//	createTrackbar("lower2", "trackbars", &c_lower[0][1], 255);
//	createTrackbar("lower3", "trackbars", &c_lower[0][2], 255);
//	createTrackbar("upper1", "trackbars", &c_upper[0][0], 255);
//	createTrackbar("upper2", "trackbars", &c_upper[0][1], 255);
//	createTrackbar("upper3", "trackbars", &c_upper[0][2], 255);
//}
//
////tiêu chuẩn màu sắc
//void normalizeColors(){
//	// copy all boundaries read from trackbar
//	// to all of the different boundaries
//	for (int i = 1; i < NSAMPLES; i++){
//		for (int j = 0; j < 3; j++){
//			c_lower[i][j] = c_lower[0][j];
//			c_upper[i][j] = c_upper[0][j];
//		}
//	}
//	// normalize all boundaries so that 
//	// threshold is within 0-255
//	for (int i = 0; i < NSAMPLES; i++){
//		if ((avgColor[i][0] - c_lower[i][0]) <0){
//			c_lower[i][0] = avgColor[i][0];
//		}if ((avgColor[i][1] - c_lower[i][1]) <0){
//			c_lower[i][1] = avgColor[i][1];
//		}if ((avgColor[i][2] - c_lower[i][2]) <0){
//			c_lower[i][2] = avgColor[i][2];
//		}if ((avgColor[i][0] + c_upper[i][0]) >255){
//			c_upper[i][0] = 255 - avgColor[i][0];
//		}if ((avgColor[i][1] + c_upper[i][1]) >255){
//			c_upper[i][1] = 255 - avgColor[i][1];
//		}if ((avgColor[i][2] + c_upper[i][2]) >255){
//			c_upper[i][2] = 255 - avgColor[i][2];
//		}
//	}
//}
//
//void produceBinaries(MyImage *m){
//	Scalar lowerBound;
//	Scalar upperBound;
//	for (int i = 0; i < NSAMPLES; i++){
//		normalizeColors();
//		lowerBound = Scalar(avgColor[i][0] - c_lower[i][0], avgColor[i][1] - c_lower[i][1], avgColor[i][2] - c_lower[i][2]);
//		upperBound = Scalar(avgColor[i][0] + c_upper[i][0], avgColor[i][1] + c_upper[i][1], avgColor[i][2] + c_upper[i][2]);
//
//		//chứa các ảnh nhị phân có kích thước bẳng 1 nửa thôi
//		m->bwList.push_back(Mat(m->srcLR.rows, m->srcLR.cols, CV_8U));
//
//		//tách ra màu cần thiết dựa vào giá trị của trackbar
//		inRange(m->srcLR, lowerBound, upperBound, m->bwList[i]);
//	}
//
//	//m->bw là tổng của tất cả các bwList
//	m->bwList[0].copyTo(m->bw);
//	for (int i = 1; i < NSAMPLES; i++){
//		m->bw += m->bwList[i];
//	}
//	medianBlur(m->bw, m->bw, 7);
//}
//
//// tim 2 đường viền lớn nhất
//int* findBiggestContour(vector<vector<Point> > contours){
//	static int index[2];
//	int indexOfBiggestContour = -1;
//	int indexOfBiggerContour = -1;
//	int sizeOfBiggestContour = 0;
//	int sizeOfBiggerContour = 0;
//
//	for (int i = 0; i < contours.size(); i++){
//		if (contours[i].size() > sizeOfBiggestContour){
//			sizeOfBiggestContour = contours[i].size();
//			indexOfBiggestContour = i;
//		}
//	}
//	for (int i = 0; i < contours.size(); i++){
//		if (contours[i].size() > sizeOfBiggerContour){
//			if (i == indexOfBiggestContour){
//				continue;
//			}
//			else{
//				sizeOfBiggerContour = contours[i].size();
//				indexOfBiggerContour = i;
//			}
//		}
//	}
//	index[0] = indexOfBiggestContour;
//	index[1] = indexOfBiggerContour;
//	return index;
//}
//void myDrawContours(MyImage *m, Shape *sh){
//
//	//ve duong bao ke thang
//	drawContours(m->src, sh->hullPoint, sh->cIdxShape, cv::Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
//	drawContours(m->src, sh->hullPoint, sh->cIdxShape2, cv::Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
//
//	//ve duong bao
//	drawContours(m->src, sh->contourShape, sh->cIdxShape, cv::Scalar(0, 200, 0), 2, 8, vector<Vec4i>(), 0, Point());
//	drawContours(m->src, sh->contourShape, sh->cIdxShape2, cv::Scalar(0, 200, 0), 2, 8, vector<Vec4i>(), 0, Point());
//
//	//ve o vuong bao tay
//	rectangle(m->src, sh->bRectShape.tl(), sh->bRectShape.br(), Scalar(0, 0, 200));
//	rectangle(m->src, sh->bRectShape2.tl(), sh->bRectShape2.br(), Scalar(0, 0, 200));
//
//}
//
//void makeContours(MyImage *m, Shape * sh){
//	Mat aBw;
//	vector<Vec4i> hierarchy;
//	//zoom anh nhi phan co kich thuoc nhu ban dau
//	pyrUp(m->bw, m->bw);
//	//tao 1 ban sao cua anh nhi phan la aBw
//	m->bw.copyTo(aBw);
//	threshold(aBw, aBw, 150, 255, THRESH_BINARY);
//	// remove noise
//	cv::erode(aBw, aBw, cv::Mat());
//	cv::erode(aBw, aBw, cv::Mat());
//	cv::dilate(aBw, aBw, cv::Mat());
//	cv::dilate(aBw, aBw, cv::Mat());
//	//hàm định biên
//	findContours(aBw, sh->contourShape, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	int *a;
//	a = new int(sizeof(int));
//	a = findBiggestContour(sh->contourShape);
//
//	sh->initVectors();
//	sh->cIdxShape = *a;//tim duong vien lon nhat co the y la ban tay
//	sh->cIdxShape2 = *(a + 1); //ban tay thu 2
//
//	if (sh->cIdxShape != -1 && sh->cIdxShape2 != -1){
//		// approxPolyDP( Mat(hg->contours[hg->cIdx]), hg->contours[hg->cIdx], 11, true );
//		// tao 1 hinh vuong to bao trọn đường viền
//		sh->bRectShape = boundingRect(Mat(sh->contourShape[sh->cIdxShape]));
//		sh->bRectShape2 = boundingRect(Mat(sh->contourShape[sh->cIdxShape2]));
//
//		// Find the convex hull object for each contour
//		//Tìm phần vỏ lồi của một điểm đặt.
//		convexHull(Mat(sh->contourShape[sh->cIdxShape]), sh->hullPoint[sh->cIdxShape], false, true);
//		convexHull(Mat(sh->contourShape[sh->cIdxShape2]), sh->hullPoint[sh->cIdxShape2], false, true);
//
//		convexHull(Mat(sh->contourShape[sh->cIdxShape]), sh->hullInt[sh->cIdxShape], false, false);
//		convexHull(Mat(sh->contourShape[sh->cIdxShape2]), sh->hullInt[sh->cIdxShape2], false, false);
//
//		//Hàm xấp xỉ 1 đa giác từ những đường cong , kín
//		approxPolyDP(Mat(sh->hullPoint[sh->cIdxShape]), sh->hullPoint[sh->cIdxShape], 18, true);
//		approxPolyDP(Mat(sh->hullPoint[sh->cIdxShape2]), sh->hullPoint[sh->cIdxShape2], 18, true);
//		//tìm kiếm ra convexityDefects từ convexHull Int
//
//		/*if (hg->contours[hg->cIdx].size()>0){*/
//		if (sh->hullInt[sh->cIdxShape].size() > 3 && sh->hullInt[sh->cIdxShape2].size() > 3){
//			//Bất kỳ độ lệch nào của vật thể từ conour này có thể được coi là khiếm khuyết lồi.
//			convexityDefects(sh->contourShape[sh->cIdxShape], sh->hullInt[sh->cIdxShape], sh->defectShape[sh->cIdxShape]);
//			convexityDefects(sh->contourShape[sh->cIdxShape2], sh->hullInt[sh->cIdxShape2], sh->defectShape[sh->cIdxShape2]);
//		}
//
//		HandGesture hg(m, sh->contourShape[sh->cIdxShape],
//			sh->hullInt[sh->cIdxShape],
//			sh->hullPoint[sh->cIdxShape],
//			sh->defectShape[sh->cIdxShape],
//			sh->bRectShape,
//			sh->frNumber);
//
//		vector <Point> p = hg.getFingerTips(m);
//
//		HandGesture hg2(m, sh->contourShape[sh->cIdxShape2],
//			sh->hullInt[sh->cIdxShape2],
//			sh->hullPoint[sh->cIdxShape2],
//			sh->defectShape[sh->cIdxShape2],
//			sh->bRectShape2,
//			sh->frNumber);
//
//		vector <Point> p2 = hg2.getFingerTips(m);
//
//		sh->drawFingerTips(m, sh, p, p2);
//		myDrawContours(m, sh);
//		//sh->hienThi(m,sh);
//		sh->grabCut(m, sh);
//		//hg->getFingerNumber(m);
//	}
//}
//
//void showWindows(MyImage m){
//	pyrDown(m.bw, m.bw);
//	pyrDown(m.bw, m.bw);
//	Rect roi(Point(3 * m.src.cols / 4, 0), m.bw.size());
//	vector<Mat> channels;
//	Mat result;
//	for (int i = 0; i < 3; i++)
//		channels.push_back(m.bw);
//	merge(channels, result);
//	result.copyTo(m.src(roi));
//	imshow("HandDetect", m.src);
//}
//void printText(Mat src, string text){
//	int fontFace = FONT_HERSHEY_PLAIN;
//	putText(src, text, Point(src.cols / 2, src.rows / 10), fontFace, 1.2f, Scalar(200, 0, 0), 2);
//}
//

#include < stdio.h>
#include < iostream>

#include < opencv2/opencv.hpp>
#include < opencv2/core/core.hpp>
#include < opencv2/highgui/highgui.hpp>
#include < opencv2/video/background_segm.hpp>


#ifdef _DEBUG        
#pragma comment(lib, "opencv_core247d.lib")
#pragma comment(lib, "opencv_imgproc247d.lib")   //MAT processing
#pragma comment(lib, "opencv_objdetect247d.lib") //HOGDescriptor
//#pragma comment(lib, "opencv_gpu247d.lib")
//#pragma comment(lib, "opencv_features2d247d.lib")
#pragma comment(lib, "opencv_highgui247d.lib")
#pragma comment(lib, "opencv_ml247d.lib")
//#pragma comment(lib, "opencv_stitching247d.lib");
//#pragma comment(lib, "opencv_nonfree247d.lib");
#pragma comment(lib, "opencv_video247d.lib")
#else
#pragma comment(lib, "opencv_core247.lib")
#pragma comment(lib, "opencv_imgproc247.lib")
#pragma comment(lib, "opencv_objdetect247.lib")
//#pragma comment(lib, "opencv_gpu247.lib")
//#pragma comment(lib, "opencv_features2d247.lib")
#pragma comment(lib, "opencv_highgui247.lib")
#pragma comment(lib, "opencv_ml247.lib")
//#pragma comment(lib, "opencv_stitching247.lib");
//#pragma comment(lib, "opencv_nonfree247.lib");
#pragma comment(lib, "opencv_video247d.lib")
#endif 

using namespace cv;
using namespace std;



int main()
{

	//global variables
	Mat frame; //current frame
	Mat resizeF;
	Mat fgMaskMOG; //fg mask generated by MOG method
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	Mat fgMaskGMG; //fg mask fg mask generated by MOG2 method


	Ptr< BackgroundSubtractor> pMOG; //MOG Background subtractor
	Ptr< BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
	Ptr< BackgroundSubtractorGMG> pGMG; //MOG2 Background subtractor



	pMOG = new BackgroundSubtractorMOG();
	pMOG2 = new BackgroundSubtractorMOG2();
	pGMG = new BackgroundSubtractorGMG();


	char fileName[100] = "C:\\POSCO\\video\\/cctv 2.mov"; //Gate1_175_p1.avi"; //mm2.avi"; //";//_p1.avi";
	VideoCapture stream1(fileName);   //0 is the id of video device.0 if you have only one camera   

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));

	//unconditional loop   
	while (true) {
		Mat cameraFrame;
		if (!(stream1.read(frame))) //get one frame form video   
			break;

		resize(frame, resizeF, Size(frame.size().width / 4, frame.size().height / 4));
		pMOG->operator()(resizeF, fgMaskMOG);
		pMOG2->operator()(resizeF, fgMaskMOG2);
		pGMG->operator()(resizeF, fgMaskGMG);
		//morphologyEx(fgMaskGMG, fgMaskGMG, CV_MOP_OPEN, element); 




		imshow("Origin", resizeF);
		imshow("MOG", fgMaskMOG);
		imshow("MOG2", fgMaskMOG2);
		imshow("GMG", fgMaskGMG);


		if (waitKey(30) >= 0)
			break;
	}

}