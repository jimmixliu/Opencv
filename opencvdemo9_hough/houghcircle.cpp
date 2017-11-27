#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


using namespace cv;
using namespace std;


int main()
{
	Mat midImage,mid2Image,dstImage;

	//1, read
	Mat srcImage = imread("1.jpg");	
	if(!srcImage.data){
        	cout<<endl<<"load 1.jpg failed\n";
                return false;
        } 

	imshow("[原始图]",srcImage);

	//2, canny and convert gray
	cvtColor(srcImage,midImage,CV_BGR2GRAY);
	imshow("[转化图]",midImage);
	GaussianBlur(midImage,mid2Image,Size(9,9),2,2);

	imshow("[高斯模糊处理]",mid2Image);

	//3 hough circles 
	vector<Vec3f> circles;
	//HoughCircles(mid2Image,circles,CV_HOUGH_GRADIENT,1.5,10,200,100,0,0);
	HoughCircles(mid2Image,circles,CV_HOUGH_GRADIENT,1,srcImage.rows/8,200,100,0,0);

	//4 draw line
	for(size_t i = 0; i < circles.size(); i++){
		Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		circle(srcImage, center,3,Scalar(0,255,0),-1,8,0);

		circle(srcImage,center,radius,Scalar(155,50,255),3,8,0);
	}




	imshow("[效果图]",srcImage);
	waitKey(0);

	return 0;
}
