#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;


Mat src,hsv,hue;
int bins = 25;

static void Hist_and_Backproj(int, void*);

int main(int argc, char ** argv)
{
	src = imread(argv[1],1);
	if(!src.data){
		printf("imread load failed\n");
		return -1;
	}

	cvtColor(src,hsv,CV_BGR2HSV);

	hue.create(hsv.size(),hsv.depth());
	int ch[] = {0,0};
	mixChannels(&hsv,1,&hue,1,ch,1);

	char* window_image = "Source image";
	namedWindow(window_image,CV_WINDOW_AUTOSIZE);
	createTrackbar("* Hue bins:",window_image,&bins,180, Hist_and_Backproj);
	Hist_and_Backproj(0,0);

	imshow(window_image,src);

   	waitKey(0);

    return 0;
}

static void Hist_and_Backproj(int, void*)
{
	Mat hist;
	int histSize = MAX(bins,2);
	float hue_range[] = {0,180};
	const float * ranges = {hue_range};

	calcHist(&hue,1,0,Mat(),hist,1,&histSize,&ranges,true,false);
	normalize(hist,hist,0,255,NORM_MINMAX,-1,Mat());

	Mat backproj;
	calcBackProject(&hue,1,0,hist,backproj,&ranges,1,true);

	imshow("Backproj",backproj);

	int w = 400, h = 400;
	int bin_w = cvRound((double)w / histSize);
	Mat histImg = Mat::zeros(w,h,CV_8UC3);

	for(int i = 0; i < bins; i ++){
		rectangle(histImg,Point(i*bin_w,h), Point((i+1)*bin_w,h-cvRound(hist.at<float>(i)*h/255.0)),Scalar(0,0,255),-1);
	}

	imshow("Histogram",histImg);
}











