#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("1.jpg");
	namedWindow("blur:ori");
	namedWindow("blur:effect");

	imshow("blur:ori",image);

	Mat out;
	blur( image, out,Size(7,7));

	imshow("blur:effect",out);

	waitKey(0);
}
