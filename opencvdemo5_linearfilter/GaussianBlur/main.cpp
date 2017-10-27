#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("1.jpg");
	namedWindow("gaussian:ori");
	namedWindow("ganussian:effect");

	imshow("ganussian:ori",image);

	Mat out;
	GaussianBlur( image, out,Size(5,5),0,0);

	imshow("ganussian:effect",out);

	waitKey(0);
}
