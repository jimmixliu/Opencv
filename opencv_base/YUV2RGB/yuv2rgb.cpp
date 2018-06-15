/*
*yuv to rgb
*rgb to yuv 
* method 1: opencv cvtColor
* method 2: float
* method 3: table
* method 4: shift
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
//linux
#include <sys/time.h>

using namespace cv;
using namespace std;

//YUV YCbCr  BGR

#if 0
YUV(256 级别) 可以从8位 RGB 直接计算：

Y = 0.299 R + 0.587 G + 0.114 B

U = - 0.1687 R - 0.3313 G + 0.5 B + 128

V = 0.5 R - 0.4187 G - 0.0813 B + 128

反过来，RGB 也可以直接从YUV (256级别) 计算:

R = Y + 1.402 (Cr-128)

G = Y - 0.34414 (Cb-128) - 0.71414 (Cr-128)

B = Y + 1.772 (Cb-128)

shift:
step 1:
256*Y = (0.299 R + 0.587 G + 0.114 B) * 256

256*U = (- 0.1687 R - 0.3313 G + 0.5 B ) * 256

256*V = (0.5 R - 0.4187 G - 0.0813 B  ) * 256


256*R = (Y + 1.402 Cr) * 256

256*G = (Y - 0.34414 Cb - 0.71414 Cr) * 256

256*B = (Y + 1.772 (Cb)* 256

step 2:
256*Y = 76.544 R + 150.272 G + 29.184 B 

256*U = - 43.1872 R - 84.8128 G + 128 B  

256*V = 128 R - 107.1872 G - 20.8128 B 


256*R = 256Y + 358.912 Cr

256*G = 256Y - 88.09984 Cb - 182.81984 Cr

256*B = 256Y + 453.632 Cb

step 3:

256*Y = 77 R + 150 G + 29 B 

256*U = - 43 R - 85 G + 128 B  

256*V = 128 R - 107 G - 21 B 


256*R = 256Y + 359 Cr

256*G = 256Y - 88 Cb - 183 Cr

256*B = 256Y + 454 Cb

step 4:

Y = (77 R + 150 G + 29 B ) >>8

U = (- 43 R - 85 G + 128 B ) >>8

V = (128 R - 107 G - 21 B ) >>8


R = (256Y + 359 Cr) >>8

G = (256Y - 88 Cb - 183 Cr) >>8

B = (256Y + 454 Cb) >>8

step 5:

Y = ( (R<<6)+(R<<3)+(R<<2)+R + (G<<7)+(G<<4)+(G<<2)+(G<<1) + (B<<4)+(B<<3)+(B<<2)+B ) >>8

U = (- ((R<<5)+(R<<3)+(R<<1)+R) - ((G<<6)+(G<<4)+(G<<2)+(G)) + (B<<7) ) >>8

V = ((R<<128) - ((G<<6)+(G<<5)+(G<<3)+(G<<1)) - ((B<<4)+(B<<2)+B) >>8


R = ((Y<<8) + (Cr<<8)+(Cr<<6)+(Cr<<5)+(Cr<<2)+(Cr<<1)+Cr) >>8

G = ((Y<<8) - ((Cb<<6)+(Cb<<4)+(Cb<<3)) - ((Cr<<7)+(Cr<<5)+(Cr<<4)+(Cr<<2)+(Cr<<1)+Cr) >>8

B = ((Y<<8) + (Cb<<8)+(Cb<<7)+(Cb<<6)+(Cb<<2)+(Cb<<1)) >>8


table:
eg:
Y = 0.299 R + 0.587 G + 0.114 B
Y = D + E + F
D = 0.299R = 1224R >>12
E = 0.587G = 2404G >>12
F = 0.114B =  467B >>12

index = R G  B --> 0--255
vaule = D  E  F


#endif


#if 0
int yuv2rgb(uchar *pYUV, uchar *pBGR24,int width,int height)
{
	int len = width * height;
	uchar *ydata = pYUV;
	uchar *udata = &ydata[len];
	//uchar *vdata = &udata[len+len>>2];
	uchar *vdata = &udata[len];

	int i,j,k;
	int yidx,uidx,vidx,idx;
	int bgr[3];

	for(i = 0; i < height; i++){
		for(j = 0; j < width;j++){
			yidx = i*width + j;
			//uidx = (i/2*width/2) + j/2; 
			uidx = (i*width) + j; 
			vidx = uidx;

			bgr[0] = (int)(ydata[yidx] + 1.732446 * (udata[vidx] - 128));                                     // b分量
            		bgr[1] = (int)(ydata[yidx] - 0.698001 * (udata[uidx] - 128) - 0.703125 * (vdata[vidx] - 128));    // g分量
            		bgr[2] = (int)(ydata[yidx] + 1.370705 * (vdata[uidx] - 128));                                     // r分量

			for(k = 0; k < 3; k++){
				idx = (i * width + j)*3 + k;
				if(bgr[k] >= 0 && bgr[k] <= 255){
					pBGR24[idx] = bgr[k];
				}else{
					pBGR24[idx] = (bgr[k] < 0)?0:255;
				}
			}

		}
	}

	return 0;
}
#endif
int yuv2rgb(uchar *pYUV, uchar *pBGR24,int width,int height)
{
	int len = width * height;
	uchar *ydata = pYUV;

	int i,j,k;
	int idx;
	
	float bgr[3];


	for(i = 0; i < height; i++){
		for(j = 0; j < width*3;j+=3){
			idx = i*width*3 + j;

			bgr[0] = ((float)ydata[idx] + 1.772 * ((float)ydata[idx+1] - 128));                                     // b分量
            		bgr[1] = ((float)ydata[idx] - 0.34414 * ((float)ydata[idx+1] - 128) - 0.71414 * ((float)ydata[idx+2] - 128));    // g分量
            		bgr[2] = ((float)ydata[idx] + 1.402 * ((float)ydata[idx+2] - 128));                                     // r分量

			for(k = 0; k < 3; k++){
				if(bgr[k] < 0){
					bgr[k] = 0;
				}else if(bgr[k] > 255){
					bgr[k] = 255;
				}
				pBGR24[idx+k] = (uchar)bgr[k];
			}	
		}
	}
	return 0;
}

int rgb2yuv(uchar *pBGR24,uchar *pYUV,int width,int height)
{
	int len = width * height;
	uchar *data = pBGR24;

	int i,j,k;
	int idx;
	

	float yuv[3];

	for(i = 0; i < height; i++){
		for(j = 0; j < width*3;j+=3){
			idx = i*width*3 + j;

			yuv[0] = ((float)data[idx] * 0.114 + 0.587 * (float)data[idx+1] + 0.299*(float)data[idx+2]); // y分量
            		yuv[1] = ( -0.1687*(float)data[idx+2] - 0.3313 * (float)data[idx+1] + 0.5 *(float)data[idx] + 128);// u分量
            		yuv[2] = (0.5 * (float)data[idx+2] - 0.4187 *(float)data[idx+1] - 0.0813*(float)data[idx] + 128); // v分量

			for(k = 0; k < 3; k++){
				if(yuv[k] < 0){
					yuv[k] = 0;
				}else if(yuv[k] > 255){
					yuv[k] = 255;
				}

				pYUV[idx+k] = (uchar)yuv[k];
			}	
		}
	}
	return 0;
}

int rgb2yuv_shift(uchar *pBGR24,uchar *pYUV,int width,int height)
{
	int len = width * height;
	uchar *data = pBGR24;

	int i,j,k;
	int idx;
	
	int yuv[3],R,G,B;

	for(i = 0; i < height; i++){
		for(j = 0; j < width*3;j+=3){
			idx = i*width*3 + j;

			B = (int)pBGR24[idx];
			G = (int)pBGR24[idx+1];
			R = (int)pBGR24[idx+2];

			
			yuv[0] = ( (R<<6)+(R<<3)+(R<<2)+R + (G<<7)+(G<<4)+(G<<2)+(G<<1) + (B<<4)+(B<<3)+(B<<2)+B ) >>8;
			
			yuv[1] = ((- ((R<<5)+(R<<3)+(R<<1)+R) - ((G<<6)+(G<<4)+(G<<2)+(G)) + (B<<7)) >>8) + 128;
			
			yuv[2] = (((R<<7) - ((G<<6)+(G<<5)+(G<<3)+(G<<1)) - ((B<<4)+(B<<2)+B)) >>8) +128;

			for(k = 0; k < 3; k++){
				if(yuv[k] < 0){
					yuv[k] = 0;
				}else if(yuv[k] > 255){
					yuv[k] = 255;
				}

				pYUV[idx+k] = (uchar)yuv[k];
			}
		}
	}
	return 0;
}



int yuv2rgb_shift(uchar *pYUV, uchar *pBGR24,int width,int height)
{
	int len = width * height;
	uchar *ydata = pYUV;

	int i,j,k;
	int idx;
	
	int bgr[3],Y,Cb,Cr;

	for(i = 0; i < height; i++){
		for(j = 0; j < width*3;j+=3){
			idx = i*width*3 + j;

			Y = (int)ydata[idx];
			Cb = (int)(ydata[idx+1] - 128);
			Cr = (int)(ydata[idx+2] - 128);

			bgr[2] = ((Y<<8) + (Cr<<8)+(Cr<<6)+(Cr<<5)+(Cr<<2)+(Cr<<1)+Cr) >>8;
			
			bgr[1] = ((Y<<8) - ((Cb<<6)+(Cb<<4)+(Cb<<3)) - ((Cr<<7)+(Cr<<5)+(Cr<<4)+(Cr<<2)+(Cr<<1)+Cr)) >>8;
			
			bgr[0] = ((Y<<8) + (Cb<<8)+(Cb<<7)+(Cb<<6)+(Cb<<2)+(Cb<<1)) >>8;

			for(k = 0; k < 3; k++){
			if(bgr[k] < 0){
				bgr[k] = 0;
			}else if(bgr[k] > 255){
				bgr[k] = 255;
			}
				pBGR24[idx+k] = (uchar)bgr[k];
			}	
		}
	}
	return 0;
}

#if 0
U = - 0.1687 R - 0.3313 G + 0.5 B + 128

V = 0.5 R - 0.4187 G - 0.0813 B + 128


table:
eg:
Y = 0.299 R + 0.587 G + 0.114 B
Y = D + E + F
D = 0.299R = 1224R >>12
E = 0.587G = 2404G >>12
F = 0.114B =  467B >>12

U = - 0.1687 R - 0.3313 G + 0.5 B + 128
D = 0.1687R =  691R >>12
E = 0.3313G = 1357G >>12
F = 0.5B =    2048B >>12

V = 0.5 R - 0.4187 G - 0.0813 B + 128
D = 0.5R =     2048R >>12
E = 0.4187G >> 1715G >>12
F = 0.0813B >>  333B >>12

#endif
static int  Y_B[256];
static int  Y_G[256];
static int  Y_R[256];

static int  U_B[256];
static int  U_G[256];
static int  U_R[256];

static int  V_B[256];
static int  V_G[256];
static int  V_R[256];

#if 0
R = Y + 1.402 (Cr-128)
D = 1.402(Cr-128) = 5742(Cr-128)>>12;

G = Y - 0.34414 (Cb-128) - 0.71414 (Cr-128)
D = 0.34414(Cb-128) = 1410(Cb-128)>>12
E = 0.71414(Cr-128) = 2925(Cb-128)>>12

B = Y + 1.772 (Cb-128)
D = 1.772(Cb-128) = 7258(Cb-128) >> 12
#endif
static int B_U[256];
static int G_U[256];
static int G_V[256];
static int R_V[256];


int table_init()
{
	for(int i = 0; i < 256; i++){
		Y_B[i] =  (467 * i) >> 12;
		Y_G[i] = (2404 * i) >> 12;
		Y_R[i] = (1224 * i) >> 12;

		U_B[i] = (2048 * i) >> 12;
		U_G[i] = (1357 * i) >> 12; 
		U_R[i] =  (691 * i) >> 12;
		      
		V_B[i] =  (333 * i) >> 12;
		V_G[i] = (1715 * i) >> 12;
		V_R[i] = (2048 * i) >> 12;

		B_U[i] = (7258 * (i - 128)) >> 12;
		G_U[i] = (1410 * (i - 128)) >> 12;
		G_V[i] = (2925 * (i - 128)) >> 12;
		R_V[i] = (5742 * (i - 128)) >> 12;
	}

	return 0;
}

int rgb2yuv_table(uchar *pBGR24,uchar *pYUV,int width,int height)
{
	int len = width * height;
	uchar *data = pBGR24;

	int i,j,k;
	int idx;
	
	int yuv[3],R,G,B;

	for(i = 0; i < height; i++){
		for(j = 0; j < width*3;j+=3){
			idx = i*width*3 + j;

			B = (int)pBGR24[idx];
			G = (int)pBGR24[idx+1];
			R = (int)pBGR24[idx+2];

			
			yuv[0] = Y_R[R] + Y_G[G] + Y_B[B];
			
			yuv[1] = -U_R[R] - U_G[G] + U_B[B] + 128;
			
			yuv[2] = V_R[R] - V_G[G] - V_B[B] + 128;

			for(k = 0; k < 3; k++){
				if(yuv[k] < 0){
					yuv[k] = 0;
				}else if(yuv[k] > 255){
					yuv[k] = 255;
				}

				pYUV[idx+k] = (uchar)yuv[k];
			}
		}
	}
	return 0;
}

int yuv2rgb_table(uchar *pYUV,uchar *pBGR24,int width,int height)
{
	int len = width * height;
	uchar *data = pBGR24;

	int i,j,k;
	int idx;
	
	int bgr[3],Y,Cb,Cr;

	for(i = 0; i < height; i++){
		for(j = 0; j < width*3;j+=3){
			idx = i*width*3 + j;

			Y = (int)pYUV[idx];
			Cb = (int)pYUV[idx+1];
			Cr = (int)pYUV[idx+2];

			
			bgr[0] = Y + B_U[Cb];
			
			bgr[1] = Y -G_U[Cb] - G_V[Cr];
			
			bgr[2] = Y + R_V[Cr];

			for(k = 0; k < 3; k++){
				if(bgr[k] < 0){
					bgr[k] = 0;
				}else if(bgr[k] > 255){
					bgr[k] = 255;
				}

				pBGR24[idx+k] = (uchar)bgr[k];
			}
		}
	}
	return 0;
}



int main(int argc, char **argv)
{

	string file_name = argv[1]; 
	Mat src = imread(file_name);

	if(src.empty()){
		cout<<file_name<<"can't be loaded"<<endl;
		return -1;
	}

	cout<<"color:"<<src.type()<<" rows:"<<src.rows<<" cols:"<<src.cols<<endl;;
	imshow("src",src);
	clock_t start, end;;
	double seconds;

	Mat yuv;
	Mat rgb;

	start = clock();
	cvtColor(src,yuv,CV_BGR2YUV);//cvtColor(src, dst,CV_BGR2YUV_I420);
	cvtColor(yuv,rgb,CV_YUV2BGR);
	end = clock();

	seconds = (double) (end - start)/CLOCKS_PER_SEC;
	printf("cvtCo time %.8f\n",seconds);
	//imshow("yuv",yuv);
//	imshow("rgb",rgb);

#if 0
	printf("src\n");
	for(int i = 0; i < 6; i++ ){
		printf("%d\n",src.data[i]);
	}
	printf("yuv\n");
	for(int i = 0; i < 6; i++){
		printf("%d\n",yuv.data[i]);
	}
#endif

	Mat rgb1 = Mat::zeros(src.size(),src.type());	
	Mat yuv1 = Mat::zeros(src.size(),src.type());	

	start = clock();
	rgb2yuv(src.data,yuv1.data,src.cols,src.rows);
	yuv2rgb(yuv1.data,rgb1.data,src.cols,src.rows);
	end = clock();

	seconds = (double) (end - start)/CLOCKS_PER_SEC;
	printf("float time %.8f\n",seconds);

	//imshow("yuv1",yuv1);
//	imshow("rgb1",rgb1);
#if 0
	printf("yuv1\n");
	for(int i = 0; i < 6; i++){
		printf("%d\n",yuv1.data[i]);
	}

	printf("rgb1\n");
	for(int i = 0; i < 6; i++){
		printf("%d\n",rgb1.data[i]);
	}
#endif

	Mat rgb2 = Mat::zeros(src.size(),src.type());	
	Mat yuv2 = Mat::zeros(src.size(),src.type());	

	start = clock();
	rgb2yuv_shift(src.data,yuv2.data,src.cols,src.rows);
	yuv2rgb_shift(yuv2.data,rgb2.data,src.cols,src.rows);
	end = clock();

	seconds = (double) (end - start)/CLOCKS_PER_SEC;
	printf("shift time %.8f\n",seconds);

	//imshow("yuv2",yuv2);
//	imshow("rgb2",rgb2);

#if 0
	printf("yuv2\n");
	for(int i = 0; i < 6; i++){
		printf("%d\n",yuv2.data[i]);
	}

	printf("rgb2\n");
	for(int i = 0; i < 6; i++){
		printf("%d\n",rgb2.data[i]);
	}
#endif

	Mat rgb3 = Mat::zeros(src.size(),src.type());	
	Mat yuv3 = Mat::zeros(src.size(),src.type());	
	table_init();

	start = clock();
	rgb2yuv_table(src.data,yuv3.data,src.cols,src.rows);
	yuv2rgb_table(yuv3.data,rgb3.data,src.cols,src.rows);
	end = clock();

	seconds = (double) (end - start)/CLOCKS_PER_SEC;
	printf("shift time %.8f\n",seconds);

	imshow("yuv3",yuv3);
	imshow("rgb3",rgb3);


	waitKey(0);
	
	return 0;
}




