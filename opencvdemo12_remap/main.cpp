#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <iostream>  
#include <stdlib.h>
#include <stdio.h>
  
using namespace cv;  
using namespace std;  

Mat g_srcImage,g_dstImage;
Mat map_x,map_y;
int ind = 0;

static void update_map(void);

int main(int argc, char **argv)  
{

	g_srcImage = imread("1.jpg",1);
	if(!g_srcImage.data){
		printf("load 1.jpg failed\n");
		return false;
	}

	imshow("原始图",g_srcImage);

	g_dstImage.create(g_srcImage.size(),g_srcImage.type()); 
	map_x.create(g_srcImage.size(),CV_32FC1); 
	map_y.create(g_srcImage.size(),CV_32FC1); 

	namedWindow("效果图",CV_WINDOW_AUTOSIZE);

	while(true){
		int c = waitKey(0);
		if(char(c) == 27){
			break;
		}

		update_map();
		remap(g_srcImage,g_dstImage,map_x,map_y,CV_INTER_LINEAR,BORDER_CONSTANT,Scalar(0,0,0));
		imshow("效果图",g_dstImage);
	}

	g_dstImage = Scalar::all(0);

	return 0;
}

static void update_map(void)
{

	ind = ind%4;

	switch(ind){
		case 0:
			for(int j = 0; j < g_srcImage.rows; j++){
				for(int i = 0; i < g_srcImage.cols; i++){
					if(j > g_srcImage.rows/4 && j < (g_srcImage.rows*3)/4 &&\
                                           i > g_srcImage.cols/4 && i < (g_srcImage.cols*3)/4){
						map_x.at<float>(j,i) = static_cast<float>(2*(i - g_srcImage.cols/4)) + 0.5;	
						map_y.at<float>(j,i) = static_cast<float>(2*(j - g_srcImage.rows/4)) + 0.5;	
					}else{
//						map_x.at<float>(j,i) = g_srcImage.rows-1;
//						map_y.at<float>(j,i) = g_srcImage.rows-1;
						map_x.at<float>(j,i) = 0;
						map_y.at<float>(j,i) = 0;
					}
				}
			}
		break;
		case 1:
			for(int j = 0; j < g_srcImage.rows; j++){
				for(int i = 0; i < g_srcImage.cols; i++){
					map_x.at<float>(j,i) = static_cast<float>(i);
					map_y.at<float>(j,i) = static_cast<float>(g_srcImage.rows-j);
				}
			}

		break;
		case 2:
			for(int j = 0; j < g_srcImage.rows; j++){
				for(int i = 0; i < g_srcImage.cols; i++){
					map_x.at<float>(j,i) = static_cast<float>(g_srcImage.cols-i);
					map_y.at<float>(j,i) = static_cast<float>(j);
				}
			}
		break;
		case 3:
			for(int j = 0; j < g_srcImage.rows; j++){
				for(int i = 0; i < g_srcImage.cols; i++){
					map_x.at<float>(j,i) = static_cast<float>(g_srcImage.cols-i);
					map_y.at<float>(j,i) = static_cast<float>(g_srcImage.rows-j);
				}
			}
		break;

	}

	ind++;
}













