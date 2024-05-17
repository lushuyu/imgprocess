#pragma once

#include "afxdialogex.h"

// CCannyEdgeDialog 对话框
class CCannyEdgeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyEdgeDialog)

public:
	CCannyEdgeDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCannyEdgeDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANNY_EDGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_lowThreshold;
	int m_highThreshold;
	int m_kernelSize;
};
