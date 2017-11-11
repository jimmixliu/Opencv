#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
#if 0
	Mat image = imread("1.jpg");

	namedWindow("[原图]膨胀操作");
	namedWindow("[效果图]膨胀操作");

	imshow("[原图]膨胀操作",image);

	Mat element = getStructuringElement(MORPH_RECT,Size(15,15));
	Mat out;

	dilate( image, out,element);

	imshow("[效果图]膨胀操作",out);

	waitKey(0);
#endif

	Mat image = imread("1.jpg");

	namedWindow("[原图]腐蚀操作");
	namedWindow("[效果图]腐蚀操作");

	imshow("[原图]腐蚀操作",image);

	Mat element = getStructuringElement(MORPH_RECT,Size(15,15));
	Mat out;

	erode( image, out,element);

	imshow("[效果图]腐蚀操作",out);

	waitKey(0);



}
