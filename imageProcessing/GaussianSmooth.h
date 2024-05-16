#pragma once

#include "afxdialogex.h"

// CGaussianSmoothDialog 对话框
class CGaussianSmoothDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CGaussianSmoothDialog)

public:
	CGaussianSmoothDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGaussianSmoothDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAUSSIAN_SMOOTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	float m_stdDev;
};
