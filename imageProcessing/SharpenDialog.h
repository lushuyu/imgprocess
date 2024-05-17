#pragma once

#include "afxdialogex.h"

// CSharpenDialog 对话框
class CSharpenDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSharpenDialog)

public:
	CSharpenDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSharpenDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHARPEN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	float m_sharpenFactor;
};
