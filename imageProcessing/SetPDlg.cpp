#include "imageProcessing.h"
#include "afxdialogex.h"
#include "SetPDlg.h"



IMPLEMENT_DYNAMIC(SetPDlg, CDialogEx)

SetPDlg::SetPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EDITAPIXEL, pParent)
	, X(0)
	, Y(0)
	, Res(0)
	, R(0)
	, G(0)
	, B(0)
{

}

SetPDlg::~SetPDlg()
{
}

void SetPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, X);
	DDX_Text(pDX, IDC_EDIT2, Y);

	DDX_Text(pDX, IDC_EDIT3, Res);
	DDX_Text(pDX, IDC_EDIT4, R);
	DDX_Text(pDX, IDC_EDIT5, G);
	DDX_Text(pDX, IDC_EDIT6, B);
}


BEGIN_MESSAGE_MAP(SetPDlg, CDialogEx)
END_MESSAGE_MAP()