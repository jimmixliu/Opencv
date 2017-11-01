#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


Mat g_srcImage,g_dstImage1,g_dstImage2,g_dstImage3;
int g_nBoxFilterValue=3;
int g_nMeanBlurValue=3;
int g_nGaussianBlurValue=3;

static void on_BoxFilter(int, void *);
static void on_MeanBlur(int,void *);
static void on_GaussianBlur(int, void*);

int main()
{
//	system("color5E");

	g_srcImage = imread("1.jpg");
	if(!g_srcImage.data){
		printf("load 1.jpg failed\n");
		return false;
	}

	g_dstImage1 = g_srcImage.clone();
	g_dstImage2 = g_srcImage.clone();
	g_dstImage3 = g_srcImage.clone();

	namedWindow("[<0> windown ori]",1);
	imshow("[<0> windowna ori]",g_srcImage);


	namedWindow("[<1>boxfilter]",1);
	createTrackbar("kernel value:","[<1>boxfilter]",&g_nBoxFilterValue,40,on_BoxFilter);
	on_BoxFilter(g_nBoxFilterValue,0);
	imshow("[<1>boxfilter]",g_dstImage1);

	namedWindow("[<2>meanblur]",1);
	createTrackbar("kernel value:","[<2>meanblur]",&g_nMeanBlurValue,40,on_BoxFilter);
	on_MeanBlur(g_nMeanBlurValue,0);

	namedWindow("[<3>gaussianblur]",1);
	createTrackbar("kernel value:","[<3>gaussianblur]",&g_nGaussianBlurValue,40,on_GaussianBlur);
	on_BoxFilter(g_nGaussianBlurValue,0);

	cout<<endl<<"\tok, move trackbar\n\n"
		<<"\t press q, exit!\n"
		<<"\n\n\t\t\t\t";

	while(char(waitKey(1)!= 'q')){}

	return 0;
}


static void on_BoxFilter(int,void *)
{
	boxFilter(g_srcImage,g_dstImage1,-1,Size(g_nBoxFilterValue+1,g_nBoxFilterValue+1));

	imshow("[<1>]boxfilter",g_dstImage1);
}


static void on_MeanBlur(int, void *)
{
	blur(g_srcImage,g_dstImage2,Size(g_nMeanBlurValue+1, g_nMeanBlurValue+1), Point(-1,-1));

	imshow("[<2>]meanblur",g_dstImage2);
}


static void on_GaussianBlur(int,void*)
{
	GaussianBlur(g_srcImage,g_dstImage3,Size(g_nGaussianBlurValue*2+1,g_nGaussianBlurValue*2+1),0,0);

	imshow("[<3>]gaussianblur",g_dstImage3);
}


