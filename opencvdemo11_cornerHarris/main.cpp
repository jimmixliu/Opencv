#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <iostream>  
#include <stdlib.h>
#include <stdio.h>
  
using namespace cv;  
using namespace std;  

Mat g_srcImage,g_srcImage1,g_dstImage,g_grayImage;
int g_harrisThreshold = 100;

static void on_Cornerharris(int,void*);
  
int main(int argc, char **argv)  
{

	g_srcImage = imread("1.jpg");
	if(!g_srcImage.data){
		printf("load 1.jpg failed\n");
		return false;
	}

	imshow("原始图",g_srcImage);

	cvtColor(g_srcImage,g_grayImage,CV_BGR2GRAY);

#if 0
	Mat cornerStrength;
	cornerHarris(g_srcImage,cornerStrength,2,3,0.04);

	Mat harrisCorner;
	threshold(cornerStrength,harrisCorner,0.00001,255,THRESH_BINARY);

	imshow("效果图",harrisCorner);

	waitKey(0);
	return 0;
#endif
	namedWindow("[效果图]");
	createTrackbar("阈值:","[效果图]",&g_harrisThreshold,255,on_Cornerharris);

	on_Cornerharris(0,0);

	while((char(waitKey(1))!='q')){}

	return 0;
}

static void on_Cornerharris(int,void*)
{
	Mat dst_Image = Mat::zeros(g_srcImage.size(),CV_32FC1);
	Mat normal_Image;
	Mat scale_Image;

	g_srcImage1 = g_srcImage.clone();

	cornerHarris(g_grayImage,dst_Image,2,3,0.04,BORDER_DEFAULT);

	normalize(dst_Image,normal_Image,0,255,NORM_MINMAX,CV_32FC1,Mat());
	convertScaleAbs(normal_Image,scale_Image);

	for(int j = 0; j < normal_Image.rows;j++){
		for(int i = 0; i < normal_Image.cols;i++){
			if((int)normal_Image.at<float>(j,i) > g_harrisThreshold + 80){
				circle(g_srcImage1, Point(i,j),5,Scalar(10,10,255),2,8,0);
				circle(scale_Image, Point(i,j),5,Scalar(0,10,255),2,8,0);
			}
		}
	}

	imshow("[效果图]",g_srcImage1);
	imshow("[效果图1]",scale_Image);
}














