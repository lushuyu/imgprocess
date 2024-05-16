#pragma once
#include "afxdialogex.h"


class GetPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GetPDlg)

public:
	GetPDlg(CWnd* pParent = nullptr);
	virtual ~GetPDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = GET_PIXEL_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()

public:
	int m_nX;
	int m_nY;
};