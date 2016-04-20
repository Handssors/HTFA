//将前景图转化为二值图

#include "opencv2/opencv.hpp"





#pragma once
class FrameToBinary
{
public:
	FrameToBinary(void);
	~FrameToBinary(void);
     

private:
	// 背景图
    IplImage* pBrImg;
	// 待处理帧
	IplImage* pFrmImg;
	// 帧差法结果图
	IplImage* pSubImg;
	// 背景图的灰度图
	IplImage* pBkGrayImg;
	// 二值化图
	IplImage* pBinary;
public:
	// 将背景图和待提取帧传入返回二值化图
	 IplImage*  main(IplImage* pBrImg, IplImage* pFrmImg);
private:
	// 两帧相减得帧差图
	void FrameSub(void);

	//二值化
	void OnBinary(void);
	// 待提取帧的中间帧
	IplImage* pFrameImg;

	//开运算
	void OnOpen(void);

	//去除噪声
    void OnRemovenoise(void);

	//图像填充
    void OnImgFill(void);

	
};

