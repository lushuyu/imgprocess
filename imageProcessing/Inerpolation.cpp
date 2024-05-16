#include "imageProcessing.h"
#include "Inerpolation.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CInterpolationDialog, CDialogEx)

CInterpolationDialog::CInterpolationDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INTERPOLATION, pParent)
	, m_xScale(1.0f)
	, m_yScale(1.0f)
	, m_interpolationMethod(0)
{
}

CInterpolationDialog::~CInterpolationDialog()
{
}

void CInterpolationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X_SCALE, m_xScale);
	DDX_Text(pDX, IDC_Y_SCALE, m_yScale);
	DDX_CBIndex(pDX, IDC_INTERPOLATION_METHOD, m_interpolationMethod);
	DDX_Control(pDX, IDC_INTERPOLATION_METHOD, m_interpolationMethodCombo); // 绑定 COMBOBOX 控件
}

BEGIN_MESSAGE_MAP(CInterpolationDialog, CDialogEx)
END_MESSAGE_MAP()

BOOL CInterpolationDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 向 COMBOBOX 中添加选项
	m_interpolationMethodCombo.AddString(_T("Nearest Neighbor"));
	m_interpolationMethodCombo.AddString(_T("Bilinear"));
	m_interpolationMethodCombo.SetCurSel(0); // 默认选择第一个选项

	return TRUE;  // return TRUE unless you set the focus to a control
}
