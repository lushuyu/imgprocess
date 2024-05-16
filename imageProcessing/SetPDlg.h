#pragma once
#include "afxdialogex.h"


class SetPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetPDlg)

public:
	SetPDlg(CWnd* pParent = nullptr);  
	virtual ~SetPDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	int X;
	int Y;
	int Res;
	int R;
	int G;
	int B;
};