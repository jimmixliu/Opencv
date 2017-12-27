#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;


Mat img,temp1,result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;


static void MatchingMethod(int, void*);

int main(int argc, char ** argv)
{
	img = imread(argv[1],1);
	if(!img.data){
		printf("imread load failed\n");
		return -1;
	}

	temp1 = imread(argv[2],1);	
	if(!img.data){
		printf("imread temp1 load failed\n");
		return -1;
	}

	namedWindow(image_window,CV_WINDOW_AUTOSIZE);
	namedWindow(result_window,CV_WINDOW_AUTOSIZE);

	char* trackbar_label = "Method:\n 0:SQDIFF\n 1:SQDIFF_NORMED\n 2:CCORR\n 3:CCORR_NORMED\n 4:CCOEFF\n 5:CCOEFF_NORMED";
	createTrackbar(trackbar_label,image_window,&match_method,max_Trackbar,MatchingMethod);

	MatchingMethod(0,0);

   	waitKey(0);

    return 0;
}


static void MatchingMethod(int, void*)
{
	Mat img_display;
	img.copyTo(img_display);

	int result_cols = img.cols - temp1.cols + 1;
	int result_rows = img.rows - temp1.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);

	matchTemplate(img, temp1, result, match_method);
	//imshow("result",result);
	
	normalize(result, result,0,1,NORM_MINMAX, -1, Mat());

	double minVal,maxVal;
	Point minLoc,maxLoc,matchLoc;

	minMaxLoc(result,&minVal,&maxVal,&minLoc,&maxLoc,Mat());

	if(match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED){
		matchLoc = minLoc;
	}else{
		matchLoc = maxLoc;
	}

	rectangle(img_display,matchLoc,Point(matchLoc.x + temp1.cols, matchLoc.y + temp1.rows),Scalar::all(0),2,8,0);
	rectangle(result,matchLoc,Point(matchLoc.x + temp1.cols, matchLoc.y + temp1.rows),Scalar::all(0),2,8,0);

	imshow(image_window,img_display);
	imshow(result_window,result);
}













