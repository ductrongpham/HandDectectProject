#ifndef _MAIN_HEADER_ 
#define _MAIN_HEADER_ 

#ifdef __linux__
#else

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>

#define _WIN32_WINNT 0x0501 
#include <winuser.h>
#pragma comment(lib,"ws2_32.lib" )
unsigned int __stdcall ServClient(void *data);

#endif

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "handGesture.hpp"
#include "shape.hpp"
#include "myImage.hpp"
#include "roi.hpp"

#define ORIGCOL2COL CV_BGR2HLS 
#define COL2ORIGCOL CV_HLS2BGR
#define NSAMPLES 7 //7 Diem lay mau mau da.
#define PI 3.14159265358979323846 //su dung de tinh goc 

#include <math.h>
#include <string>
#include <vector>
#include <cmath>

#ifdef __linux__

#include <wiringPi.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define LED1 17
#define LED2 27
#define LED3 22
#define LED4 13
#define LED5 19
#define LED_NO 26

#endif

using namespace std;
using namespace cv;


int flagOn = 0; //cờ thông báo bắt đàu thực hiện sau khi nhấn start
int fontFace = FONT_HERSHEY_PLAIN; //font chữ
int square_len; //kich thước của 1 ROI
int avgColor[NSAMPLES][3]; // chưa lần lượt bộ 3 tham số HLS của từng ô mẫu màu
int c_lower[NSAMPLES][3]; //chỉ số lơn nhất lần lượt của các tham số HLS	
int c_upper[NSAMPLES][3]; //chỉ số tối thiểu lần lượt của các tham số HLS
int threshold_bar = 0;

vector <MyRoi> roi;

//chờ nhận mẫu màu từ lòng bàn tay
void waitForPalmCover(MyImage* m);

//tính toán màu thu thập đc
void average(MyImage *m);

//Tạo ra thanh công cụ trackbars để tùy chỉnh các tham số HLS
void initWindows();
void initTrackbars();

//tạo ảnh nhị phân
void produceBinaries(MyImage *m);

//tìm ra đường biên lớn nhất (bao gồm 2 đường biên lớn nhất)
int* findBiggestContour(vector<vector<Point> > contours);

//tính toán giá trị median
int getMedian(vector<int> val);

//Hiển thị text
void printText(Mat src, string text);

//laấy ra giá trị trung bình của HLS
void getAvgColor(MyRoi roi, int avg[3]);

//tiêu chuẩn hóa màu
void normalizeColors();

//Hiển thị cửa sổ
void showWindows(MyImage m);

//tạo đường bao cơ bản
void makeContours(MyImage *m, Shape *sh, HandGesture *hg1, HandGesture *hg2);

//phan tich va ve duong bao
void analyContour(MyImage *m, Shape *sh, int idxSh, int &fr, HandGesture hg);

//Hàm trả về tỉ lệ sai lệch giữa ảnh nền và ảnh hiện tại (tỉ lệ phần trăm càng cao độ sai lệch càng ít)
float returnPercentDiff(Mat src_base, Mat src_test1);

//hàm đổi bool thành string
string bool2string(bool tf);

//hàm sử lí sự kiện khi t nhấp chuột vào start
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

//hàm xử lí loại bỏ background
Mat returnImage(Mat frameCurrent, Mat framePrev);

Mat returnImagePrev(MyImage *m);

void waitStartProgress(MyImage m);

#ifdef __linux__
void sig_handler(int signo);
void error(const char *msg);
struct PointCenter
{
	int x; //Tọa độ x
	int y; //Tọa độ y
	int fingerNum; //Số ngón tay
};
void progressLinux(int argc, char *argv[]);
#else
// Di chuyển chuột
void MouseMove(int dx, int dy);
// Chuột trái ấn
void MouseLeftBtnDown();
// Chuột trái nhả
void MouseLeftBtnUp();
// Chuột phải ấn
void MouseRightBtnDown();
// Chuột phải nhả
void MouseRightBtnUp();
void progressWindows();
void controlCursor(int &flagC, int &flagM, int xx, int yy, int xcu, int ycu, HandGesture hg1);

#endif
#endif