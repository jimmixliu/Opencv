/*
*read a seq of img, and show them
* method 1: string and vector
* method 2: string and videocapture
* method 3: read  file name list in file or xml
*/





//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
//string
#include <string>

using namespace cv;
using namespace std;

#define FILE_NUMBER 10

int main(int argc, char **argv)
{
#if 0
	vector<Mat> images;

	for(int i = 0; i < FILE_NUMBER; i++){
		string file_name = format("pic/split_%02d.jpg",i);
		Mat img = imread(file_name);

		if(img.empty()){
			cout<<file_name<<"can't be loaded"<<endl;
			continue;
		}

		images.push_back(img);	
	}

	for(int i = 0; i < images.size(); i++){
		Mat img = images[i];
		imshow("show img",img);
		waitKey();
	}	

	return 0;
#else
	//string file_list = "pic/split_%02d.jpg";	
	string file_list = "pic/kkk.avi";	
	VideoCapture sequence(file_list);
	if(!sequence.isOpened()){
		cout<<"can't be loaed"<<endl;
		return 1;
	}

	Mat image;
	namedWindow("test",1);

	for(;;){
		sequence >> image;
		if(image.empty()){
			cout << "end of sequence" << endl;
			break;
		}

		imshow("test",image);
		waitKey(500);
	}

#endif




}





