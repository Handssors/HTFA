
// HTFA4.18Dlg.h : 头文件
//

#pragma once


// CHTFA418Dlg 对话框
class CHTFA418Dlg : public CDialogEx
{
// 构造
public:
	CHTFA418Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HTFA418_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
