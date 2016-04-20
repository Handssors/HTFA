#include "StdAfx.h"
#include "FrameToBinary.h"




FrameToBinary::FrameToBinary(void)
	: pBrImg(NULL)
	, pFrmImg(NULL)
	, pSubImg(NULL)
	, pBkGrayImg(NULL)
	, pBinary(NULL)
	, pFrameImg(NULL)
{
}


FrameToBinary::~FrameToBinary(void)
{
}


// ������ͼ�ʹ���ȡ֡���뷵�ض�ֵ��ͼ
 IplImage*  FrameToBinary::main(IplImage* pBackImg, IplImage* pFirImg)
{
	pFrameImg=pFirImg;

	pBrImg=pBackImg;

	//ͼ���ʼ��

	pBkGrayImg=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	pSubImg=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	pFrmImg=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	pBinary=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	//�Ա���ͼ���д���
	cvCvtColor(pBrImg,pBkGrayImg,CV_BGR2GRAY);

	//���Ҷ�ͼ����и�˹ƽ��
	cvSmooth(pBkGrayImg,pBkGrayImg,CV_GAUSSIAN,5,0,0);

	FrameSub();

	OnBinary();

	OnOpen();

	OnRemovenoise();

	OnImgFill();

	return pBinary;
}


// ��֡�����֡��ͼ
void FrameToBinary::FrameSub(void)
{
	//֡ͼ��ת��Ϊ�Ҷ�ͼ
	cvCvtColor(pFrameImg,pFrmImg,CV_BGR2GRAY);
	 
	//��ͼ����и�˹ƽ��
	cvSmooth(pFrmImg,pFrmImg,CV_GAUSSIAN,5,0,0);

	//�����֡
	cvAbsDiff(pFrmImg,pBkGrayImg,pSubImg);

}

//��ֵ��
void FrameToBinary::OnBinary(void)
{

	cvThreshold(pSubImg,pBinary,0,255,CV_THRESH_OTSU);

}

// ������
void FrameToBinary::OnOpen(void)
{
	//���帯ʴ�����͵ĺ�
	IplConvKernel* kernel_5_5;
	kernel_5_5=cvCreateStructuringElementEx(5,5,2,2,CV_SHAPE_ELLIPSE,0);

	IplConvKernel* kernel_7_7;
	kernel_7_7=cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE,0);

	//��ǰ��ͼ����п������Գ�ȥ�ӵ㣬�ָ���˶�����
	//��ͼ����и�ʴ���㣨��������ȥ������䰵��
	cvErode(pBinary,pBinary,kernel_5_5,1);

	//��ͼ������������㣬�����
	cvDilate(pBinary,pBinary,kernel_7_7,1);

}

// ȥ������
void FrameToBinary::OnRemovenoise(void)
{
	//ȥ����С����ɢ��ͨ����
	//ͼ����
	IplImage* pBinary2; 
	pBinary2=cvCloneImage(pBinary);
	//�����ڴ�ռ�
	CvMemStorage* storage=cvCreateMemStorage(0);

	//����ͳ����ͨ��������������
	CvSeq* area_contour=NULL;
	int Number_contours=NULL;  //��������
	double Area=0;  //�������
	CvScalar s_fill_new;
	s_fill_new.val[0]=0;
	CvScalar loDiff;
	loDiff.val[0]=10;
	CvScalar upDiff;
	upDiff.val[0]=10;

	//����ͼ���д�����ⲿ����
	Number_contours=cvFindContours(pBinary,storage,&area_contour,sizeof(CvContour),CV_RETR_EXTERNAL);

	//ͳ���������,�������С����ֵ����ͨ�������ɾ��
	for(CvSeq * c=area_contour;c!=NULL;c=c->h_next)
	{
		//����������
		Area=fabs(cvContourPerimeter(c));
		if(Area<35)
		{
			for(int i=0;i<1;i++)
			{
				//ȡ�����ϵ�һ������Ϊͼ��������ӵ�
				CvPoint * seedpoint=CV_GET_SEQ_ELEM(CvPoint,c,i);
				//����ˮ����㷨��С���������Ϊ����ɫ
				cvFloodFill(pBinary2,*seedpoint,s_fill_new,loDiff,upDiff,NULL,4,0);
			}
		}
	}
	pBinary=cvCloneImage(pBinary2);
}

// ͼ�����
void FrameToBinary::OnImgFill(void)
{
	CvScalar s;
	CvScalar s_new;
	s_new.val[0]=255;
	//��¼����е�����λ��
	int left_X=0;
	//��¼��������λ��
	int Right_X=0;
	//��¼����е�����λ��
	int Top_Y=0;
	//��¼����е�����λ��
	int Bottom_Y=0;

	//�������
	for(int wt=0;wt<pBinary->width;wt++)
	{
		for(int ht=0;ht<pBinary->height;ht++)
		{
			s=cvGet2D(pBinary,ht,wt);
			//�����ҳ���ߵ����͵�
			if(255==s.val[0])
			{
				Top_Y=ht;
				for(int m=pBinary->height-1;m>=0;m--)
				{
					s=cvGet2D(pBinary,m,wt);
					if(s.val[0]==255)
					{
						Bottom_Y=m;
						break;
					}
				}
				//��top_y��bottom_y֮��ĵ�����Ϊ��ɫ
				for(int i=Top_Y;i<Bottom_Y;i++)
				{
					cvSet2D(pBinary,i,wt,s_new);
				}
				ht=pBinary->height;//ǿ����������ѭ��
			}
		}
	}
	/*--------�������-------*/

	//�������
	for(int ht=0;ht<pBinary->height;ht++)
	{
		for(int wt=0;wt<pBinary->width;wt++)
		{
			s=cvGet2D(pBinary,ht,wt);

			if(s.val[0]==255)//255�ǰ�ɫ
			{
				left_X=wt;//�ҵ�����ߵİ�ɫ��
				for(int m=pBinary->width-1;m>0;m--)
				{
					s=cvGet2D(pBinary,ht,m);
					if(s.val[0]==255)
					{
						Right_X=m;//�ҵ����ұߵĵ�
						break;
					}
				}
				//�������
				for(int i=left_X;i<Right_X;i++)
				{
					cvSet2D(pBinary,ht,i,s_new);
				}
				wt=pBinary->width;//ǿ������ѭ��
			}

		}
	}
}


