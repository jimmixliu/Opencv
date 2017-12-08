#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;


int main(int argc, char ** argv)
{

	Mat src,dst;

	src = imread(argv[1],1);
	if(!src.data){
		printf("imread error\n");
		return -1;
	}

	cvtColor(src,src,CV_BGR2GRAY);

	equalizeHist(src,dst);

	imshow("source window",src);
	imshow("equalizeHist window",dst);

   	waitKey(0);

    return 0;
}
