#pragma once

#include "afxdialogex.h"

class CBilateralFilterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBilateralFilterDialog)

public:
	CBilateralFilterDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBilateralFilterDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BILATERAL_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	float m_sigma_d;
	float m_sigma_R;
};