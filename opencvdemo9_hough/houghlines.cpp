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
	//cvtColor(srcImage,midImage,CV_GRAY2BGR);
	imshow("[转化图]",midImage);
	Canny(midImage,mid2Image,50,200,3);
	cvtColor(mid2Image,dstImage,CV_GRAY2BGR);
	imshow("[边缘检测图dst]",dstImage);

#if 0
	//3 hough
	vector<Vec2f> lines; HoughLines(mid2Image, lines,1,CV_PI/180,150,0,0); 
	//4 draw line
	for(size_t i = 0; i < lines.size(); i++){
		float rho = lines[i][0],theta = lines[i][1];
		Point pt1,pt2;
		double a = cos(theta),b = sin(theta);
		double x0 = a*rho, y0 = b*rho;

		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(-a));

		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(-a));
		line(dstImage,pt1,pt2,Scalar(0,255,64),1,CV_AA);
	}
#else
	//3 hough P
	vector<Vec4i> lines;
	HoughLinesP(mid2Image,lines,1,CV_PI/180,80,50,10);

	//4 draw line
	for(size_t i = 0; i < lines.size(); i++){
		Vec4i l = lines[i];
		line(dstImage,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,255,64),1,CV_AA);
	}
#endif




	imshow("[效果图]",dstImage);
	waitKey(0);

	return 0;
}
