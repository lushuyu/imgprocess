#include "imageProcessing.h"
#include "CannyEdgeDialog.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CCannyEdgeDialog, CDialogEx)

CCannyEdgeDialog::CCannyEdgeDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CANNY_EDGE, pParent)
	, m_lowThreshold(50)
	, m_highThreshold(150)
	, m_kernelSize(3)
{
}

CCannyEdgeDialog::~CCannyEdgeDialog()
{
}

void CCannyEdgeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOW_THRESHOLD, m_lowThreshold);
	DDX_Text(pDX, IDC_HIGH_THRESHOLD, m_highThreshold);
	DDX_Text(pDX, IDC_KERNEL_SIZE, m_kernelSize);
}

BEGIN_MESSAGE_MAP(CCannyEdgeDialog, CDialogEx)
END_MESSAGE_MAP()
