// imageProcessingView.cpp: CimageProcessingView 类的实现
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "imageProcessing.h"
#include "imageProcessingDoc.h"
#include "imageProcessingView.h"
#include "_GlobalCommon.h"

#include "GetPDlg.h"
#include "SetPDlg.h"
#include "Inerpolation.h"
#include "GaussianSmooth.h"
#include "MedianFilterDialog.h"
#include "BilateralFilterDialog.h"
#include "SharpenDialog.h"
#include "CannyEdgeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CimageProcessingView, CView)

BEGIN_MESSAGE_MAP(CimageProcessingView, CView)
	ON_COMMAND(ID_IMAGEPROCESS_OPENBMPFILE, &CimageProcessingView::OnImageprocessOpenbmpfile)
	ON_COMMAND(ID_IMAGEPROCESS_SAVETOFILE, &CimageProcessingView::OnImageprocessSavetofile)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYFILEHEADER, &CimageProcessingView::OnImageprocessDisplayfileheader)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYPALETTE, &CimageProcessingView::OnImageprocessDisplaypalette)
	ON_COMMAND(ID_IMAGEPROCESS_GETPIXELVALUE, &CimageProcessingView::OnImageprocessGetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_SETPIXELVALUE, &CimageProcessingView::OnImageprocessSetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_INERPOLATION, &CimageProcessingView::OnImageprocessInerpolation)
	ON_COMMAND(ID_IMAGEPROCESS_GAUSSSMOOTH, &CimageProcessingView::OnImageprocessGausssmooth)
	ON_COMMAND(ID_IMAGEPROCESS_MEDIANFILTER, &CimageProcessingView::OnImageprocessMedianfilter)
	ON_COMMAND(ID_IMAGEPROCESS_BILATERALFILTER, &CimageProcessingView::OnImageprocessBilateralfilter)
	ON_COMMAND(ID_IMAGEPROCESS_HISTOEQUALIZATION, &CimageProcessingView::OnImageprocessHistoequalization)
	ON_COMMAND(ID_IMAGEPROCESS_SHARPENGRAD, &CimageProcessingView::OnImageprocessSharpengrad)
	ON_COMMAND(ID_IMAGEPROCESS_CANNYEDGE, &CimageProcessingView::OnImageprocessCannyedge)
	ON_COMMAND(ID_IMAGEPROCESS_OTSUSEGMENT, &CimageProcessingView::OnImageprocessOtsusegment)
END_MESSAGE_MAP()

CimageProcessingView::CimageProcessingView() noexcept
{
	pFileBuf = NULL;
}

CimageProcessingView::~CimageProcessingView()
{
	if( pFileBuf ) 
	{
		delete [] pFileBuf;
		pFileBuf = 0;
	}
}

BOOL CimageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CimageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CimageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CimageProcessingDoc *CimageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimageProcessingDoc)));
	return (CimageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// CimageProcessingView 绘图
void CimageProcessingView::OnDraw(CDC *pDC)
{
	CimageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if( pFileBuf != NULL )
	{
		DisplayImage(pDC,pFileBuf,10,10,0,0,0);
	}
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//Here are the functions to be programmed by you!

//Open a BMP file
void CimageProcessingView::OnImageprocessOpenbmpfile()
{
	LPCTSTR lpszFilter = _T("BMP Files (*.bmp)|*.bmp||");
	CFileDialog dlg(TRUE,NULL,NULL,OFN_NOCHANGEDIR,lpszFilter,NULL);
	if( dlg.DoModal() != IDOK ) return;
	if( pFileBuf != NULL )
	{
		delete [] pFileBuf;
	}
	pFileBuf = OpenBMPfile( dlg.GetPathName() );
	Invalidate();
	UpdateWindow();
}

//Save to a new BMP file
void CimageProcessingView::OnImageprocessSavetofile()
{
	if(pFileBuf == NULL) return;
	LPCTSTR lpszFilter = _T("Bitmap Files (*.bmp)|*.bmp | All Files (*.*)|*.* ||");
	CFileDialog dlg(FALSE, _T(".bmp"), NULL, OFN_OVERWRITEPROMPT, lpszFilter, NULL); 
	if (dlg.DoModal() != IDOK) return;
	//Add your code to choose the new BMP filename
	CString strBmpFile = dlg.GetPathName();
	SaveDIB(pFileBuf, strBmpFile);
}

//Display BMP file header
void CimageProcessingView::OnImageprocessDisplayfileheader()
{
	if(pFileBuf == NULL) {return;}	
	/**/
	DisplayHeaderMessage(pFileBuf);
}

//Display Pallete
void CimageProcessingView::OnImageprocessDisplaypalette()
{
	if(pFileBuf == NULL) return;
	/**/
	int num = 0;
	RGBQUAD *pallete = GetDIBPaletteData(pFileBuf,&num);
	if( pallete == NULL )
	{
		AfxMessageBox("No palette");
	}
	else
	{
		//Here are your code
		CString paletteInfo;
        for(int i = 0; i < num; ++i)
        {
            CString colorInfo;
            colorInfo.Format("Color %3d - R: %03d, G: %03d, B: %03d\n", i, pallete[i].rgbRed, pallete[i].rgbGreen, pallete[i].rgbBlue);
            paletteInfo += colorInfo;
        }
        AfxMessageBox(paletteInfo);
	}
}

//Get pixel value
void CimageProcessingView::OnImageprocessGetpixelvalue()
{
	if(pFileBuf == NULL) return;
	/**/
	//Add your code to choose the coordinate (x,y)
	GetPDlg dlg;
	int x=0, y=0;
	if (dlg.DoModal() == IDOK) {
		x = dlg.m_nX;
		y = dlg.m_nY;
	}
	RGBQUAD rgb;
	bool bGray;
	GetPixel(pFileBuf,x,y,&rgb,&bGray);
	char buf[100];
	if( bGray )
		sprintf(buf, "(%d,%d) = %d",x,y,rgb.rgbReserved);
	else
		sprintf(buf, "(%d,%d) = (%d,%d,%d)",x,y,rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	AfxMessageBox( buf );
}

//Set pixel value
void CimageProcessingView::OnImageprocessSetpixelvalue()
{
	if(pFileBuf == NULL) return;
	/**/
	//Add your code to choose the coordinate (x,y)
	SetPDlg dlg;
	int x=0 ;
	int y=0 ;
	RGBQUAD rgb;
	if (dlg.DoModal() == IDOK) {
		x = dlg.X;
		y = dlg.Y;
		rgb.rgbReserved = dlg.Res;
		rgb.rgbRed = dlg.R;
		rgb.rgbGreen = dlg.G;
		rgb.rgbBlue = dlg.B;
	}
	SetPixel(pFileBuf,x,y,rgb);
	Invalidate();
	UpdateWindow();
}

//Image interpolaion
void CimageProcessingView::OnImageprocessInerpolation()
{
	if (pFileBuf == NULL) return;

	BITMAPINFOHEADER *pBmpInfo = GetDIBINFO(pFileBuf);
	int currentWidth = pBmpInfo->biWidth;
	int currentHeight = pBmpInfo->biHeight;

	CInterpolationDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		float xScale = dlg.m_xScale;
		float yScale = dlg.m_yScale;
		int interpolationMethod = dlg.m_interpolationMethod;

		int newWidth = static_cast<int>(currentWidth * xScale);
		int newHeight = static_cast<int>(currentHeight * yScale);

		char *pNewImage = ImageInterpolation(pFileBuf, newWidth, newHeight, interpolationMethod);
		// ImageInterpolation() 功能: 图像插值
		// nMethod  插值算法
		// 0 = 最临近插值法
		// 1 = (双)线性插值法

		delete[] pFileBuf;
		pFileBuf = pNewImage;
		Invalidate();
		UpdateWindow();
	}
}

//Gaussian smoothing
void CimageProcessingView::OnImageprocessGausssmooth()
{
    if (pFileBuf == NULL) return;

    CGaussianSmoothDialog dlg;
    if (dlg.DoModal() == IDOK)
    {
        float stdDev = dlg.m_stdDev;

        char *pNewImage = GaussianSmooth(pFileBuf, stdDev);

        delete[] pFileBuf;
        pFileBuf = pNewImage;
        Invalidate();
        UpdateWindow();
    }
}

// Median filtering
void CimageProcessingView::OnImageprocessMedianfilter()
{
	if (pFileBuf == NULL) return;

	CMedianFilterDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		int filterSize = dlg.m_filterSize;

		char* pNewImage = MedianFilter(pFileBuf, filterSize);
		if (pNewImage != nullptr)
		{
			delete[] pFileBuf;
			pFileBuf = pNewImage;
			Invalidate();
			UpdateWindow();
		}
		else
		{
			AfxMessageBox("Failed to apply median filter");
		}
	}
}


// Bilateral filtering
void CimageProcessingView::OnImageprocessBilateralfilter()
{
	if (pFileBuf == NULL) return;

	CBilateralFilterDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		float sigmaD = dlg.m_sigma_d;
		float sigmaR = dlg.m_sigma_R;

		char* pFilteredBuf = BilateralFilter(pFileBuf, sigmaD, sigmaR);
		if (pFilteredBuf)
		{
			delete[] pFileBuf;
			pFileBuf = pFilteredBuf;
			Invalidate();
			UpdateWindow();
		}
		else
		{
			AfxMessageBox("Failed to apply bilateral filter");
		}
	}
}

//Histogram equalization
void CimageProcessingView::OnImageprocessHistoequalization()
{
	if (pFileBuf == NULL) return;

	BITMAPINFOHEADER *pBmpInfo = GetDIBINFO(pFileBuf);
	int width = pBmpInfo->biWidth;
	int height = pBmpInfo->biHeight;
	int colorBits = pBmpInfo->biBitCount;

	if (colorBits != 8) {
		AfxMessageBox("Only 8-bit grayscale images are supported for histogram equalization");
		return;
	}

	char *pImageData = GetDIBImageData(pFileBuf);

	// Calculate the histogram
	int histogram[256] = { 0 };
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int pixelValue = pImageData[y * GetWidthBytes(pFileBuf) + x] & 0xFF;
			histogram[pixelValue]++;
		}
	}

	// Calculate the cumulative histogram
	int cumulativeHistogram[256] = { 0 };
	cumulativeHistogram[0] = histogram[0];
	for (int i = 1; i < 256; i++) {
		cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
	}

	// Normalize the cumulative histogram
	int totalPixels = width * height;
	BYTE equalizationMap[256];
	for (int i = 0; i < 256; i++) {
		equalizationMap[i] = static_cast<BYTE>((cumulativeHistogram[i] * 255) / totalPixels);
	}

	// Apply the equalization map to the image data
	char *pNewImageData = new char[height * GetWidthBytes(pFileBuf)];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int pixelValue = pImageData[y * GetWidthBytes(pFileBuf) + x] & 0xFF;
			pNewImageData[y * GetWidthBytes(pFileBuf) + x] = equalizationMap[pixelValue];
		}
	}

	// Copy new image data back to original buffer
	memcpy(pImageData, pNewImageData, height * GetWidthBytes(pFileBuf));
	delete[] pNewImageData;

	Invalidate();
	UpdateWindow();
}




//Sharpening by gradient
void CimageProcessingView::OnImageprocessSharpengrad()
{
	if (pFileBuf == NULL) return;

	CSharpenDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		float sharpenAmount = dlg.m_sharpenFactor;

		char* pSharpenedImage = SharpeningByGradient(pFileBuf, sharpenAmount);

		delete[] pFileBuf;
		pFileBuf = pSharpenedImage;
		Invalidate();
		UpdateWindow();
	}
}


//Cany edge detection
void CimageProcessingView::OnImageprocessCannyedge()
{
	if (pFileBuf == NULL) return;

	CCannyEdgeDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		int lowThreshold = dlg.m_lowThreshold;
		int highThreshold = dlg.m_highThreshold;
		int kernelSize = dlg.m_kernelSize;

		char* pNewImage = CannyEdgeDetection(pFileBuf, lowThreshold, highThreshold, kernelSize);

		delete[] pFileBuf;
		pFileBuf = pNewImage;
		Invalidate();
		UpdateWindow();
	}
}


//Otsu segmentation
void CimageProcessingView::OnImageprocessOtsusegment()
{
    if (pFileBuf == NULL) return;

    BITMAPINFOHEADER *pBmpInfo = GetDIBINFO(pFileBuf);
    int width = pBmpInfo->biWidth;
    int height = pBmpInfo->biHeight;

    if (pBmpInfo->biBitCount != 8) {
        AfxMessageBox("Only 8-bit grayscale images are supported for Otsu segmentation.");
        return;
    }

    unsigned char* pImageData = (unsigned char*)GetDIBImageData(pFileBuf);
    int threshold = OtsuThreshold(pImageData, width, height);

    CString message;
    message.Format("Otsu threshold: %d", threshold);
    AfxMessageBox(message);

    ApplyThreshold(pImageData, width, height, threshold);

    Invalidate();
    UpdateWindow();
}
