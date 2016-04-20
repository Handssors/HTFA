//��ǰ��ͼת��Ϊ��ֵͼ

#include "opencv2/opencv.hpp"





#pragma once
class FrameToBinary
{
public:
	FrameToBinary(void);
	~FrameToBinary(void);
     

private:
	// ����ͼ
    IplImage* pBrImg;
	// ������֡
	IplImage* pFrmImg;
	// ֡����ͼ
	IplImage* pSubImg;
	// ����ͼ�ĻҶ�ͼ
	IplImage* pBkGrayImg;
	// ��ֵ��ͼ
	IplImage* pBinary;
public:
	// ������ͼ�ʹ���ȡ֡���뷵�ض�ֵ��ͼ
	 IplImage*  main(IplImage* pBrImg, IplImage* pFrmImg);
private:
	// ��֡�����֡��ͼ
	void FrameSub(void);

	//��ֵ��
	void OnBinary(void);
	// ����ȡ֡���м�֡
	IplImage* pFrameImg;

	//������
	void OnOpen(void);

	//ȥ������
    void OnRemovenoise(void);

	//ͼ�����
    void OnImgFill(void);

	
};

