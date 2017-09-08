#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

bool ROI_AddImage();
bool LinearBlending();
bool ROI_LinearBlending();

#define ALPHA_VALUE 0.5

int main()
{
	system("color 5E");

	if(ROI_AddImage() && LinearBlending() && ROI_LinearBlending())
	{
		cout<<endl<<"ok,show your image";	
	}

	waitKey(0);
	return 0;
}

bool ROI_AddImage()
{
	Mat srcImage1 = imread("dota_pa.jpg");
	if(!srcImage1.data)
	{
		cout << "load srcImage1 error\n";
		return false;
	}

	Mat logoImage = imread("dota_logo.jpg");
	if(!logoImage.data)
	{
		cout << "load dota_logo error\n";
		return false;
	}

	Mat imageROI = srcImage1(Rect(200,250,logoImage.cols,logoImage.rows));

	Mat mask = imread("dota_logo.jpg",0);

	logoImage.copyTo(imageROI,mask);

	namedWindow("<1>show ROI");
	imshow("<1>show ROI",srcImage1);

	return true;
}


bool LinearBlending()
{
	double alphaValue = ALPHA_VALUE;
	double betaValue;
	Mat srcImage2, srcImage3, dstImage;

	srcImage2 = imread("mogu.jpg");
	if(!srcImage2.data)
	{
		cout << "load mogu error\n";
		return false;
	}

	srcImage3 = imread("rain.jpg");
	if(!srcImage3.data)
	{
		cout << "load rain error\n";
		return false;
	}

	betaValue = 1.0 - alphaValue;
	addWeighted(srcImage2,alphaValue, srcImage3, betaValue,0.0,dstImage);

	namedWindow("<2>LinearBlending before",1);
	imshow("<2>LinearBlending before",srcImage2);

	namedWindow("<2>LinearBlending before rain",1);
	imshow("<2>LinearBlending before rain",srcImage3);

	namedWindow("<2>LinearBlending after",1);
	imshow("<2>LinearBlending after",dstImage);

}




bool ROI_LinearBlending()
{
	Mat srcImage1 = imread("dota_pa.jpg");
	if(!srcImage1.data)
	{
		cout << "load srcImage1 error\n";
		return false;
	}

	Mat logoImage = imread("dota_logo.jpg");
	if(!logoImage.data)
	{
		cout << "load dota_logo error\n";
		return false;
	}

	Mat imageROI = srcImage1(Rect(200,250,logoImage.cols,logoImage.rows));

	addWeighted(imageROI,0.5,logoImage,0.3,0.,imageROI);

	namedWindow("<4>ROI LinearBlending");
	imshow("<4>ROI LinearBlending",srcImage1);



}
