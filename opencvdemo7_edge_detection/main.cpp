#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace cv;
using namespace std;

Mat g_srcImage,g_srcGrayImage,g_dstImage;


//Canny
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold=1;

//Sobel
Mat g_sobelGradient_X,g_sobelGradient_Y;
Mat g_sobelAbsGradient_X,g_sobelAbsGradient_Y;
int g_sobelKernelSize=1;

//Laplacian
Mat g_Lap,g_LapAbs;
int g_lapKernelSize=1;

//Roberts
Mat g_roberts;



static void on_Canny(int,void*);
static void on_Sobel(int,void*);
static void on_Laplacian(int,void*);

static void on_Roberts(int,void*);
static void on_Prewitt(int,void*);

int main()
{

	g_srcImage = imread("1.jpg");
        if(!g_srcImage.data){
                cout<<endl<<"load 1.jpg failed\n";
                return false;
        }   
 
        namedWindow("[原始图]");
        imshow("[原始图]",g_srcImage);
	
	g_dstImage.create(g_srcImage.size(),g_srcImage.type());

	cvtColor(g_srcImage,g_srcGrayImage,CV_BGR2GRAY);

        namedWindow("[效果图]Canny边缘检测");
        namedWindow("[效果图]Sobel边缘检测");
        namedWindow("[效果图]Laplacian边缘检测");

	createTrackbar("参数值:","[效果图]Canny边缘检测",&g_cannyLowThreshold,100,on_Canny);

	createTrackbar("参数值:","[效果图]Sobel边缘检测",&g_sobelKernelSize,3,on_Sobel);

	createTrackbar("参数值:","[效果图]Laplacian边缘检测",&g_lapKernelSize,3,on_Laplacian);

	on_Canny(0,0);
	on_Sobel(0,0);
	on_Laplacian(0,0);
	on_Roberts(0,0);
	on_Prewitt(0,0);

	while((char(waitKey(1))!= 'q')){}

	return 0;
}


static void on_Canny(int,void*)
{
	//1, denoise
	blur(g_srcGrayImage,g_cannyDetectedEdges,Size(3,3));

	Canny(g_cannyDetectedEdges,g_cannyDetectedEdges,g_cannyLowThreshold,g_cannyLowThreshold*3,3);

	g_dstImage = Scalar::all(0);

	g_srcImage.copyTo(g_dstImage,g_cannyDetectedEdges);

	imshow("[效果图]Canny边缘检测",g_dstImage);
//	imshow("[效果图]Canny边缘检测",g_cannyDetectedEdges);
}



static void on_Sobel(int,void*)
{
	Sobel(g_srcGrayImage,g_sobelGradient_X,CV_16S,1,0,(2*g_sobelKernelSize+1),1,1,BORDER_DEFAULT);
	convertScaleAbs(g_sobelGradient_X,g_sobelAbsGradient_X);

	Sobel(g_srcGrayImage,g_sobelGradient_Y,CV_16S,0,1,(2*g_sobelKernelSize+1),1,1,BORDER_DEFAULT);
	convertScaleAbs(g_sobelGradient_Y,g_sobelAbsGradient_Y);

	addWeighted(g_sobelAbsGradient_X,0.5,g_sobelAbsGradient_Y,0.5,0,g_dstImage);

	imshow("[效果图]Sobel边缘检测",g_dstImage);
}


static void on_Laplacian(int,void*)
{
	Laplacian(g_srcGrayImage,g_Lap,CV_16S,(2*g_lapKernelSize+1),1,0,BORDER_DEFAULT);
	convertScaleAbs(g_Lap,g_LapAbs);

	imshow("[效果图]Laplacian边缘检测",g_LapAbs);

}

static void on_Roberts(int,void*)
{
	int nRows = g_srcGrayImage.rows;
	int nCols = g_srcGrayImage.cols;

	g_roberts = g_srcGrayImage.clone();

	for(int i = 0; i < nRows - 1; i++){
		for(int j = 0; j < nCols - 1; j++){
			int t1 = g_srcGrayImage.at<uchar>(i,j) - g_srcGrayImage.at<uchar>(i+1,j+1);
			t1 = t1*t1;

			int t2 = g_srcGrayImage.at<uchar>(i+1,j) - g_srcGrayImage.at<uchar>(i,j+1);
			t1 = t2*t2;

			g_roberts.at<uchar>(i,j) = (uchar)sqrt(t1 + t2);
		
		}
	}

	imshow("[效果图]Roberts边缘检测",g_roberts);
}


static void on_Prewitt(int,void*)
{
	int nRows = g_srcGrayImage.rows;
	int nCols = g_srcGrayImage.cols;

	g_roberts = g_srcGrayImage.clone();

	for(int i = 1; i < nRows - 1; i++){
		for(int j = 1; j < nCols - 1; j++){
			int t1 = g_srcGrayImage.at<uchar>(i-1,j+1) + g_srcGrayImage.at<uchar>(i,j+1) + g_srcGrayImage.at<uchar>(i+1,j+1) - g_srcGrayImage.at<uchar>(i-1,j-1) - g_srcGrayImage.at<uchar>(i,j-1) - g_srcGrayImage.at<uchar>(i+1,j-1);
			t1 = t1*t1;

			int t2 = g_srcGrayImage.at<uchar>(i+1,j-1) + g_srcGrayImage.at<uchar>(i+1,j) +  g_srcGrayImage.at<uchar>(i+1,j+1) - g_srcGrayImage.at<uchar>(i-1,j-1) - g_srcGrayImage.at<uchar>(i-1,j) - g_srcGrayImage.at<uchar>(i-1,j+1);
			t1 = t2*t2;

			g_roberts.at<uchar>(i,j) = (uchar)sqrt(t1 + t2);
		
		}
	}

	imshow("[效果图]Prewitt边缘检测",g_roberts);
}













