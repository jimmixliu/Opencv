#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <iostream>  
#include <stdlib.h>
#include <stdio.h>
  
using namespace cv;  
using namespace std;  

Mat g_srcImage,g_dstImage,g_grayImage,g_maskImage;
int g_nFillMode = 1;
int g_nLowDifference = 20,g_nUpDifference = 20;
int g_nConnectivity = 4;
bool g_bIsColor = true;
bool g_bUseMask = false;
int g_nNewMaskVal = 255;

static void ShowHelpText()
{
    printf("\n\n\n\t欢迎来到漫水填充示例程序~\n\n");    
    printf( "\n\n\t按键操作说明: \n\n"    
        "\t\t鼠标点击图中区域- 进行漫水填充操作\n"    
        "\t\t键盘按键【ESC】- 退出程序\n"    
        "\t\t键盘按键【1】-  切换彩色图/灰度图模式\n"    
        "\t\t键盘按键【2】- 显示/隐藏掩膜窗口\n"    
        "\t\t键盘按键【3】- 恢复原始图像\n"    
        "\t\t键盘按键【4】- 使用空范围的漫水填充\n"    
        "\t\t键盘按键【5】- 使用渐变、固定范围的漫水填充\n"    
        "\t\t键盘按键【6】- 使用渐变、浮动范围的漫水填充\n"    
        "\t\t键盘按键【7】- 操作标志符的低八位使用4位的连接模式\n"    
        "\t\t键盘按键【8】- 操作标志符的低八位使用8位的连接模式\n"    
        "\n\n\t\t\t\t\t\t\t\t by浅墨\n\n\n"    
        ); 
}

static void onMouse(int event, int x, int y, int, void*)
{
	//printf("x = %d, y = %d\n",x,y);
	if(event != CV_EVENT_LBUTTONDOWN){
		return ;
	}

	Point seed = Point(x,y);
	int LowDifference = g_nFillMode  == 0 ? 0 : g_nLowDifference;
	int UpDifference = g_nFillMode  == 0 ? 0 : g_nUpDifference;
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) + (g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);

	int b = (unsigned)theRNG() & 255;
	int g = (unsigned)theRNG() & 255;
	int r = (unsigned)theRNG() & 255;

	Rect ccomp;

	Scalar  newVal = g_bIsColor ? Scalar(b,g,r):Scalar(r*0.299 + g*0.587 + b*0.114);

	Mat dst = g_bIsColor ? g_dstImage:g_grayImage;

	int area;

	if(g_bUseMask){
		threshold(g_maskImage,g_maskImage,1,128,CV_THRESH_BINARY);
		area = floodFill(dst,g_maskImage,seed,newVal,&ccomp,Scalar(LowDifference,LowDifference,LowDifference),Scalar(UpDifference,UpDifference,UpDifference),flags);
		imshow("mask",g_maskImage);
	}else{
		area = floodFill(dst,seed,newVal,&ccomp,Scalar(LowDifference,LowDifference,LowDifference),Scalar(UpDifference,UpDifference,UpDifference),flags);
	}

	imshow("效果图",dst);
	cout<<area<<"个像素被重绘\n";
}
  
int main(int argc, char **argv)  
{

	g_srcImage = imread("1.jpg");
	if(!g_srcImage.data){
		printf("load 1.jpg failed\n");
		return false;
	}

	ShowHelpText();
	
	g_srcImage.copyTo(g_dstImage);
	cvtColor(g_srcImage,g_grayImage,COLOR_BGR2GRAY);
	g_maskImage.create(g_srcImage.rows+2,g_srcImage.cols+2,CV_8UC1);

	namedWindow("效果图",CV_WINDOW_AUTOSIZE);
	createTrackbar("LowDifference","效果图",&g_nLowDifference,255,0);
	createTrackbar("UpDifference","效果图",&g_nUpDifference,255,0);

	setMouseCallback("效果图",onMouse,0);

	while(1){
		imshow("效果图",g_bIsColor ? g_dstImage:g_grayImage);

		int c = waitKey(0);
		switch((char)c){
			case 27:
        			printf("\t\t键盘按键【ESC】- 退出程序\n");    
				return 0;
			break;

			case '1':
        			printf("\t\t键盘按键【1】-  切换彩色图/灰度图模式\n");

				if(g_bIsColor){
					cvtColor(g_srcImage,g_grayImage,COLOR_BGR2GRAY);
					g_maskImage = Scalar::all(0);
					g_bIsColor = false;
				}else{
					g_srcImage.copyTo(g_dstImage);
					g_maskImage = Scalar::all(0);
					g_bIsColor = true;
				}
				
			break;
			case '2':
       				printf( "\t\t键盘按键【2】- 显示/隐藏掩膜窗口\n");

				if(g_bUseMask){
					destroyWindow("mask");
					g_bUseMask = false;
				}else{
					namedWindow("mask");
					g_maskImage = Scalar::all(0);
					imshow("mask",g_maskImage);
					g_bUseMask = true; 
				}

			break;
			case '3':
       				printf( "\t\t键盘按键【3】- 恢复原始图像\n");    
				g_srcImage.copyTo(g_dstImage);
				cvtColor(g_srcImage,g_grayImage,COLOR_BGR2GRAY);
				g_maskImage = Scalar::all(0);
			
			break;
			case '4':
       				printf( "\t\t键盘按键【4】- 使用空范围的漫水填充\n");
				g_nFillMode = 0;
			break;
			case '5':
       				printf( "\t\t键盘按键【5】- 使用渐变、固定范围的漫水填充\n");   
				g_nFillMode = 1;
			break;
			case '6':
       				printf( "\t\t键盘按键【6】- 使用渐变、浮动范围的漫水填充\n"); 
				g_nFillMode = 2;
			break;
			case '7':
       				printf( "\t\t键盘按键【7】- 操作标志符的低八位使用4位的连接模式\n");   
				g_nConnectivity = 4;
			break;
			case '8':
       				printf( "\t\t键盘按键【8】- 操作标志符的低八位使用8位的连接模式\n"); 
				g_nConnectivity = 8;
			break;
			default:
				printf("\t\tundefined key\n");
			break;
		}
	}
}
