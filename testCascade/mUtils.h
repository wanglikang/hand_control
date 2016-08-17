#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
class mutils
{
public:
	mutils(void);
	~mutils(void);
public:
	int scaledWidth;
	Point *mpoints;
	Mat cur;
	Mat hsv_pic;
	Mat dst;
	Mat hue_image;
	Mat cur_hue;
	Mat cur_hsv;
	MatND backproj;
	Mat threResult;
	MatND hist;
	Mat model;
	Mat srcImg;

public:
	void k_means(Point *points,int points_num,Point* resultPoint,int k);
	void countPoint(Mat mat,int &n,int thresh);
	int diss(Point p1,Point p2);
	void loadMat(Mat);

	void createModel(VideoCapture vc);
	void loadsrcImg(Mat srcImage);
	void run_k_means(Point *resultPoints);
	void drawKMeansResult(Mat toshowImage,Point *resultPoints);
};

