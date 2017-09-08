#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

bool MultiChannelBlending();

int main()
{
	system("color 5E");

	if(MultiChannelBlending())
	{
		cout<<endl<<"ok,show your image";	
	}

	waitKey(0);
	return 0;
}

bool MultiChannelBlending()
{
	Mat srcImage;
	Mat logoImage;
	vector<Mat> channels;
	Mat imageBlueChannel;


	logoImage = imread("dota_logo.jpg",0);
	if(!logoImage.data)
	{
		cout << "load dota_logo error\n";
		return false;
	}

	srcImage = imread("dota_jugg.jpg");
	if(!srcImage.data)
	{
		cout << "load srcImage error\n";
		return false;
	}

	split(srcImage,channels);

	imageBlueChannel = channels.at(0);// 0--B 1--G  2--R

	addWeighted(imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)),1.0,logoImage,0.5,0,imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)));

	merge(channels,srcImage);

	namedWindow("<1>playgame+logo blue channel");
	imshow("<1>playgame+logo blue channel",srcImage);

	return true;
}



