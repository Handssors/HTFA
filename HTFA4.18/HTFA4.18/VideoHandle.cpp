#include "StdAfx.h"
#include "VideoHandle.h"
#include "FrameToBinary.h"


VideoHandle::VideoHandle(void)
	: pCapture(NULL)
{
	if (SingleFrameToBinary!=NULL)
	{
		SingleFrameToBinary=FrameToBinary();
	}
}


VideoHandle::~VideoHandle(void)
{

}


// 上采样
inline IplImage* VideoHandle::UpSample(IplImage* pFirImg, int multipate)
{
	Mat src=Mat(pFirImg);
	Mat dst;
	pyrUp(src,dst,Size(src.cols*multipate,src.rows*multipate));

    IplImage* dstImg=&IplImage(dst);
	return dstImg;
}


// 下采样
inline IplImage* VideoHandle::DownSample(IplImage* pFirImg, int multipate)
{
	Mat src=Mat(pFirImg);
	Mat dst;
	pyrDown(src,dst,Size(src.cols*multipate,src.rows*multipate));

	IplImage* dstImg=&IplImage(dst);
	return dstImg;
}


// 加载视频
char* VideoHandle::LoadCapture(char* FileName)
{
     if(!(pCapture=cvCaptureFromFile(FileName)))
	 {
	     return "无法打开文件";
	 }
	 return "成功打开文件";

}



// 帧读取并处理
void VideoHandle::FrameHandle(int Speed)
{
	IplImage* pFrameImg;
	IplImage* pBkImg;
	while(1)
	{
	    pFrameImg=cvQueryFrame(pCapture);
		if(!pFrameImg)
		{
		    break;
		}
	   
	  SingleFrameToBinary.main(DownSample(pBkImg,2),DownSample(pFrameImg,2));
		
      char c=cvWaitKey(Speed);

	}
}


// 背景提取
void VideoHandle::BKextract(void)
{

}
