#pragma once
#include <opencv.hpp>
using namespace cv;
class mutils
{
public:
	mutils(void);
	~mutils(void);

public:
	void run_k_means(Point *points,int points_num,Point* resultPoint,int k);
	void countPoint(Mat mat,Point *points,int &n,int thresh);
	int diss(Point p1,Point p2);
	void loadMat(Mat);
};

