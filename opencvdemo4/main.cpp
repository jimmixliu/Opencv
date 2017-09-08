#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

static void ContrastAndBright(int, void *);

int g_nContrastValue;
int g_nBrightValue;
Mat g_srcImage,g_dstImage;

int main()
{
	system("color 5E");

	g_srcImage = imread("pic1.jpg");
	if(!g_srcImage.data)
	{
		cout << "load pic1 error\n";
	}

	g_dstImage = Mat::zeros( g_srcImage.size(), g_srcImage.type());

	g_nContrastValue = 80;
	g_nBrightValue = 80;

	namedWindow("[effect picture]");

	createTrackbar("Contrast:","[effect picture]",&g_nContrastValue,300,ContrastAndBright);
	createTrackbar("Bright:","[effect picture]",&g_nBrightValue,200,ContrastAndBright);

	ContrastAndBright(g_nContrastValue,0);
	ContrastAndBright(g_nBrightValue,0);

	cout<<endl<<"\tok, please move the trackbar\n"
		  <<"\tpress q ,and quit\n";

	while(char(waitKey(1)) != 'q') {}


	return 0;
}


static void ContrastAndBright(int, void *)
{
	namedWindow("[ori picture]",1);

	for(int y = 0; y < g_srcImage.rows; y++)
	{
		for(int x = 0; x < g_srcImage.cols; x++)
		{
			for(int c = 0; c < 3; c++ )
			{
				g_dstImage.at<Vec3b>(y,x)[c] = saturate_cast<uchar>((g_nContrastValue*0.01)*(g_srcImage.at<Vec3b>(y,x)[c]) + g_nBrightValue);
			}
		}
	}

	imshow("[ori picture]",g_srcImage);
	imshow("[effect picture]",g_dstImage);
}







