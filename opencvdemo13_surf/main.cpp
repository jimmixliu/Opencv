#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/features2d/features2d.hpp"  
#include "opencv2/nonfree/features2d.hpp"  
#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/legacy/legacy.hpp"
#include <iostream>  
#include <stdlib.h>
#include <stdio.h>

  
using namespace cv;  
using namespace std;  

int main(int argc, char **argv)  
{

	Mat srcImage1 = imread("1.jpg",1);
	Mat srcImage2 = imread("2.jpg",1);
	if(!srcImage1.data || !srcImage2.data){
		printf("load 1.jpg failed\n");
		return false;
	}

	imshow("原始图1",srcImage1);
	imshow("原始图2",srcImage2);

	int minHessian = 4000;
	SurfFeatureDetector detector(minHessian);
	vector<KeyPoint> keypoints_1,keypoints_2;

	detector.detect(srcImage1,keypoints_1);
	detector.detect(srcImage2,keypoints_2);

	Mat img_keypoints_1,img_keypoints_2;
#if 0
	drawKeypoints(srcImage1,keypoints_1,img_keypoints_1,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
	drawKeypoints(srcImage2,keypoints_2,img_keypoints_2,Scalar::all(-1),DrawMatchesFlags::DEFAULT);

	imshow("效果图1",img_keypoints_1);
	imshow("效果图2",img_keypoints_2);
#else
	SurfDescriptorExtractor extractor;
	extractor.compute(srcImage1,keypoints_1,img_keypoints_1);
	extractor.compute(srcImage2,keypoints_2,img_keypoints_2);

	BruteForceMatcher< L2<float> > matcher;
	vector<DMatch> matches;
	matcher.match (img_keypoints_1,img_keypoints_2,matches);

	Mat img_matches;	
	drawMatches(srcImage1,keypoints_1,srcImage2,keypoints_2,matches,img_matches);

	imshow("Matches",img_matches);


#endif





	waitKey(0);

	return 0;
}


