#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;


float rect_iou(Rect &r1, Rect &r2);
float rect_iou1(Rect &r1, Rect &r2);


int main(int argc, char ** argv)
{
	Mat img = imread(argv[1]);
	Rect r1;
 	Rect r2;

	r1.x = 50;
	r1.y = 50;
	r1.width = 50;
	r1.height = 50;
    
	r2.x = 200;
	r2.y = 200;
	r2.width = 300;
	r2.height = 400;

	rectangle(img,r1,Scalar(0,0,255),3,8,0);
	rectangle(img,r2,Scalar(0,255,255),3,8,0);

	imshow("rectangle",img);

	cout<<rect_iou(r1, r2)<<endl;
	cout<<rect_iou1(r1, r2)<<endl;
	waitKey(0);



    return 0;
}


/*
*
*IOU = (Area(x) & Area(y))/(Area(x) | Area(y)) - (Area(x) & Area(y))
*
*
*/


float rect_iou1(Rect &r1, Rect &r2)
{
	Rect rect0 = r1 & r2;
	//Rect rect1 = r1 | r2;
	float area0 = rect0.width * rect0.height;

	float area1 = r1.width * r1.height + r2.width * r2.height;

	float ratio = area0 /(area1-area0);
	return ratio;
	
}

float rect_iou(Rect &r1, Rect &r2)
{
	float ratio;	

	int W = (min(r1.x+r1.width,r2.x+r2.width) - max(r1.x,r2.x));

	int H = (min(r1.y+r1.height,r2.y+r2.height) - max(r1.y,r2.y));

	if(W < 1 || H < 1){
		return 0;
	}	

	int Area = W * H;
	int AllArea = r1.width * r1.height + r2.width * r2.height;

	ratio = (float)Area/(float)(AllArea-Area);

	return ratio;
}
