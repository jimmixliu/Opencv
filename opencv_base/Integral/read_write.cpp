/*
*
*integral
* 
*/
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>
//string

using namespace cv;
using namespace std;

void my_integral(InputArray src, OutputArray sum);

int main(int argc, char **argv)
{

	uchar val[5][5] = {{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};
	Mat src(Size(5,5),CV_8UC1,val);
	cout<<src<<endl;

	Mat sum,sqsum;


	my_integral(src,sum);	

	cout<<"-------------------"<<endl;
	cout<<sum<<endl;




}



void _my_integral( const uchar* src, size_t _srcstep, int* sum, size_t _sumstep, Size size, int cn )
{
    int x, y, k;

    int srcstep = (int)(_srcstep/sizeof(uchar));
    int sumstep = (int)(_sumstep/sizeof(int));

    cout<<"srcstep :"<<srcstep<<endl;
    cout<<"sumstep :"<<sumstep<<endl;


    size.width *= cn;

    memset( sum, 0, (size.width+cn)*sizeof(sum[0]));
    sum += sumstep + cn;
	

        for( y = 0; y < size.height; y++, src += srcstep - cn, sum += sumstep - cn )
        {
            for( k = 0; k < cn; k++, src++, sum++ )
            {
                int s = sum[-cn] = 0;
                for( x = 0; x < size.width; x += cn )
                {
                    s += src[x];
                    sum[x] = sum[x - sumstep] + s;
                }
            }
        }
}




void my_integral(InputArray _src, OutputArray _sum)
{
	Mat src = _src.getMat(),sum;
	int depth = src.depth(), cn = src.channels();
	Size isize(src.cols + 1, src.rows+1);

	//int sdepth = depth == CV_8U ? CV_32S : CV_64F;
	int sdepth = CV_32S;
	_sum.create( isize, CV_MAKETYPE(sdepth, cn) );
	sum = _sum.getMat();


	_my_integral( src.data, src.step, (int *)sum.data, sum.step, src.size(), cn );
}





