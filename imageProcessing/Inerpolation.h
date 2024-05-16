#pragma once

#include "afxdialogex.h"

// CInterpolationDialog 对话框
class CInterpolationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInterpolationDialog)

public:
	CInterpolationDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInterpolationDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTERPOLATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog(); // 声明 OnInitDialog 函数

	DECLARE_MESSAGE_MAP()

public:
	float m_xScale;
	float m_yScale;
	int m_interpolationMethod;
	CComboBox m_interpolationMethodCombo; // 声明 COMBOBOX 变量
};
