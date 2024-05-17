#include "imageProcessing.h"
#include "BilateralFilterDialog.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CBilateralFilterDialog, CDialogEx)

CBilateralFilterDialog::CBilateralFilterDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BILATERAL_FILTER, pParent)
	, m_sigma_d(1.0f)
	, m_sigma_R(25.0f)
{
}

CBilateralFilterDialog::~CBilateralFilterDialog()
{
}

void CBilateralFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIGMA_d, m_sigma_d);
	DDX_Text(pDX, IDC_SIGMA_R, m_sigma_R);
}

BEGIN_MESSAGE_MAP(CBilateralFilterDialog, CDialogEx)
END_MESSAGE_MAP()
