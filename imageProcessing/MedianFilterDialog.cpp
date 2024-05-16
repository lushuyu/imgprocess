#include "imageProcessing.h"
#include "MedianFilterDialog.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CMedianFilterDialog, CDialogEx)

CMedianFilterDialog::CMedianFilterDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEDIAN_FILTER, pParent)
	, m_filterSize(3)  // 默认滤波器大小为3
{
}

CMedianFilterDialog::~CMedianFilterDialog()
{
}

void CMedianFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILTER_SIZE, m_filterSize);
	DDV_MinMaxInt(pDX, m_filterSize, 1, 15); // 设置滤波器大小的范围
}

BEGIN_MESSAGE_MAP(CMedianFilterDialog, CDialogEx)
END_MESSAGE_MAP()
