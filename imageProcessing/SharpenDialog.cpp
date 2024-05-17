#include "imageProcessing.h"
#include "SharpenDialog.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CSharpenDialog, CDialogEx)

CSharpenDialog::CSharpenDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHARPEN_DIALOG, pParent)
	, m_sharpenFactor(1.0f)
{
}

CSharpenDialog::~CSharpenDialog()
{
}

void CSharpenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SHARPEN_FACTOR, m_sharpenFactor);
}

BEGIN_MESSAGE_MAP(CSharpenDialog, CDialogEx)
END_MESSAGE_MAP()
