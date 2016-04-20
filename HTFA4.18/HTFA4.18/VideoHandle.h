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
	// 获取的视频
	CvCapture* pCapture;

	// 上采样
    inline IplImage* UpSample(IplImage* pFirImg, int multipate);
	// 下采样
	inline IplImage* DownSample(IplImage* pFirImg, int multipate);
	// 加载视频
	char* LoadCapture(char* FileName);
	// 帧读取并处理
	void FrameHandle(int Speed);
	// 唯一图像处理类
	static FrameToBinary SingleFrameToBinary;
	// 背景提取
	void BKextract(void);
};

