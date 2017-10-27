#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("1.jpg");
	namedWindow("boxfilter:ori");
	namedWindow("boxfilter:effect");

	imshow("boxfilter:ori",image);

	Mat out;
	boxFilter( image, out, -1, Size(5,5));

	imshow("boxfilter:effect",out);

	waitKey(0);
}
