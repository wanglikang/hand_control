#include <opencv2/opencv.hpp>

#include "mUtils.h"
#include <ctime>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
using namespace cv;
#define debug false


void track_change(int ,void*){}
int main(){


	VideoCapture vc = VideoCapture(0);
	mutils myutils;
	myutils.createModel(vc);//手掌的样本在开始的时候进行获取，，让用户把手掌放在窗口的指定位置;

	Mat srcImg;
	Mat gray;

	
	CascadeClassifier classifier ;
	classifier.load("C:\\Users\\WLK\\Pictures\\data\\cascade.xml");
	vector<Rect> objects;
	

	

	namedWindow("button");//////////////////////////创建一个trackbar窗口；
	int minSize=30,
		maxSize = 120,
		minnerghor =22;	
	createTrackbar("mixSize:","button",&minSize,100,track_change);
	createTrackbar("maxSize:","button",&maxSize,200,track_change);
	createTrackbar("minnerghor:","button",&minnerghor,100,track_change);
	track_change(minSize,0);
	track_change(maxSize,0);
	track_change(minnerghor,0);
	static	Point* resultPoints = new Point[5];

	while(true){
		vc>>srcImg;
		myutils.loadsrcImg(srcImg);

		objects.clear();
		int scaledWidth = 300;
		Mat gray;
		if (srcImg.channels() == 3) {
			cvtColor(srcImg, gray, CV_BGR2GRAY);
		}
		else if (srcImg.channels() == 4) {
			cvtColor(srcImg, gray, CV_BGRA2GRAY);
		}
		else {
			// Access the input image directly, since it is already grayscale.
			gray = srcImg;
		}
		// Possibly shrink the image, to run much faster.
		Mat toshowImage;
		float scale = srcImg.cols / (float)scaledWidth;
		if (srcImg.cols > scaledWidth) {
			// Shrink the image while keeping the same aspect ratio.
			int scaledHeight = cvRound(srcImg.rows / scale);
			resize(gray, toshowImage, Size(scaledWidth, scaledHeight));
		}
		else {
			// Access the input image directly, since it is already small.
			toshowImage = gray;
		}
		// Standardize the brightness and contrast to improve dark images.
		Mat equalizedImg;
		equalizeHist(toshowImage, equalizedImg);
		classifier.detectMultiScale(equalizedImg,objects,1.1f,minnerghor,0,Size(minSize,minSize),Size(maxSize,maxSize));
		printf("\nminSize is:%d\nmaxSize id:%d\n\n",minSize,maxSize);
		
		vector<Rect>::iterator begin = objects.begin();
		vector<Rect>::iterator end = objects.end();
			while(begin != end){
		
				rectangle(toshowImage,*begin,Scalar(0,225,255),5);
				printf("draw a rect\n");
				begin++;
			}

			printf("\n");
////////////////////////k-mean///////////////////////
			
			myutils.run_k_means(resultPoints);///////////KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKMMMMMMEEEEEEAAAAAAAAANNNNNNNNNSSS

			myutils.drawKMeansResult(toshowImage,resultPoints);
			
			imshow("finalResult",toshowImage);
		
	
////////////////k-mean//////////////////////////
		Mat finalresult;
		resize(srcImg,finalresult,Size(toshowImage.cols,toshowImage.rows));
///////////k-mean and harracascade//////////////
		for(int x = 0;x<objects.size();x++){
			for(int q = 0;q<4;q++){
				Point center;
				center.x = objects[x].x+objects[x].width/2;
				center.y = objects[x].y-objects[x].height/2;
				if(sqrt(myutils.diss(center,resultPoints[q])) < 20){
					circle(finalresult,resultPoints[q],30,Scalar(255,100,200),3);
				}

			}

		}
		imshow("show hand position:",finalresult);
	waitKey(10);
	}
	return 0;
}
