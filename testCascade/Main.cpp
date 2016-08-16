#include <opencv2/opencv.hpp>

#include "mUtils.h"
#include <ctime>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
using namespace cv;
#define debug false

void countPoint(Mat mat,Point *points,int &n,int thresh);
int diss(Point p1,Point p2);

void track_change(int ,void*){}
int main(){

	mutils utils;

	VideoCapture vc = VideoCapture(0);
	Mat srcImg;
	
	Mat gray;

	
	
	CascadeClassifier classifier ;
	classifier.load("C:\\Users\\WLK\\Pictures\\data\\cascade.xml");
	vector<Rect> objects;
	
///////////////////////K-MEAN///init///////////////////////////
	//Mat  model = imread("d:\\newmodel.jpg");

//////////////////手掌的样本在开始的时候进行获取，，让用户把手掌放在窗口的指定位置;
	Mat temp;
	 time_t mtime;
	 time_t old_time;
	 time(&old_time);
	 int backcount = 10;
	 char str[4];
	 Rect captureRect(Point(190,110),Point(290,210));
	
	 
	 while(true){
		vc>>temp;
		resize(temp,temp,Size(480,320));
		rectangle(temp,captureRect,Scalar(100,200,45),3);
		putText(temp,"please put you hand below",Point(100,80),0,0.8,Scalar(0,0,100),1);
		sprintf(str,"%d",backcount);
		putText(temp,str,Point(50,40),0,1.2,Scalar(100,100,0),5);
		waitKey(10);
		time(&mtime);
		if(mtime>old_time){
			backcount--;
			old_time = mtime;
			if(backcount<0)
				break;
		}
		imshow("show you hand,please!",temp);

	}

	Mat  model = temp(captureRect);
	//resize(model,model,Size(480,320));
	imshow("model:",model);
	waitKey(1000);
	destroyAllWindows();
/////////////////////////////////////////////////////////////

	
	mixChannels(&hsv_pic,1,&hue_image,1,ch,1);
	static	MatND hist;
	int histSize = 5;
	float hue_range[] = {0,180};
	const float * ranges = {hue_range};
	calcHist(&hue_image,1,0,Mat(),hist,1,&histSize,&ranges,true,false);
	normalize(hist,hist,0,255,NORM_MINMAX,-1,Mat());
	//////////////////创建一个trackbar
	namedWindow("button");
	int minSize=30,
		maxSize = 120,
		minnerghor =22;
		
	createTrackbar("mixSize:","button",&minSize,100,track_change);
	createTrackbar("maxSize:","button",&maxSize,200,track_change);
	createTrackbar("minnerghor:","button",&minnerghor,100,track_change);
	track_change(minSize,0);
	track_change(maxSize,0);
	track_change(minnerghor,0);

	
///////////////////////K-mean//////////////////////////////
	
	
	while(true){
		vc>>srcImg;

			objects.clear();



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
			
			utils.run_k_means(points,n,resultPoints,4);///////////KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKMMMMMMEEEEEEAAAAAAAAANNNNNNNNNSSS

			static Mat finalResult;

			finalResult = Mat(cur.rows,cur.cols,CV_8UC3);

			circle(threResult,resultPoints[0],20,Scalar(255,255,255),5);
			circle(threResult,resultPoints[1],20,Scalar(255,255,255),5);
			circle(threResult,resultPoints[2],20,Scalar(255,255,255),5);
			circle(threResult,resultPoints[3],20,Scalar(255,255,255),5);
			imshow("erode",threResult);
			line(toshowImage,resultPoints[1],resultPoints[2],Scalar(0,0,255));
			line(toshowImage,resultPoints[2],resultPoints[3],Scalar(0,0,255));
			line(toshowImage,resultPoints[3],resultPoints[4],Scalar(0,0,255));
			line(toshowImage,resultPoints[4],resultPoints[1],Scalar(0,0,255));
			imshow("finalResult",toshowImage);
		
	
////////////////k-mean//////////////////////////
		Mat toshowImage2;
		resize(srcImg,toshowImage2,Size(toshowImage.cols,toshowImage.rows));






		
///////////k-mean and harracascade//////////////
		for(int x = 0;x<objects.size();x++){
			for(int q = 0;q<4;q++){
				Point center;
				center.x = objects[x].x+objects[x].width/2;
				center.y = objects[x].y-objects[x].height/2;
				if(sqrt(diss(center,resultPoints[q])) < 20){
					circle(toshowImage2,resultPoints[q],30,Scalar(255,100,200),3);

				}

			}

		}
		imshow("xxxresult:",toshowImage2);


////////////////////////////////////////////////

	waitKey(10);
	}
	
	return 0;
}



