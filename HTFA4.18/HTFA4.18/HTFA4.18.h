
// HTFA4.18.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHTFA418App:
// �йش����ʵ�֣������ HTFA4.18.cpp
//

class CHTFA418App : public CWinApp
{
public:
	CHTFA418App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHTFA418App theApp;