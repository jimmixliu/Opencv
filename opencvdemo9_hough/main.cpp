#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace cv;
using namespace std;

Mat g_srcImage,g_midImage,g_dstImage;
vector<Vec4i> g_lines;

int g_nthreshold = 100;

static void on_HoughLines(int,void*);


int main()
{

	g_srcImage = imread("1.jpg");
        if(!g_srcImage.data){
                cout<<endl<<"load 1.jpg failed\n";
                return false;
        }   

        imshow("[原始图]",g_srcImage);

	Canny(g_srcImage,g_midImage,50,200,3);
	cvtColor(g_midImage,g_dstImage,CV_GRAY2BGR);


        namedWindow("[效果图]",1);
	createTrackbar("参数值:","[效果图]",&g_nthreshold ,200,on_HoughLines);

	on_HoughLines(0,0);

	while((char(waitKey(1))!= 'q')){}

	return 0;
}


static void on_HoughLines(int,void*)
{
	Mat dstImage = g_dstImage.clone();	
	Mat midImage = g_midImage.clone();

	vector<Vec4i> mylines;
	HoughLinesP(midImage,mylines,1,CV_PI/180,g_nthreshold+1,50,10);

	for(size_t i = 0; i < mylines.size();i++){
		Vec4i l = mylines[i];
		line(dstImage,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(23,180,55),1,CV_AA);
	}

	imshow("[效果图]",dstImage);
}

