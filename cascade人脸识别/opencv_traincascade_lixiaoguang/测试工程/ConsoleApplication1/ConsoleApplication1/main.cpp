// face_detect.cpp : 定义控制台应用程序的入口点。
//
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img,
                   CascadeClassifier& cascade, 
                   double scale);

String cascadeName = "cascade.xml";//人脸的训练数据

int main( int argc, const char** argv )
{
    Mat image;
    CascadeClassifier cascade, nestedCascade;//创建级联分类器对象
    double scale = 1.3;
    //    image = imread("obama_gray.bmp",1);
    image = imread("1.jpg",1);
    namedWindow( "result", 1 );//opencv2.0以后用namedWindow函数会自动销毁窗口

    if( !cascade.load( cascadeName ) )//从指定的文件目录中加载级联分类器
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return 0;
    }

    if( !image.empty() )//读取图片数据不能为空
    {
        detectAndDraw( image, cascade, scale );
        waitKey(0);
    }

    return 0;
}

void detectAndDraw( Mat& img,
                   CascadeClassifier& cascade, 
                   double scale)
{
    int i = 0;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;//用不同的颜色表示不同的人脸

    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//将图片缩小，加快检测速度

    cvtColor( img, gray, CV_BGR2GRAY );//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//将尺寸缩小到1/scale,用线性插值
    equalizeHist( smallImg, smallImg );//直方图均衡

    t = (double)cvGetTickCount();//用来计算算法执行时间

    //检测人脸
    //detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，faces表示检测到的人脸目标序列，1.1表示
    //每次图像尺寸减小的比例为1.1，2表示每一个候选矩形需要记录2个邻居,CV_HAAR_SCALE_IMAGE表示使用haar特征，Size(30, 30)
    //为目标的最小最大尺寸
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );

    t = (double)cvGetTickCount() - t;//相减为算法执行的时间
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        center.x = cvRound((r->x + r->width*0.5)*scale);//还原成原来的大小
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        circle( img, center, radius, color, 3, 8, 0 );
        smallImgROI = smallImg(*r);
    }
    cv::imshow( "result", img );

    cv::imwrite( "result.jpg", img );
}