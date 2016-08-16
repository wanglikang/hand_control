#include "mUtils.h"
#include <ctime>
using namespace cv;
using namespace std;
mutils::mutils(void)
{
	scaledWidth = 500;
	mpoints = new Point[1000000];
	
}

mutils::~mutils(void)
{

}


void mutils::k_means(Point *points,int points_num,Point* resultPoint,int k){
	//初始化多个聚类中心;
	//4个聚类中心
	static Point
		*centerPoints;
	static Point *old_centers;
	centerPoints = (Point*)malloc(sizeof(Point)*(k+1));
	old_centers = (Point*)malloc(sizeof(Point)*(k+1));
	//初始化聚类中心
	for(int q = 1;q<=k;q++){
		centerPoints[q] = points[points_num/k*q];
	}



	int dis[5];
	do{

		int center_count[5];
		center_count[0] = 1;
		center_count[1] = 1;
		center_count[2] = 1;
		center_count[3] = 1;
		center_count[4] = 1;
		int sum[5][2];
		for(int z = 0;z<5;z++)
			for(int a = 0;a<2;a++)
				sum[z][a] = 0;

		//遍历数据集中的每一个点，，
		for(int q = 0;q<points_num;q++){

			//计算点q距离每个聚类中心的距离
			for(int x = 1;x<=k;x++){
				dis[x] = (points[q].x - centerPoints[x].x)*(points[q].x - centerPoints[x].x)
					+ (points[q].y - centerPoints[x].y)*(points[q].y - centerPoints[x].y);
			}
			/////////求出点q输入哪一个中心
			int min = dis[1];
			int min_num = 1;
			for(int mi = 1;mi<=k;mi++){
				if(min>dis[mi]){
					min_num = mi;
					min = dis[mi];
				}
			}
			//为下一步移动聚类中心做准备
			//统计属于此聚类中心的x,y坐标，，，
			center_count[min_num]++;
			sum[min_num][0] += points[q].x;
			sum[min_num][1] += points[q].y;			

		}

		for(int x = 1;x<=k;x++){
			//将老中心设置为当前中心，
			old_centers[x] = centerPoints[x];

			//移动聚类中心至新的中心
			centerPoints[x].x = sum[x][0]/center_count[x];
			centerPoints[x].y = sum[x][1]/center_count[x];
			//判断老聚类中心距离新中心的距离，，，为是否进行下次运算做准备
			dis[x] = diss(old_centers[x],centerPoints[x]);
		}
	}while(dis[1] >10 || dis[2]>10 || 
		dis[3] > 10 || dis[4] > 10);

	//返回统计到的聚类中心至resultPoints数组中
	for(int x = 1;x<=k;x++)
		resultPoint[x] = centerPoints[x];
}

void mutils::countPoint(Mat mat,int &n,int thresh){
	n = 0;
	int rows = mat.rows;
	int cols = mat.cols;
	for(int q = 0;q<rows;q++){
		uchar* ptr = mat.ptr<uchar>(q);
		for(int p = 0;p<cols;p++){
			if(ptr[p]>thresh){
				mpoints[n] = Point(p,q);
				n++;
			}
		}
	}
}

int mutils::diss(Point p1,Point p2){
	return (p1.x-p2.x)^2+(p1.y-p2.y)^2;
}

void mutils::run_k_means(Point *resultPoints){


	cvtColor(model,hsv_pic,COLOR_RGB2HSV);
	hue_image.create(model.size(),model.depth());
	int ch[] = {0,0};
	mixChannels(&hsv_pic,1,&hue_image,1,ch,1);
	int histSize = 5;
	float hue_range[] = {0,180};
	const float * ranges = {hue_range};
	calcHist(&hue_image,1,0,Mat(),hist,1,&histSize,&ranges,true,false);
	normalize(hist,hist,0,255,NORM_MINMAX,-1,Mat());
	resize(srcImg,cur,Size(scaledWidth,scaledWidth));
	cvtColor(cur,cur_hsv,COLOR_RGB2HSV);
	cur_hue.create(cur_hsv.size(),cur_hsv.depth());
	mixChannels(&cur_hsv,1,&cur_hue,1,ch,1);
	calcBackProject(&cur_hue,1,0,hist,backproj,&ranges,1,true);
	threResult = backproj>250;
	erode(threResult,threResult,getStructuringElement(MORPH_RECT,Size(3,3)));
	int n  = 0;
	countPoint(threResult,n,250);
	printf("has %d points",n);
	k_means(mpoints,n,resultPoints,4);


}

void mutils::loadsrcImg(Mat srcImg){
	this->srcImg = srcImg;
}

void mutils::createModel(VideoCapture vc){
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
	model = temp(captureRect);
	//resize(model,model,Size(480,320));
	imshow("model:",model);
	waitKey(1000);
	destroyAllWindows();	
}

void mutils::drawKMeansResult(Mat toshowImage,Point *resultPoints){
	circle(toshowImage,resultPoints[0],20,Scalar(255,255,255),5);
	circle(toshowImage,resultPoints[1],20,Scalar(255,255,255),5);
	circle(toshowImage,resultPoints[2],20,Scalar(255,255,255),5);
	circle(toshowImage,resultPoints[3],20,Scalar(255,255,255),5);
	line(toshowImage,resultPoints[1],resultPoints[2],Scalar(0,0,255));
	line(toshowImage,resultPoints[2],resultPoints[3],Scalar(0,0,255));
	line(toshowImage,resultPoints[3],resultPoints[4],Scalar(0,0,255));
	line(toshowImage,resultPoints[4],resultPoints[1],Scalar(0,0,255));
}