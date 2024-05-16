#include "imageProcessing.h"
#include "afxdialogex.h"
#include "GetPDlg.h"


IMPLEMENT_DYNAMIC(GetPDlg, CDialogEx)

GetPDlg::GetPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(GET_PIXEL_DLG, pParent)
	, m_nX(0)
	, m_nY(0)
{
}

GetPDlg::~GetPDlg()
{
}

void GetPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, PixelX, m_nX);
	DDX_Text(pDX, PixelY, m_nY);
	DDV_MinMaxInt(pDX, m_nX, 0, INT_MAX); 
	DDV_MinMaxInt(pDX, m_nY, 0, INT_MAX); 
}

BEGIN_MESSAGE_MAP(GetPDlg, CDialogEx)
END_MESSAGE_MAP()