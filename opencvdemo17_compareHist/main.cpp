#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

#if 0
static Mat my_histImage(Mat src)
{
	//1,split R G B channels
	vector<Mat> rgb_planes;
	split(src,rgb_planes);


	//2,set the number of bin;
	int histSize = 256;

	//3,range
	float range[]= {0,255};
	const float *histRange = {range};

	//4, calcHist
	bool uniform = true;
	bool accumulate = false;

	Mat r_hist,g_hist,b_hist;

	calcHist(&rgb_planes[0],1,0,Mat(),b_hist,1,&histSize,&histRange,uniform,accumulate);
	calcHist(&rgb_planes[1],1,0,Mat(),g_hist,1,&histSize,&histRange,uniform,accumulate);
	calcHist(&rgb_planes[2],1,0,Mat(),r_hist,1,&histSize,&histRange,uniform,accumulate);

	//5 hist graph 	
	int hist_w = 400;
	int hist_h = 400;
	int bin_w = cvRound((double)(hist_w)/histSize);

	Mat histImage(hist_w,hist_h,CV_8UC3,Scalar(0,0,0));

	normalize(r_hist,r_hist,0,histImage.rows,NORM_MINMAX,-1,Mat());
	normalize(g_hist,g_hist,0,histImage.rows,NORM_MINMAX,-1,Mat());
	normalize(b_hist,b_hist,0,histImage.rows,NORM_MINMAX,-1,Mat());

	for(int i = 1; i < histSize; i++){
		line(histImage,Point(bin_w*(i - 1),hist_h - cvRound(r_hist.at<float>(i - 1))), \
			       Point(bin_w*i ,hist_h - cvRound(r_hist.at<float>(i))),		\
			       Scalar(0,0,255),2,8,0);	
		line(histImage,Point(bin_w*(i - 1),hist_h - cvRound(g_hist.at<float>(i - 1))),\
			       Point(bin_w*i ,hist_h - cvRound(g_hist.at<float>(i))), \
			       Scalar(0,255,0),2,8,0);	
		line(histImage,Point(bin_w*(i - 1),hist_h - cvRound(b_hist.at<float>(i - 1))),\
			       Point(bin_w*i ,hist_h - cvRound(b_hist.at<float>(i))),\
			       Scalar(255,0,0),2,8,0);	
	}

	return histImage;
}

static Mat my_grayhistImage(Mat src)
{

	//1,set the number of bin;
	int histSize = 256;

	//2,range
	float range[]= {0,255};
	const float *histRange = {range};

	//3, calcHist
	bool uniform = true;
	bool accumulate = false;

	Mat hist;

	calcHist(&src,1,0,Mat(),hist,1,&histSize,&histRange,uniform,accumulate);

	//5 hist graph 	
	int hist_w = 400;
	int hist_h = 400;
	int bin_w = cvRound((double)(hist_w)/histSize);

	Mat histImage(hist_w,hist_h,CV_8UC3,Scalar(0,0,0));

	normalize(hist,hist,0,histImage.rows,NORM_MINMAX,-1,Mat());

	for(int i = 1; i < histSize; i++){
		line(histImage,Point(bin_w*(i - 1),hist_h - cvRound(hist.at<float>(i - 1))), \
			       Point(bin_w*i ,hist_h - cvRound(hist.at<float>(i))),		\
			       Scalar(255,255,255),2,8,0);	
	}

	return histImage;
}
#endif





int main(int argc, char ** argv)
{

#if 0
	Mat src,dst,src_histImage,dst_histImage;

	src = imread(argv[1],1);
	if(!src.data){
		printf("imread error\n");
		return -1;
	}


	cvtColor(src,src,CV_BGR2GRAY);
	imshow("src",src);

	src_histImage = my_grayhistImage(src);
	imshow("src Hist",src_histImage);

#if 1

	equalizeHist(src,dst);

	imshow("equalizeHist window",dst);
	dst_histImage = my_grayhistImage(dst);

	imshow("dst Hist",dst_histImage);

#endif
#endif
	Mat src_base,  hsv_base;
	Mat src_test1, hsv_test1;
	Mat src_test2, hsv_test2;
	Mat hsv_half_down;

	if(argc < 4){
		printf("./test 1.jpg 2.jpg 3.jpg\n");
	}

	src_base = imread(argv[1],1);
	src_test1 = imread(argv[2],1);
	src_test2 = imread(argv[3],1);

	if(!(src_base.data && src_test1.data && src_test2.data)){
		printf("load error\n");
	}

	cvtColor(src_base,hsv_base,CV_BGR2HSV);
	cvtColor(src_test1,hsv_test1,CV_BGR2HSV);
	cvtColor(src_test2,hsv_test2,CV_BGR2HSV);

	hsv_half_down = hsv_base(Range(hsv_base.rows/2, hsv_base.rows-1),Range(0,hsv_base.cols-1));

	int h_bins = 30, s_bins = 32;
	int histSize[] = {h_bins,s_bins};

	float h_ranges[] = {0,256};
	float s_ranges[] = {0,180};

	const float *rangs[] = {h_ranges,s_ranges};

	int channels[] = {0,1};

	Mat hist_base;
	Mat hist_half_down;
	Mat hist_test1;
	Mat hist_test2;

	calcHist(&hsv_base,1,channels, Mat(), hist_base,2,histSize,rangs,true,false);	
	normalize(hist_base,hist_base,0,1,NORM_MINMAX,-1,Mat());

	calcHist(&hsv_half_down,1,channels, Mat(), hist_half_down,2,histSize,rangs,true,false);	
	normalize(hist_half_down,hist_half_down,0,1,NORM_MINMAX,-1,Mat());

	calcHist(&hsv_test1,1,channels, Mat(), hist_test1,2,histSize,rangs,true,false);	
	normalize(hist_test1,hist_test1,0,1,NORM_MINMAX,-1,Mat());

	calcHist(&hsv_test2,1,channels, Mat(), hist_test2,2,histSize,rangs,true,false);	
	normalize(hist_test2,hist_test2,0,1,NORM_MINMAX,-1,Mat());

	for(int i = 0; i < 4; i++){
		int compare_method = i;
		double base_base = compareHist(hist_base,hist_base,compare_method);
		double base_half = compareHist(hist_base,hist_half_down,compare_method);
		double base_test1 = compareHist(hist_base,hist_test1,compare_method);
		double base_test2 = compareHist(hist_base,hist_test2,compare_method);

		printf("Method = %d, base-base = %f, base_half = %f, base_test1 = %f, base_test2 = %f\n",compare_method,base_base,base_half,base_test1,base_test2);
	}

	printf("Done\n");


   	waitKey(0);

    return 0;
}
