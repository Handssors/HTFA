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


// 将背景图和待提取帧传入返回二值化图
 IplImage*  FrameToBinary::main(IplImage* pBackImg, IplImage* pFirImg)
{
	pFrameImg=pFirImg;

	pBrImg=pBackImg;

	//图像初始化

	pBkGrayImg=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	pSubImg=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	pFrmImg=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	pBinary=cvCreateImage(cvSize(pBrImg->width,pBrImg->height),8,1);

	//对背景图进行处理
	cvCvtColor(pBrImg,pBkGrayImg,CV_BGR2GRAY);

	//将灰度图像进行高斯平滑
	cvSmooth(pBkGrayImg,pBkGrayImg,CV_GAUSSIAN,5,0,0);

	FrameSub();

	OnBinary();

	OnOpen();

	OnRemovenoise();

	OnImgFill();

	return pBinary;
}


// 两帧相减得帧差图
void FrameToBinary::FrameSub(void)
{
	//帧图像转化为灰度图
	cvCvtColor(pFrameImg,pFrmImg,CV_BGR2GRAY);
	 
	//将图像进行高斯平滑
	cvSmooth(pFrmImg,pFrmImg,CV_GAUSSIAN,5,0,0);

	//相减得帧
	cvAbsDiff(pFrmImg,pBkGrayImg,pSubImg);

}

//二值化
void FrameToBinary::OnBinary(void)
{

	cvThreshold(pSubImg,pBinary,0,255,CV_THRESH_OTSU);

}

// 开运算
void FrameToBinary::OnOpen(void)
{
	//定义腐蚀和膨胀的核
	IplConvKernel* kernel_5_5;
	kernel_5_5=cvCreateStructuringElementEx(5,5,2,2,CV_SHAPE_ELLIPSE,0);

	IplConvKernel* kernel_7_7;
	kernel_7_7=cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE,0);

	//对前景图像进行开运算以除去杂点，分割出运动物体
	//对图像进行腐蚀运算（将明亮点去除，会变暗）
	cvErode(pBinary,pBinary,kernel_5_5,1);

	//对图像进行膨胀运算，会变亮
	cvDilate(pBinary,pBinary,kernel_7_7,1);

}

// 去除噪声
void FrameToBinary::OnRemovenoise(void)
{
	//去除较小的离散连通区域
	//图像复制
	IplImage* pBinary2; 
	pBinary2=cvCloneImage(pBinary);
	//定义内存空间
	CvMemStorage* storage=cvCreateMemStorage(0);

	//定义统计连通区域轮廓的序列
	CvSeq* area_contour=NULL;
	int Number_contours=NULL;  //轮廓个数
	double Area=0;  //轮廓面积
	CvScalar s_fill_new;
	s_fill_new.val[0]=0;
	CvScalar loDiff;
	loDiff.val[0]=10;
	CvScalar upDiff;
	upDiff.val[0]=10;

	//查找图像中大的最外部轮廓
	Number_contours=cvFindContours(pBinary,storage,&area_contour,sizeof(CvContour),CV_RETR_EXTERNAL);

	//统计轮廓面积,对于面积小于阈值的连通区域进行删除
	for(CvSeq * c=area_contour;c!=NULL;c=c->h_next)
	{
		//获得轮廓面积
		Area=fabs(cvContourPerimeter(c));
		if(Area<35)
		{
			for(int i=0;i<1;i++)
			{
				//取轮廓上的一个点作为图像填充种子点
				CvPoint * seedpoint=CV_GET_SEQ_ELEM(CvPoint,c,i);
				//用漫水填充算法将小的轮廓填充为背景色
				cvFloodFill(pBinary2,*seedpoint,s_fill_new,loDiff,upDiff,NULL,4,0);
			}
		}
	}
	pBinary=cvCloneImage(pBinary2);
}

// 图像填充
void FrameToBinary::OnImgFill(void)
{
	CvScalar s;
	CvScalar s_new;
	s_new.val[0]=255;
	//记录填充行的最左位置
	int left_X=0;
	//记录填充的最右位置
	int Right_X=0;
	//记录填充列的最上位置
	int Top_Y=0;
	//记录填充列的最下位置
	int Bottom_Y=0;

	//纵向填充
	for(int wt=0;wt<pBinary->width;wt++)
	{
		for(int ht=0;ht<pBinary->height;ht++)
		{
			s=cvGet2D(pBinary,ht,wt);
			//纵向找出最高点和最低点
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
				//将top_y与bottom_y之间的点设置为白色
				for(int i=Top_Y;i<Bottom_Y;i++)
				{
					cvSet2D(pBinary,i,wt,s_new);
				}
				ht=pBinary->height;//强制跳出此列循环
			}
		}
	}
	/*--------纵向填充-------*/

	//横向填充
	for(int ht=0;ht<pBinary->height;ht++)
	{
		for(int wt=0;wt<pBinary->width;wt++)
		{
			s=cvGet2D(pBinary,ht,wt);

			if(s.val[0]==255)//255是白色
			{
				left_X=wt;//找到最左边的白色点
				for(int m=pBinary->width-1;m>0;m--)
				{
					s=cvGet2D(pBinary,ht,m);
					if(s.val[0]==255)
					{
						Right_X=m;//找到最右边的点
						break;
					}
				}
				//横向填充
				for(int i=left_X;i<Right_X;i++)
				{
					cvSet2D(pBinary,ht,i,s_new);
				}
				wt=pBinary->width;//强制跳出循环
			}

		}
	}
}


