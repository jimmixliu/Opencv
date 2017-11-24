#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


using namespace cv;
using namespace std;

static void ShowHelpText()
{
	printf("\n\n\n\t Opencv pryup/down resize \n\n");
	printf("\n\n\t key: \n\n"
	       "\t\t exit:ESC/q\n"
	       "\t\t key a:pryUp zoom out\n"	
	       "\t\t key d:pryDown zoom in\n"	
	       "\t\t key w:resize zoom out\n"	
	       "\t\t key s:resize zoom in\n"	
		);
}

int main()
{
#if 0
	Mat tmpImage,dstImage1,dstImage2,dstImage3,dstImage4;
	Mat srcImage = 	imread("1.jpg");
        if(!srcImage.data){
        	cout<<endl<<"load 1.jpg failed\n";
                return false;
        }   

	tmpImage = srcImage;

        imshow("[原始图]",srcImage);

	resize(tmpImage,dstImage1,Size(tmpImage.cols/2,tmpImage.rows/2),(0,0),(0,0),3);	
	resize(tmpImage,dstImage2,Size(tmpImage.cols*2,tmpImage.rows*2),(0,0),(0,0),3);	

	imshow("[效果图]resize down",dstImage1);
	imshow("[效果图]resize up",dstImage2);

	pyrUp(tmpImage,dstImage3,Size(tmpImage.cols*2,tmpImage.rows*2));
	imshow("[效果图]pyrUp",dstImage3);

	pyrDown(tmpImage,dstImage4,Size(tmpImage.cols/2,tmpImage.rows/2));
	imshow("[效果图]pyrDown",dstImage4);

	while((char(waitKey(1))!= 'q')){}
#endif
	Mat tmpImage,dstImage;
	char key = 0;	

	Mat srcImage = imread("1.jpg");	
	if(!srcImage.data){
        	cout<<endl<<"load 1.jpg failed\n";
                return false;
        } 

	namedWindow("[Zoom in-out demo]",CV_WINDOW_AUTOSIZE);
	imshow("[Zoom in-out demo]",srcImage);

	tmpImage = srcImage;
	dstImage = srcImage;

	ShowHelpText();

	while(1){
		key = waitKey(0);
		printf("key %d = %c\n",key,key);

		switch(key){
			case 27:// ESC
			case 'q':
				return 0;
			case 'a':
				pyrUp(tmpImage,dstImage,Size(tmpImage.cols*2,tmpImage.rows*2));
				printf("key a %d, pryUp\n",'a');
				imshow("[Zoom in-out demo]",dstImage);
				tmpImage = dstImage;

				break;
			case 'w':
				resize(tmpImage,dstImage,Size(tmpImage.cols*2,tmpImage.rows*2));
				printf("key w, resize zoom out\n");
				imshow("[Zoom in-out demo]",dstImage);
				tmpImage = dstImage;

				break;

			case 's':
				resize(tmpImage,dstImage,Size(tmpImage.cols/2,tmpImage.rows/2));
				printf("key s, resize zoom in\n");
				imshow("[Zoom in-out demo]",dstImage);
				tmpImage = dstImage;

				break;
			case 'd':
				pyrDown(tmpImage,dstImage,Size(tmpImage.cols/2,tmpImage.rows/2));
				printf("key d, pryDown\n");
				imshow("[Zoom in-out demo]",dstImage);
				tmpImage = dstImage;

				break;
			default:
				printf("unsupport key\n");
				break;
		}
	}

	return 0;
}
