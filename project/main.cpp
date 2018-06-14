#include "main.hpp"
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include <ctype.h>
#ifdef __linux__
#else
// Positive dx value means move right, Negative dx value means move left
// Positive dy value means move down, Negative dy value means move right
void MouseMove(int dx, int dy)
{
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE; //|MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = dx;
	Input.mi.dy = dy;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseLeftBtnDown()
{
	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseLeftBtnUp()
{
	INPUT    Input = { 0 };
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseRightBtnDown()
{
	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseRightBtnUp()
{
	INPUT Input = { 0 };
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void controlCursor(int &flagC, int & flagLC, int xx, int yy, int xcu, int ycu, HandGesture hg1){
	//move
	if (hg1.isHand && hg1.mostFrequentFingerNumber == 1){
		int dx = xx - xcu;
		int dy = yy - ycu;
		if (!(abs(dx) <= 2 && abs(dy) <= 2))
			MouseMove(4 * dx, 4 * dy);

		//reset flag click
		flagC = 0;
	}

	//press down mouse
	if (hg1.mostFrequentFingerNumber == 2 && hg1.isHand){
		//reset flag longclick
		Sleep(50);
		MouseLeftBtnUp();
		flagLC = 0;
	}

	//rightclick
	if (hg1.mostFrequentFingerNumber == 3 && hg1.isHand && flagC == 0){
		flagC = 1;
		MouseRightBtnDown();
		cout << "click right mouse " << endl;
		Sleep(50);
		MouseRightBtnUp();
	}

	//leftclick
	if (hg1.mostFrequentFingerNumber == 4 && hg1.isHand && flagC == 0){
		flagC = 1; // them co de xac dinh la muon click thi phai tro ve 1 moi click dc
		MouseLeftBtnDown();
		cout << "click left mouse " << endl;
		Sleep(50);
		MouseLeftBtnUp();
	}

	//longclick
	if (hg1.mostFrequentFingerNumber == 5 && hg1.isHand && flagLC == 0){
		MouseLeftBtnDown();
		cout << "long click left mouse " << endl;
		flagLC = 1;
	}

}

#endif


Mat returnImagePrev(VideoCapture cap){
	Mat framePrev;
	Mat src;
	for (int i = 0; i < 40; i++){
		cap >> src;
#ifdef __linux__
		flip(src, src, 0);
#else
		flip(src, src, 1);
#endif
		if (i <= 20){
			putText(src, "Sample image", Point(src.cols / 5, src.rows / 2), FONT_HERSHEY_PLAIN, 1.5f, Scalar(41, 0, 223), 2);
		}
		else{
			putText(src, "Sample image", Point(src.cols / 5, src.rows / 2), FONT_HERSHEY_PLAIN, 1.5f, Scalar(191, 178, 0), 2);
		}
		if (i == 20){
			cap >> framePrev;
#ifdef __linux__
			flip(framePrev, framePrev, 0);
#else
			flip(framePrev, framePrev, 1);
#endif
		}
		imshow("Sample Image", src);
		moveWindow("Sample Image", 100, 100);
		if (cv::waitKey(30) == char('q')) break;
	}
	destroyWindow("Sample Image");
	return framePrev;
}

Mat returnImage(Mat frameCurrent, Mat framePrev){

	cv::Mat diffImage;
	Mat anhGrab;
	cv::absdiff(framePrev, frameCurrent, diffImage);

	cv::Mat foregroundMask = cv::Mat::zeros(frameCurrent.rows, frameCurrent.cols, CV_8UC1);

	float threshold1 = 30.0f;
	float dist;

	for (int j = 0; j < diffImage.rows; ++j)
	for (int i = 0; i < diffImage.cols; ++i)
	{
		cv::Vec3b pix = diffImage.at<cv::Vec3b>(j, i);

		dist = (pix[0] * pix[0] + pix[1] * pix[1] + pix[2] * pix[2]);
		dist = sqrt(dist);

		if (dist> threshold1)
		{
			foregroundMask.at<unsigned char>(j, i) = 255;
		}
	}
	//cv::threshold(diffImage, foregroundMask, 30, 255.0, CV_THRESH_BINARY);
	//adaptiveThreshold(foregroundMask, foregroundMask, 255,/* ADAPTIVE_THRESH_MEAN_C*/ ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 15, 0);
	GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5, 4);
	//bilateralFilter
	threshold(foregroundMask, foregroundMask, threshold_bar, 255, THRESH_BINARY);
	cv::erode(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::erode(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	frameCurrent.copyTo(anhGrab, foregroundMask);
	//imshow("anh grab", anhGrab);
	//////////////////////////////////////////////////////////////////////////////////////////////
	//cv::Mat imgFrame1Copy = framePrev.clone();
	//cv::Mat imgFrame2Copy = frameCurrent.clone();

	//cv::Mat imgDifference;
	//cv::Mat imgThresh;

	//cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
	//cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

	//cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
	//cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);

	//cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

	//cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

	//cv::imshow("imgThresh", imgThresh);

	//cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	//cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	//cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	//cv::Mat structuringElement9x9 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));

	//cv::dilate(imgThresh, imgThresh, structuringElement5x5);
	//cv::dilate(imgThresh, imgThresh, structuringElement5x5);
	//cv::erode(imgThresh, imgThresh, structuringElement5x5);

	//GaussianBlur(imgThresh, imgThresh, Size(11, 11), 3.5, 3.5);
	//threshold(imgThresh, imgThresh, threshold_bar, 255, THRESH_BINARY);
	//cv::erode(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//cv::dilate(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//cv::dilate(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//cv::erode(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//frameCurrent.copyTo(anhGrab, imgThresh);
	/////////////////////////////////////////////////////////////////////////////////////////////
	return anhGrab;
}

void drawHist(String name, Mat src){

	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, histImage);
}

Mat canBangHistogram(Mat &imageSrc){
	Mat imageHsv, imageDst;

	cvtColor(imageSrc, imageHsv, CV_BGR2HSV);

	vector<Mat> hsvChannels;
	// Tách imageHsv thành 3 kênh màu
	split(imageHsv, hsvChannels);

	// Cân bằng histogram kênh màu v (Value)
	equalizeHist(hsvChannels[2], hsvChannels[2]);

	// Trộn ảnh
	merge(hsvChannels, imageHsv);

	// Chuyển đổi HSV sang RGB để hiển thị
	cvtColor(imageHsv, imageDst, CV_HSV2BGR);

	return imageDst;
}

//hàm sử lí sự kiện khi t nhấp chuột vào start
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	MyImage* m = (MyImage*)userdata;
	Point p;
	if (event == EVENT_LBUTTONDOWN)
	{
		p.x = x;
		p.y = y;
		int gioihan;

#ifdef __linux__
		gioihan = 80;
#else
		gioihan = 120;
#endif

		if (p.x <= (m->src.cols) && p.x >= (m->src.cols - gioihan))
		{
			if (p.y <= (gioihan) && p.y >= (0)){
				flagOn = 1;
				rectangle(m->src, Point(m->src.cols - gioihan, 0), Point(m->src.cols, gioihan), Scalar(249, 244, 0), CV_FILLED);
#ifdef __linux__
				putText(m->src, "START", Point(m->src.cols - 70, 50), fontFace, 1.2f, Scalar(255, 255, 255), 2);
#else
				putText(m->src, "START", Point(m->src.cols - 92, 70), fontFace, 1.5f, Scalar(255, 255, 255), 2);
#endif
				imshow("Wait", m->src);
				moveWindow("Wait", 100, 100);
				waitKey(350);
				destroyWindow("Wait");

			}
		}
		else{
			cout << "not start" << endl;
		}
	}
}
void waitStartProgress(MyImage m){
	while (flagOn != 1){
		m.cap >> m.src;

#ifdef __linux__
		flip(m.src, m.src, 0);
		MyRoi roiStart(Point(m.src.cols - 80, 0), Point(m.src.cols, 80), m.src);
		vector <MyRoi> roi7;
		roiStart.drawRect(m.src);
		putText(m.src, "START", Point(m.src.cols - 70, 50), fontFace, 1.2f, Scalar(200, 0, 0), 2);
#else
		flip(m.src, m.src, 1);
		MyRoi roiStart(Point(m.src.cols - 120, 0), Point(m.src.cols, 120), m.src);
		vector <MyRoi> roi7;
		roiStart.drawRect(m.src);
		putText(m.src, "START", Point(m.src.cols - 92, 70), fontFace, 1.5f, Scalar(200, 0, 0), 2);
#endif
		//vẽ ra 7ROI để lấy mẫu màu da để tính toán ra các tham số HLS
		roi7.push_back(MyRoi(Point(m.src.cols / 3 - 40, m.src.rows / 6), Point(m.src.cols / 3 - 40 + square_len, m.src.rows / 6 + square_len), m.src));
		roi7.push_back(MyRoi(Point(m.src.cols / 4 - 40, m.src.rows / 2), Point(m.src.cols / 4 - 40 + square_len, m.src.rows / 2 + square_len), m.src));
		roi7.push_back(MyRoi(Point(m.src.cols / 3 - 40, m.src.rows / 1.5), Point(m.src.cols / 3 - 40 + square_len, m.src.rows / 1.5 + square_len), m.src));
		roi7.push_back(MyRoi(Point(m.src.cols / 2 - 40, m.src.rows / 2), Point(m.src.cols / 2 - 40 + square_len, m.src.rows / 2 + square_len), m.src));
		roi7.push_back(MyRoi(Point(m.src.cols / 2.5 - 40, m.src.rows / 2.5), Point(m.src.cols / 2.5 - 40 + square_len, m.src.rows / 2.5 + square_len), m.src));
		roi7.push_back(MyRoi(Point(m.src.cols / 2 - 40, m.src.rows / 1.5), Point(m.src.cols / 2 - 40 + square_len, m.src.rows / 1.5 + square_len), m.src));
		roi7.push_back(MyRoi(Point(m.src.cols / 2.5 - 40, m.src.rows / 1.8), Point(m.src.cols / 2.5 - 40 + square_len, m.src.rows / 1.8 + square_len), m.src));
		
		Point u_corner(m.src.cols / 2.5 - 40, m.src.rows / 1.8);
		Point l_corner(m.src.cols / 2.5 - 40 + square_len, m.src.rows / 1.8 + square_len);

		mySelection.x = u_corner.x;
		mySelection.y = u_corner.y;
		mySelection.width = std::abs(l_corner.x - u_corner.x);
		mySelection.height = std::abs(l_corner.y - u_corner.y);

		for (int j = 0; j < NSAMPLES; j++){
			roi7[j].drawRect(m.src);
		}
		imshow("Wait", m.src);
		moveWindow("Wait", 100, 100);
		if (cv::waitKey(30) == char('q')) break;
		roi7.clear();
	}
}

void waitForPalmCover(MyImage *m){

	m->cap >> m->src; // truyen anh tu camera vao;

#ifdef __linux__
	flip(m->src, m->src, 0); // sử dụng để lật hình với trục y(1)
#else
	flip(m->src, m->src, 1); // sử dụng để lật hình với trục y(1)
#endif

	//Vẽ 7 khung ảnh nhỏ ddeeer nhận mẫu
	roi.push_back(MyRoi(Point(m->src.cols / 3 - 40, m->src.rows / 6), Point(m->src.cols / 3 - 40 + square_len, m->src.rows / 6 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 4 - 40, m->src.rows / 2), Point(m->src.cols / 4 - 40 + square_len, m->src.rows / 2 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 3 - 40, m->src.rows / 1.5), Point(m->src.cols / 3 - 40 + square_len, m->src.rows / 1.5 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2 - 40, m->src.rows / 2), Point(m->src.cols / 2 - 40 + square_len, m->src.rows / 2 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2.5 - 40, m->src.rows / 2.5), Point(m->src.cols / 2.5 - 40 + square_len, m->src.rows / 2.5 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2 - 40, m->src.rows / 1.5), Point(m->src.cols / 2 - 40 + square_len, m->src.rows / 1.5 + square_len), m->src));
	roi.push_back(MyRoi(Point(m->src.cols / 2.5 - 40, m->src.rows / 1.8), Point(m->src.cols / 2.5 - 40 + square_len, m->src.rows / 1.8 + square_len), m->src));

	for (int i = 0; i < 30; i++){
		m->cap >> m->src;

#ifdef __linux__
		flip(m->src, m->src, 0);
#else
		flip(m->src, m->src, 1);
#endif

		for (int j = 0; j < NSAMPLES; j++){
			roi[j].drawRect(m->src);
		}
		//hiển thị thông tin
		string imgText = string("Cover rectangles with palm");
		printText(m->src, imgText);
		imshow("WaitPalm", m->src);
		moveWindow("WaitPalm", 100, 100);
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
	roi.roiPtr.copyTo(r); //copy 1 ROI vao r
	vector<int> hm;
	vector<int> sm;
	vector<int> lm;
	// generate vectors
	// truyền lần lượt các giá trị của H L S vào 
	//for (int i = 2; i < r.rows - 2; i++){
	//	for (int j = 2; j < r.cols - 2; j++){
	//		hm.push_back(r.data[r.channels()*(r.cols*i + j) + 0]);
	//		sm.push_back(r.data[r.channels()*(r.cols*i + j) + 1]);
	//		lm.push_back(r.data[r.channels()*(r.cols*i + j) + 2]);
	//	}
	//}
	for (int i = 0; i < r.rows; i++){
		for (int j = 0; j < r.cols; j++){
			Vec3b hsv = r.at<Vec3b>(i, j);
			hm.push_back(hsv.val[0]);
			sm.push_back(hsv.val[1]);
			lm.push_back(hsv.val[2]);
		}
	}
	//int H = hsv.val[0]; //hue
	//int S = hsv.val[1]; //saturation
	//int V = hsv.val[2]; //value

	avg[0] = getMedian(hm);
	avg[1] = getMedian(sm);
	avg[2] = getMedian(lm);
}

// tính toán ra giá trị H L S của 7 ô đc lưu trong avgColor
// khi tính toán xong 7 ô sẽ chuyển sang màu trắng
void average(MyImage *m){
	m->cap >> m->src;

#ifdef __linux__
	flip(m->src, m->src, 0);
#else
	flip(m->src, m->src, 1);
#endif

	for (int i = 0; i < 10; i++){
		m->cap >> m->src;

#ifdef __linux__
		flip(m->src, m->src, 0);
#else
		flip(m->src, m->src, 1);
#endif

		cvtColor(m->src, m->src, ORIGCOL2COL);// chuyển từ RGB sang HLS để tách ra tham số HLS // cho doi mau cua o tu xanh la thanh trang
		for (int j = 0; j < NSAMPLES; j++){
			getAvgColor(roi[j], avgColor[j]); //avgColor[NSAMPLES][3]
			roi[j].drawRect(m->src);
		}
		cvtColor(m->src, m->src, COL2ORIGCOL);
		string imgText = string("Finding average color of hand");
		printText(m->src, imgText);
		imshow("WaitPalm", m->src);
		moveWindow("WaitPalm", 100, 100);
		if (cv::waitKey(30) >= 0) break;
	}
}

void printText(Mat src, string text){
	int fontFace = FONT_HERSHEY_PLAIN;
	putText(src, text, Point(src.cols / 2, src.rows / 10), fontFace, 1.2f, Scalar(200, 0, 0), 2);
}

void inittrackbar(){

	//các tham số cơ bản của màu da tay
	for (int i = 0; i < NSAMPLES; i++){
		c_lower[i][0] = 20;
		c_upper[i][0] = 100;
		c_lower[i][1] = 30;
		c_upper[i][1] = 255;
		c_lower[i][2] = 15;
		c_upper[i][2] = 255;
	}
	threshold_bar = 150;
	createTrackbar("H-Upper", "trackbar", &c_upper[0][0], 180);
	createTrackbar("H-Lower", "trackbar", &c_lower[0][0], 180);
	createTrackbar("S-Upper", "trackbar", &c_upper[0][1], 255);
	createTrackbar("S-Lower", "trackbar", &c_lower[0][1], 255);
	createTrackbar("L-Upper", "trackbar", &c_upper[0][2], 255);
	createTrackbar("L-Lower", "trackbar", &c_lower[0][2], 255);
	createTrackbar("Threshsold", "trackbar", &threshold_bar, 255);
}
//tiêu chuẩn màu sắc
void normalizeColors(){
	// copy all boundaries read from trackbar
	// to all of the different boundaries
	for (int i = 1; i < NSAMPLES; i++){ // tại vì các giá trị vào được để trong 0
		for (int j = 0; j < 3; j++){
			c_lower[i][j] = c_lower[0][j];
			c_upper[i][j] = c_upper[0][j];
		}
	}
	// normalize all boundaries so that 
	// threshold is within 0-255
	for (int i = 0; i < NSAMPLES; i++){
		//lower: avgColor - c_lower < 0		thì sẽ bằng 0
		if ((avgColor[i][0] - c_lower[i][0]) < 0){
			c_lower[i][0] = avgColor[i][0];
		}
		if ((avgColor[i][1] - c_lower[i][1]) < 0){
			c_lower[i][1] = avgColor[i][1];
		}
		if ((avgColor[i][2] - c_lower[i][2]) <0){
			c_lower[i][2] = avgColor[i][2];
		}

		//upper: avgColor + c_upper > 255	thì sẽ bằng 255
		if ((avgColor[i][0] + c_upper[i][0]) >255){
			c_upper[i][0] = 255 - avgColor[i][0];
		}
		if ((avgColor[i][1] + c_upper[i][1]) > 255){
			c_upper[i][1] = 255 - avgColor[i][1];
		}
		if ((avgColor[i][2] + c_upper[i][2]) > 255){
			c_upper[i][2] = 255 - avgColor[i][2];
		}
	}
}

void produceBinaries(MyImage *m){
	Scalar lowerBound;
	Scalar upperBound;
	for (int i = 0; i < NSAMPLES; i++){

		normalizeColors();

		//avgColor - c_lower < 0	thì sẽ bằng 0
		//avgColor + c_upper > 255	thì sẽ bằng 255
		// còn lại nếu thỏa mãn thì lấy giá trị
		lowerBound = Scalar(avgColor[i][0] - c_lower[i][0], avgColor[i][1] - c_lower[i][1], avgColor[i][2] - c_lower[i][2]);
		upperBound = Scalar(avgColor[i][0] + c_upper[i][0], avgColor[i][1] + c_upper[i][1], avgColor[i][2] + c_upper[i][2]);

		//chứa các ảnh nhị phân có kích thước bẳng 1 nửa thôi
		m->bwList.push_back(Mat(m->srcLR.rows, m->srcLR.cols, CV_8U));

		//tách ra màu cần thiết dựa vào giá trị của trackbar
		inRange(m->srcLR, lowerBound, upperBound, m->bwList[i]);
	}

	//m->bw là tổng của tất cả các bwList
	m->bwList[0].copyTo(m->bw);
	for (int i = 1; i < NSAMPLES; i++){
		m->bw += m->bwList[i];
	}
	medianBlur(m->bw, m->bw, NSAMPLES); // làm mờ để lọc nhiễu
}

void drawRectCursor(Mat src,Mat bw,int xx,int yy,int xcc,int ycc){
	
	int w = src.size().width - bw.size().width / 4;
	int h = src.size().height - bw.size().height / 4;

	Rect roiCursor(0, bw.size().height / 4,w,h);
	cv::rectangle(src, roiCursor, cv::Scalar(0, 255, 0));
	Point centerCursor(roiCursor.size().width / 2, bw.size().height / 4 + roiCursor.size().height / 2);
	int wc = 40;
	int hc = 40;

	//if (0 < xx && xx < w){
	//	if ((src.size().height-h) < yy && yy < src.size().height){
	//		////ve o vuong
	//		//int xc = centerCursor.x;
	//		//int yc = centerCursor.y;
	//		//cv::rectangle(src, cvPoint(xc - wc / 2, yc - hc / 2), cvPoint(xc + wc / 2, yc + hc / 2), Scalar(0, 200, 0), 1, 8);
	//	}
	//}
}

void showWindows(Mat src, Mat bw){
	pyrDown(bw, bw);
	pyrDown(bw, bw);
	Rect roi(Point(3 * src.cols / 4, 0), bw.size());
	vector<Mat> channels;
	Mat result;
	for (int i = 0; i < 3; i++)
		channels.push_back(bw);
	merge(channels, result);
	result.copyTo(src(roi));
	imshow("HandDetect", src);
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
	//for (int i = 0; i < contours.size(); i++){
	//	if (contours[i].size() > sizeOfBiggerContour){
	//		if (i == indexOfBiggestContour){
	//			continue;
	//		}
	//		else{
	//			sizeOfBiggerContour = contours[i].size();
	//			indexOfBiggerContour = i;
	//		}
	//	}
	//}
	index[0] = indexOfBiggestContour;
	//index[1] = indexOfBiggerContour;
	return index;
}

void analyContour(Mat src, Shape *sh, int idxSh, int &fr, HandGesture *hg){
	Rect bRectSh = boundingRect(Mat(sh->contourShape[idxSh]));

	// Find the convex hull object for each contour
	//Tìm phần vỏ lồi của một điểm đặt.
	//tìm tập hợp hullPoint
	convexHull(Mat(sh->contourShape[idxSh]), sh->hullPoint[idxSh], false, true);
	//tìm tập hợp hullInt--được sử dụng để tìm convexityDefects
	convexHull(Mat(sh->contourShape[idxSh]), sh->hullInt[idxSh], false, false);

	//Hàm xấp xỉ 1 đa giác từ những đường cong , kín
	approxPolyDP(Mat(sh->hullPoint[idxSh]), sh->hullPoint[idxSh], 18, true);

	//tìm kiếm ra convexityDefects từ convexHull Int
	if (sh->hullInt[idxSh].size() > 3){
		//Bất kỳ độ lệch nào của vật thể từ conour này có thể được coi là khiếm khuyết lồi.
		convexityDefects(sh->contourShape[idxSh], sh->hullInt[idxSh], sh->defectShape[idxSh]);
	}
	//thiết lập tham số cơ bản cho việc defect bàn tay
	hg->setHandGesture(sh->contourShape[idxSh],
		sh->hullInt[idxSh],
		sh->hullPoint[idxSh],
		sh->defectShape[idxSh],
		bRectSh,
		fr);

	hg->eliminateDefects();

	bool ishand = hg->detectIfHand(src);

	if (ishand){
		hg->getFingerNumber(src, fr);

		//putText(m->src, bool2string(ishand), Point(80, 80), fontFace, 2.0f, Scalar(255, 255, 255));
		vector <Point> p = hg->getFingerTips(src);
		//ve cac diem dau ngon tay
		sh->drawFingerTips(src, sh, idxSh, p);
		//ve duong convex hull
		//drawContours(src, sh->hullPoint, idxSh, cv::Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
		//ve duong bao
		//drawContours(src, sh->contourShape, idxSh, cv::Scalar(0, 200, 0), 2, 8, vector<Vec4i>(), 0, Point());
		//ve o vuong bao tay
		rectangle(src, bRectSh.tl(), bRectSh.br(), Scalar(0, 0, 200));
	}
}

void makeContours(Mat src, Mat bw, Shape * sh, HandGesture *hg1){
	Mat aBw;
	double currentArea;
	vector<Vec4i> hierarchy;

	//zoom anh nhi phan co kich thuoc nhu ban dau
	//pyrUp(bw, bw);

	//tao 1 ban sao cua anh nhi phan la aBw
	bw.copyTo(aBw);

	GaussianBlur(aBw, aBw, Size(11, 11), 3.5, 3.5);
	threshold(aBw, aBw, threshold_bar, 255, THRESH_BINARY);
	cv::erode(aBw, aBw, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(aBw, aBw, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(aBw, aBw, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::erode(aBw, aBw, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//hàm định biên
	findContours(aBw, sh->contourShape, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	int *a;
	a = new int(sizeof(int));
	a = findBiggestContour(sh->contourShape);
	//if (*a == (-1)){
	//	hg1->mostFrequentFingerNumber = 0;
	//}
	sh->initVectors();
	sh->cIdxShape = *a;//tim duong vien lon nhat co the y la ban tay

	Mat imgClone;
	src.copyTo(imgClone);

	if (sh->cIdxShape != -1){
		//lấy diện tích của đường định biên
		currentArea = contourArea(sh->contourShape[sh->cIdxShape]);
		if (currentArea > 2500){
			analyContour(src, sh, sh->cIdxShape, sh->frNumber, hg1);
		}
		//else{
		//	hg1->mostFrequentFingerNumber = 0;
		//}
	}

	//sh->cIdxShape2 = *(a + 1); //ban tay thu 2
	/*
	if (sh->cIdxShape2 != -1){
	currentArea = contourArea(sh->contourShape[sh->cIdxShape2]);
	if (currentArea > 5000){
	analyContour(m, sh, sh->cIdxShape2, sh->frNumber2, hg2);
	}
	}
	*/
}

class SkinDetector
{
public:
	SkinDetector(void);
	~SkinDetector(void);
	cv::Mat getSkin(cv::Mat input);
	void initTrackBarYCrCb();
	int threshold_barYCrCb;
private:
	int Y_MIN;
	int Y_MAX;
	int Cr_MIN;
	int Cr_MAX;
	int Cb_MIN;
	int Cb_MAX;
};

void SkinDetector::initTrackBarYCrCb(){
	createTrackbar("Y-Upper", "trackbarYCrCb", &Y_MAX, 255);
	createTrackbar("Y-Lower", "trackbarYCrCb", &Y_MIN, 255);
	createTrackbar("Cr-Upper", "trackbarYCrCb", &Cr_MAX, 255);
	createTrackbar("Cr-Lower", "trackbarYCrCb", &Cr_MIN, 255);
	createTrackbar("Cb-Upper", "trackbarYCrCb", &Cb_MAX, 255);
	createTrackbar("Cb-Lower", "trackbarYCrCb", &Cb_MIN, 255);
	createTrackbar("Threshsold", "trackbarYCrCb", &threshold_barYCrCb, 255);
}


SkinDetector::SkinDetector(void)
{
	//YCrCb threshold
	// You can change the values and see what happens
	Y_MIN = 0;
	Y_MAX = 255;
	Cr_MIN = 133;
	Cr_MAX = 173;
	Cb_MIN = 77;
	Cb_MAX = 127;
	threshold_barYCrCb = 150;
}

SkinDetector::~SkinDetector(void)
{
}

//this function will return a skin masked image
cv::Mat SkinDetector::getSkin(cv::Mat input)
{
	cv::Mat skin;
	//first convert our RGB image to YCrCb
	cv::cvtColor(input, skin, cv::COLOR_BGR2YCrCb);
	vector<Mat> Channels;
	//filter the image in YCrCb color space
	cv::inRange(skin, cv::Scalar(Y_MIN, Cr_MIN, Cb_MIN), cv::Scalar(Y_MAX, Cr_MAX, Cb_MAX), skin);
	medianBlur(skin, skin, 1);
	GaussianBlur(skin, skin, Size(11, 11), 3.5, 3.5);
	threshold(skin, skin, threshold_barYCrCb, 255, THRESH_BINARY);
	cv::erode(skin, skin, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(skin, skin, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::dilate(skin, skin, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::erode(skin, skin, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	return skin;
}

//tạo hàm bắt sự kiện khi nhấn Ctrl+C
#ifdef __linux__
void sig_handler(int signo){
	if (signo == SIGINT){
		cout << "Quit program clear GPIO" << endl;
		digitalWrite(LED1, 0);
		digitalWrite(LED2, 0);
		digitalWrite(LED3, 0);
		digitalWrite(LED4, 0);
		digitalWrite(LED5, 0);
		digitalWrite(LED_NO, 0);

		pinMode(LED1, INPUT);
		pinMode(LED2, INPUT);
		pinMode(LED3, INPUT);
		pinMode(LED4, INPUT);
		pinMode(LED5, INPUT);
		pinMode(LED_NO, INPUT);
	}
}

void error(const char *msg)
{
	perror(msg);
	exit(0);
}
#endif


double getOrientation(vector<Point> &pts, Mat &img)
{
	//Construct a buffer used by the pca analysis
	Mat data_pts = Mat(pts.size(), 2, CV_64FC1);
	for (int i = 0; i < data_pts.rows; ++i)
	{
		data_pts.at<double>(i, 0) = pts[i].x;
		data_pts.at<double>(i, 1) = pts[i].y;
	}
	//Perform PCA analysis
	PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);

	//Store the position of the object
	Point pos = Point(pca_analysis.mean.at<double>(0, 0),
		pca_analysis.mean.at<double>(0, 1));

	//Store the eigenvalues and eigenvectors
	vector<Point2d> eigen_vecs(2);
	vector<double> eigen_val(2);
	for (int i = 0; i < 2; ++i)
	{
		eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
		pca_analysis.eigenvectors.at<double>(i, 1));

		eigen_val[i] = pca_analysis.eigenvalues.at<double>(i,0);
	}

	// Draw the principal components
	circle(img, pos, 3, CV_RGB(255, 0, 255), 2);
	line(img, pos, pos + 0.02 * Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]), CV_RGB(255, 255, 0));
	line(img, pos, pos + 0.02 * Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]), CV_RGB(0, 255, 255));

	return atan2(eigen_vecs[0].y, eigen_vecs[0].x);
}

void handDect(Mat &m){
	Mat m1;
	m.copyTo(m1);
	CascadeClassifier Palm;
	vector<Rect> palm;
	Palm.load("Hand.Cascade.1.xml");
	pyrDown(m1, m1);
	pyrDown(m1, m1);
	Palm.detectMultiScale(m1, palm, 1.1, 3, 0 | CV_HAAR_FIND_BIGGEST_OBJECT, Size(30, 30));
	Point sh;
	if (palm.size()){
		sh.x = (palm[0].br().x + palm[0].tl().x) / 2;
		sh.y = (palm[0].br().y + palm[0].tl().y) / 2;
		putText(m, "Hand", Point(sh), FONT_HERSHEY_PLAIN, 1.5f, Scalar(41, 0, 223), 2);
	}
		
}

// camshift
void camShiftDemo(Mat &src, Mat &imageHLS, Mat& maskHLS,Mat &hue,Mat &hist, const float * phranges, int &hsize, Rect &trackWindow,bool &flagCam){

	Mat backproj;
	int ch[] = { 0, 0 };
	hue.create(imageHLS.size(), imageHLS.depth());
	mixChannels(&imageHLS, 1, &hue, 1, ch, 1);

	if (flagCam){
		Mat mroi(hue, mySelection), maskroi(maskHLS, mySelection);
		calcHist(&mroi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
		normalize(hist, hist, 0, 255, CV_MINMAX);
		trackWindow = mySelection;
		flagCam = false;
	}
	
	calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
	backproj &= maskHLS;
	RotatedRect trackBox = CamShift(backproj, trackWindow,TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));
	if (trackWindow.area() <= 1)
	{
		int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5) / 6;
		trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,trackWindow.x + r, trackWindow.y + r) & Rect(0, 0, cols, rows);
	}
	int angle = trackBox.angle;
	Size rect_size = trackBox.size;
	if (angle >90){
		angle -= 180;
		angle *= -1;
	}

	//Mat M = getRotationMatrix2D(trackBox.center, angle, 1.0);
	//Mat rotatedMask;
	//Mat croppedMask;
	//warpAffine(backproj, rotatedMask, M, backproj.size(), INTER_CUBIC);
	//getRectSubPix(rotatedMask, rect_size, trackBox.center, croppedMask);
	//threshold(croppedMask, croppedMask,150, 255, THRESH_BINARY);
	//putText(src, "br", Point(trackWindow.br()), FONT_HERSHEY_PLAIN, 1.5f, Scalar(41, 0, 223), 2);
	//putText(src, "tl", Point(trackWindow.tl()), FONT_HERSHEY_PLAIN, 1.5f, Scalar(41, 0, 223), 2);
	//Point sh;
	//sh.x = (trackWindow.br().x + trackWindow.tl().x) / 2;
	//sh.y = (trackWindow.br().y + trackWindow.tl().y) / 2;
	//putText(src, "Hand", Point(sh), FONT_HERSHEY_PLAIN, 1.0f, Scalar(0, 200, 0), 2);
	rectangle(src, trackWindow, Scalar(0, 200, 0), 2, 8);
	//ellipse(src, trackBox, Scalar(0,200,0), 2, CV_AA);
}

//void imadjust(const Mat& src, Mat& dst, int tol = 1, Vec2i in = Vec2i(0, 255), Vec2i out = Vec2i(0, 255))
//{
//	// src : input CV_8UC1 image
//	// dst : output CV_8UC1 imge
//	// tol : tolerance, from 0 to 100.
//	// in  : src image bounds
//	// out : dst image buonds
//
//	dst = src.clone();
//
//	tol = max(0, min(100, tol));
//
//	if (tol > 0)
//	{
//		// Compute in and out limits
//
//		// Histogram
//		vector<int> hist(256, 0);
//		for (int r = 0; r < src.rows; ++r) {
//			for (int c = 0; c < src.cols; ++c) {
//				hist[src.at<unsigned char>(r, c)]++;
//			}
//		}
//
//		// Cumulative histogram
//		vector<int> cum = hist;
//		for (int i = 1; i < hist.size(); ++i) {
//			cum[i] = cum[i - 1] + hist[i];
//		}
//
//		// Compute bounds
//		int total = src.rows * src.cols;
//		int low_bound = total * tol / 100;
//		int upp_bound = total * (100 - tol) / 100;
//		in[0] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), low_bound));
//		in[1] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), upp_bound));
//
//	}
//
//	// Stretching
//	float scale = float(out[1] - out[0]) / float(in[1] - in[0]);
//	for (int r = 0; r < dst.rows; ++r)
//	{
//		for (int c = 0; c < dst.cols; ++c)
//		{
//			int vs = max(src.at<unsigned char>(r, c) - in[0], 0);
//			int vd = min(int(vs * scale + 0.5f) + out[0], out[1]);
//			dst.at<unsigned char>(r, c) = saturate_cast<uchar>(vd);
//		}
//	}
//}
//
//Mat returnImAdjust(Mat src){
//	vector<Mat> planes;
//	split(src, planes);
//	for (int i = 0; i < 3; ++i)
//	{
//		imadjust(planes[i], planes[i]);
//	}
//	Mat result;
//	merge(planes, result);
//	return result;
//}


int main()
{
	MyImage m(0);
#ifdef __linux__
	system("sudo modprobe bcm2835-v4l2");

	system("sudo modprobe bcm2835-v4l2");
	wiringPiSetupGpio();
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED5, OUTPUT);
	pinMode(LED_NO, OUTPUT);

	if (signal(SIGINT, sig_handler) == SIG_ERR){
		cout << "Not clear GPIO" << endl;
	}

	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(2222);
	serv.sin_addr.s_addr = inet_addr("192.168.1.188");
	socklen_t m_server = sizeof(serv);
	struct PointCenter pointCenter;
#endif


	while (1){

#ifdef __linux__
		square_len = 15;
#else
		square_len = 15;
#endif
		Shape sh;
		HandGesture hg1;
		m.cap >> m.src; // chuyen anh tu cam qua anh nguon

		//lay anh nen background
		Mat framePrev;
		framePrev = returnImagePrev(m.cap);

		//set man hinh cho
		namedWindow("Wait", 1);
		setMouseCallback("Wait", CallBackFunc, &m);
		waitStartProgress(m);
		flagOn = 0;
		//chờ nhận mẫu màu từ lòng bàn tay
		m.cap >> m.src;
		waitForPalmCover(&m);

		//tinh toan HLS
		//tính toán
		average(&m);


		//tat cua so
		destroyWindow("WaitPalm");
		destroyAllWindows();

		//cac trang thai co
		bool flag1 = false;
		bool flag2 = false;
		bool flag3 = false;
		bool flag4 = false;
		bool flag5 = false;
		bool flag6 = false;
		//Hien thi trackbar
		namedWindow("trackbar", CV_WINDOW_AUTOSIZE);
		namedWindow("HandDetect", CV_WINDOW_KEEPRATIO);
		inittrackbar();

		//ve background
		Mat cameraFeed;
		SkinDetector mySkinDetector;
		Mat skinMat;
		Mat show;
		namedWindow("trackbarYCrCb", CV_WINDOW_AUTOSIZE);
		mySkinDetector.initTrackBarYCrCb();

		//dieu khien chuot
		int xcu, ycu, xx, yy;
		xcu = ycu = xx = yy = 0;
		int flagM = 1;
		int flagC = 1;
		int flagLC = 1;

		int xcc, ycc,xc,yc;
		xcc = ycc = xc=yc=0;

		////////////////////////////////////////////////

		Rect trackWindow;
		int hsize = 16;
		float hranges[] = { 0, 180 };
		const float* phranges = hranges;
		Mat hue, hist;
		bool flagCam = true;
		mySelection &= Rect(0, 0, 320, 240);

		////////////////////////////////////////////////
		while (1){
			//0 ==> show histogram chua can bang
			//1 ==> loai bo background
			//2 ==> can bang histogram
			//3 ==> show histogram can bang
			char c = (char)waitKey(10);
			if (c == 27) // reset lai chuong trinh
				break;
			switch (c)
			{
			case '1': // loai bo background
				flag1 = !flag1; break;
			case '2': // lay lai background
				flag2 = !flag2; break;
			case '3': // can bang histogram
				flag3 = !flag3; break;
			case '4': // histogram
				flag4 = !flag4; break;
			case '5': // doi sang ko gian mau YCrCb
				flag5 = !flag5; break;
			case '6': // dieu khien chuot
				flag6 = !flag6; break;
			case 'q': // thoat khoi chuong trinh
#ifdef __linux__
				cout << "program finished clear GPIO" << endl;
				digitalWrite(LED1, 0);
				digitalWrite(LED2, 0);
				digitalWrite(LED3, 0);
				digitalWrite(LED4, 0);
				digitalWrite(LED5, 0);
				digitalWrite(LED_NO, 0);

				pinMode(LED1, INPUT);
				pinMode(LED2, INPUT);
				pinMode(LED3, INPUT);
				pinMode(LED4, INPUT);
				pinMode(LED5, INPUT);
				pinMode(LED_NO, INPUT);
				close(sockfd);
#endif
				return 0;
			}

			sh.frNumber++;
			m.cap >> m.src;
#ifdef __linux__
			flip(m.src, m.src, 0); 
#else
			flip(m.src, m.src, 1);
#endif
			m.src.copyTo(m.srcLR);

			if (flag1){
				m.srcLR = returnImage(m.src, framePrev);
			}
			if (flag2){
				framePrev = returnImagePrev(m.cap);
				flag2 = !flag2;
			}

			if (flag3){
				//m.srcLR = returnImAdjust(m.srcLR);
				m.srcLR = canBangHistogram(m.srcLR);
				imshow("CanBangHistogram", m.srcLR);
			}
			else{
				cvDestroyWindow("CanBangHistogram");
			}

			if (flag4){
				drawHist("Histogram", m.srcLR);
			}
			else{
				cvDestroyWindow("Histogram");
			}

			//if (flag5){
			//	/////////////////////////////////////////////
			//	//imshow("Original Image", cameraFeed);
			//	m.bw = mySkinDetector.getSkin(m.srcLR);
			//	imshow("binary ycrcb", m.bw);
			//	/////////////////////////////////////////////
			//}
			//else{
			//	cvDestroyWindow("binary ycrcb");
				pyrDown(m.srcLR, m.srcLR);
				blur(m.srcLR, m.srcLR, Size(3, 3));
				//chuyển đổi màu của ảnh đó sang HLS
				cvtColor(m.srcLR, m.srcLR, ORIGCOL2COL);
				//tạo ảnh nhị phân hiển thị màu cùng với màu da
				produceBinaries(&m);
				pyrUp(m.bw, m.bw);
				pyrUp(m.srcLR, m.srcLR);
				camShiftDemo(m.src, m.srcLR, m.bw, hue, hist, phranges, hsize, trackWindow, flagCam);
				//chuyển lại ảnh 1 nửa thành màu RGB lại
				cvtColor(m.srcLR, m.srcLR, COL2ORIGCOL);
				//imshow("Mask", m.bw);
			//}
			makeContours(m.src, m.bw, &sh, &hg1);
			///////////////////////////////////////////////////////////
			//int startidx, endidx, faridx;//diem dau diem cuoi diem giua
			//vector<Vec4i>::iterator d = hg1.defects.begin();
			//while (d != hg1.defects.end()) {
			//	Vec4i& v = (*d);
			//	startidx = v[0];	Point ptStart(hg1.contours[startidx]);
			//	endidx = v[1];		Point ptEnd(hg1.contours[endidx]);
			//	faridx = v[2];		Point ptFar(hg1.contours[faridx]);
			//	circle(m.src, ptFar, 3, Scalar(0, 200, 0), 5);
			//	d++;
			//}

			if (flag6){
#ifdef __linux__
				pointCenter.x = hg1.getPointCenter().x;
				pointCenter.y = hg1.getPointCenter().y;
				pointCenter.fingerNum = hg1.mostFrequentFingerNumber;
				sendto(sockfd, (char*)&pointCenter, sizeof(pointCenter), 0, (struct sockaddr *)&serv, m_server);
				memset(&pointCenter, 0, sizeof(pointCenter));
#else
				xx = hg1.getPointCenter().x;
				yy = hg1.getPointCenter().y;
				controlCursor(flagC, flagLC, xx, yy, xcu, ycu, hg1);
				xcu = xx;
				ycu = yy;

				//if (hg1.mostFrequentFingerNumber != 0 ){
				//	xc = hg1.getPointHighest().x;
				//	yc = hg1.getPointHighest().y;
				//	drawRectCursor(m.src, m.bw, xc, yc, xcc, ycc);
				//	xcc = xc;
				//	ycc = yc;
				//}
#endif
			}
			//handDect(m.src);
			//getOrientation(hg1.contours, m.src);
			showWindows(m.src, m.bw);
		}

		for (int i = 0; i < NSAMPLES; i++){
			c_lower[i][0] = 20;
			c_upper[i][0] = 100;
			c_lower[i][1] = 30;
			c_upper[i][1] = 255;
			c_lower[i][2] = 15;
			c_upper[i][2] = 255;
			avgColor[i][0] = 0;
			avgColor[i][1] = 0;
			avgColor[i][2] = 0;
		}

#ifdef __linux__
		cout << "program finished clear GPIO" << endl;
		digitalWrite(LED1, 0);
		digitalWrite(LED2, 0);
		digitalWrite(LED3, 0);
		digitalWrite(LED4, 0);
		digitalWrite(LED5, 0);
		digitalWrite(LED_NO, 0);

		pinMode(LED1, INPUT);
		pinMode(LED2, INPUT);
		pinMode(LED3, INPUT);
		pinMode(LED4, INPUT);
		pinMode(LED5, INPUT);
		pinMode(LED_NO, INPUT);
#endif
		roi.clear();
		cout << "reset" << endl;
		destroyAllWindows();
	}
	m.cap.release();
	return 0;
}