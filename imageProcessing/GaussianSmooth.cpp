#include "imageProcessing.h"
#include "GaussianSmooth.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGaussianSmoothDialog, CDialogEx)

CGaussianSmoothDialog::CGaussianSmoothDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAUSSIAN_SMOOTH, pParent)
	, m_stdDev(1.0f)
{
}

CGaussianSmoothDialog::~CGaussianSmoothDialog()
{
}

void CGaussianSmoothDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STD_DEV, m_stdDev);
	DDV_MinMaxFloat(pDX, m_stdDev, 0.1f, 10.0f); 
}

BEGIN_MESSAGE_MAP(CGaussianSmoothDialog, CDialogEx)
END_MESSAGE_MAP()
