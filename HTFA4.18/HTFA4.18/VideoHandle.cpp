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


// �ϲ���
inline IplImage* VideoHandle::UpSample(IplImage* pFirImg, int multipate)
{
	Mat src=Mat(pFirImg);
	Mat dst;
	pyrUp(src,dst,Size(src.cols*multipate,src.rows*multipate));

    IplImage* dstImg=&IplImage(dst);
	return dstImg;
}


// �²���
inline IplImage* VideoHandle::DownSample(IplImage* pFirImg, int multipate)
{
	Mat src=Mat(pFirImg);
	Mat dst;
	pyrDown(src,dst,Size(src.cols*multipate,src.rows*multipate));

	IplImage* dstImg=&IplImage(dst);
	return dstImg;
}


// ������Ƶ
char* VideoHandle::LoadCapture(char* FileName)
{
     if(!(pCapture=cvCaptureFromFile(FileName)))
	 {
	     return "�޷����ļ�";
	 }
	 return "�ɹ����ļ�";

}



// ֡��ȡ������
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


// ������ȡ
void VideoHandle::BKextract(void)
{

}
