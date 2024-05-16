#pragma once

#include "afxdialogex.h"

// CMedianFilterDialog 对话框
class CMedianFilterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMedianFilterDialog)

public:
	CMedianFilterDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMedianFilterDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEDIAN_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_filterSize;
};
