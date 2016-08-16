#include "mUtils.h"


mutils::mutils(void)
{

	
}


mutils::~mutils(void)
{
}

void mutils::run_k_means(Point *points,int points_num,Point* resultPoint,int k){
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

void mutils::countPoint(Mat mat,Point *points,int &n,int thresh){
	n = 0;
	int rows = mat.rows;
	int cols = mat.cols;
	for(int q = 0;q<rows;q++){
		uchar* ptr = mat.ptr<uchar>(q);
		for(int p = 0;p<cols;p++){
			if(ptr[p]>thresh){
				points[n] = Point(p,q);
				n++;
			}
		}
	}
}

int mutils::diss(Point p1,Point p2){
	return (p1.x-p2.x)^2+(p1.y-p2.y)^2;
}

void mutils::loadMat(Mat srcImg){
	Mat equalizedImg;

	cvtColor(model,hsv_pic,COLOR_RGB2HSV);

	hue_image.create(model.size(),model.depth());
	float scaledWidth = 500;

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

	int ch[] = {0,0};

	static	Mat cur;
	static	Mat hsv_pic;
	static	Mat dst;
	static	Mat hue_image;

	static Mat cur;
	static Mat toshowImage;
	resize(srcImg,cur,Size(toshowImage.cols,toshowImage.rows));
	static	Mat cur_hue;
	static	Mat cur_hsv;
	cvtColor(cur,cur_hsv,COLOR_RGB2HSV);
	cur_hue.create(cur_hsv.size(),cur_hsv.depth());
	mixChannels(&cur_hsv,1,&cur_hue,1,ch,1);

	static	MatND backproj;
	calcBackProject(&cur_hue,1,0,hist,backproj,&ranges,1,true);
	if(debug)
		imshow("backproj",backproj);

	static		Mat threResult;
	threResult = backproj>250;
	if(debug)
		imshow("htres",threResult);


	erode(threResult,threResult,getStructuringElement(MORPH_RECT,Size(3,3)));


	static	Point *points = new Point[1000000];
	static	Point* resultPoints = new Point[5];
	int n  = 0;
	try{
		countPoint(threResult,points,n,250);
	}
	catch(exception e ){
		printf("%s",e.what());
	}
	printf("has %d points",n);

}