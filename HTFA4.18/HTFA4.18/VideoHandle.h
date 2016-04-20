#include<opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

#pragma once
class VideoHandle
{
public:
	VideoHandle(void);
	~VideoHandle(void);
	// ��ȡ����Ƶ
	CvCapture* pCapture;

	// �ϲ���
    inline IplImage* UpSample(IplImage* pFirImg, int multipate);
	// �²���
	inline IplImage* DownSample(IplImage* pFirImg, int multipate);
	// ������Ƶ
	char* LoadCapture(char* FileName);
	// ֡��ȡ������
	void FrameHandle(int Speed);
	// Ψһͼ������
	static FrameToBinary SingleFrameToBinary;
	// ������ȡ
	void BKextract(void);
};

