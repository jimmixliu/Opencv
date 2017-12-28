#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;


Mat g_src,g_src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

static void thresh_callback(int,void *);

int main(int argc, char ** argv)
{
	g_src = imread(argv[1],1);
	if(!g_src.data){
		printf("imread load failed\n");
		return -1;
	}

	cvtColor(g_src,g_src_gray,CV_BGR2GRAY);
	blur(g_src_gray,g_src_gray,Size(3,3));

	char *source_window = "Source";
	namedWindow(source_window,CV_WINDOW_AUTOSIZE);
	imshow(source_window,g_src);

	createTrackbar("Canny thresh",source_window,&thresh,max_thresh,thresh_callback);
	thresh_callback(0,0);

   	waitKey(0);

    return 0;
}

/*
    CV_RETR_EXTERNAL=0,
    CV_RETR_LIST=1,
    CV_RETR_CCOMP=2,
    CV_RETR_TREE=3,
    CV_RETR_FLOODFILL=4
*/
#define CV_CUSTOM_MODE CV_RETR_CCOMP

/*
    CV_CHAIN_CODE=0,
    CV_CHAIN_APPROX_NONE=1,
    CV_CHAIN_APPROX_SIMPLE=2,
    CV_CHAIN_APPROX_TC89_L1=3,
    CV_CHAIN_APPROX_TC89_KCOS=4,
    CV_LINK_RUNS=5
*/

#define CV_CUSTOM_METHOD CV_CHAIN_APPROX_NONE

static void thresh_callback(int,void *)
{

	Mat canny_output;
	vector<vector<Point> >  contours;
	vector<Vec4i> hierarchy;

	Canny(g_src_gray,canny_output,thresh,thresh*2,3);

	findContours(canny_output,contours,hierarchy,CV_CUSTOM_MODE,CV_CUSTOM_METHOD,Point(0,0));

	Mat drawing = Mat::zeros(canny_output.size(),CV_8UC3);
#if 0
	for(int i = 0; i < contours.size(); i++){
		Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255));
		drawContours(drawing,contours,i,color,2,8,hierarchy,0,Point());
	}
#else
	Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255));
	drawContours(drawing,contours,-1,color);

#endif
	imshow("Contours",drawing);

}
