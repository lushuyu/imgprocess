//GlobalCommon.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "_GlobalCommon.h"
#include <cmath>
#include <vector>

#define M_PI 3.14159265358979323846
/**
	 功能: 从图像文件中建造DIB类
	 参数: strBmpFile --- 需要打开的BMP文件名
	 返回：文件缓冲区指针 (NULL表示失败)
**/
char *OpenBMPfile(CString strBmpFile)
{
	CFile hFile;
	if( !hFile.Open(strBmpFile,CFile::modeRead|CFile::typeBinary) )
	{
		AfxMessageBox("Failed to open the BMP file");
		return( NULL );
	}
	/**/
//	if(	hFile.Seek(0L,CFile::begin) != 0L )
//	{
//		return( NULL );
//	}
	/**/
	long lFileSize = (long)hFile.Seek(0L, CFile::end);
	char *pFileBuf = new char [lFileSize+1];
	hFile.Seek(0L, CFile::begin);
	hFile.Read(pFileBuf, lFileSize);
	hFile.Close();
	/**/
	BITMAPFILEHEADER *pBmpHead = (BITMAPFILEHEADER *)pFileBuf;
	BITMAPINFOHEADER *pBmpInfo = (BITMAPINFOHEADER *)(pFileBuf + sizeof(BITMAPFILEHEADER));
	/**/
	if(	pBmpHead->bfType   != 0x4D42 ||		//"BM"=0x424D
	    pBmpInfo->biSize   != 0x28   ||		// 位图信息子结构长度(等于40,即0x28)
		pBmpInfo->biPlanes != 0x01 )		// 此域必须等于1
	{
		AfxMessageBox("It isn't a valid BMP file");
		return( NULL );
	}
	/**/
	if( pBmpInfo->biCompression != BI_RGB )
	{
		AfxMessageBox("It is a compressed BMP file");
		return( NULL );
	}
	/**/
	if( pBmpInfo->biBitCount != 8  &&
	    pBmpInfo->biBitCount != 24 )
	{
		AfxMessageBox("Only 8-bit and 24-bit BMP files are supported");
		return( NULL );
	}
	/**/
	return( pFileBuf );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	

BITMAPFILEHEADER *GetDIBHEADER(char *pFileBuffer)
{
	char *p = pFileBuffer + 0;
	return( (BITMAPFILEHEADER *)p );
}

BITMAPINFOHEADER *GetDIBINFO(char *pFileBuffer)
{
	char *p = pFileBuffer + sizeof(BITMAPFILEHEADER);
	return( (BITMAPINFOHEADER *)p );
}

char *GetDIBImageData(char *pFileBuffer)
{
	const BITMAPFILEHEADER *pBmpHead = GetDIBHEADER(pFileBuffer);
	char *p = pFileBuffer + pBmpHead->bfOffBits;
	return( p );
}

//return NULL denoting no palette
RGBQUAD *GetDIBPaletteData(char *pFileBuffer,int nEntryNumber[1])
{
	char *pPaletteData = NULL;
	if( GetColorBits(pFileBuffer) <= 8 )
	{
		nEntryNumber[0] = 0;
		char *pDIBImageData = GetDIBImageData(pFileBuffer);
		pPaletteData = pFileBuffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		int  nNum = (DWORD)(pDIBImageData - pPaletteData) / sizeof(RGBQUAD);
		int  iUsedColors = (int)GetDIBINFO(pFileBuffer)->biClrUsed;
		if( nNum > 0 && (int)iUsedColors > 0 )
			nEntryNumber[0] = min(nNum,(int)iUsedColors);
		else
			pPaletteData = NULL;
	}
	return( (RGBQUAD *)pPaletteData );
}

int GetImageWidth(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biWidth );
}

int GetImageHeight(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biHeight );
}

int GetColorBits(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biBitCount );
}

long GetUsedColors(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( (long)pInfo->biClrUsed );
}

long GetWidthBytes(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	long nBytesPerRow = 4 * ((pInfo->biWidth * pInfo->biBitCount + 31) / 32);
	return( nBytesPerRow );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	

void DisplayHeaderMessage(char *pBmpFileBuf)
{
	BITMAPFILEHEADER *pBmpHead = GetDIBHEADER(pBmpFileBuf);
	BITMAPINFOHEADER *pBmpInfo = GetDIBINFO(pBmpFileBuf);
	/**/
	char msg[4096];
	sprintf(msg,
	"bfType (file type) = %4.4X \n"
	"bfSize (file length) = %ld \n"
	"bfOffBits (offset of bitmap data in bytes) = %ld \n"
	"biSize (header structure length should be 40 or 0x28) = %ld \n"
	"biWidth (image width)  = %ld \n"
	"biHeight (image height) = %ld \n"
	"biPlanes (must be eaual to 1) = %u \n"
	"biBitCount (color/pixel bits) = %u \n"
	"biCompression (compressed?) = %ld \n"
	"biSizeImage (length of bitmap data in bytes must be the times of 4) = %ld \n"
	"biXPelsPerMeter (horizontal resolution of target device in pixels/metre) = %ld \n"
	"biYPelsPerMeter (vertical resolution of target device in pixels/metre) = %ld \n"
	"biColorUsed (number of colors used in bitmap,0=2**biBitCount) = %ld \n"
	"biColorImportant (number of important colors,0=all colors are important) = %ld \n\n"
	"The following is additional information: \n"
	"Bytes per row in bitmap (nBytesPerRow) = %ld \n"
	"Total bytes of bitmap (nImageSizeInByte) = %ld \n"
	"Actual pixels per row in bitmap (nPixelsPerRow) = %ld \n"
	"Total rows of bitmap (nTotalRows) = %ld \n"
	"Total colors (2**biBitCount)(nTotalColors) = %ld \n"
	"Used colors (biColorUsed)(nUsedColors) = %ld ",
	pBmpHead->bfType,
	pBmpHead->bfSize,
	pBmpHead->bfOffBits,
	pBmpInfo->biSize,
	pBmpInfo->biWidth,
	pBmpInfo->biHeight,
	pBmpInfo->biPlanes,
	pBmpInfo->biBitCount,
	pBmpInfo->biCompression,
	pBmpInfo->biSizeImage,
	pBmpInfo->biXPelsPerMeter,
	pBmpInfo->biYPelsPerMeter,
	pBmpInfo->biClrUsed,
	pBmpInfo->biClrImportant,
	GetWidthBytes(pBmpFileBuf),
	GetWidthBytes(pBmpFileBuf) * GetImageHeight(pBmpFileBuf),
	GetImageWidth(pBmpFileBuf),
	GetImageHeight(pBmpFileBuf),
	1 << GetColorBits(pBmpFileBuf),
	GetUsedColors(pBmpFileBuf) );
	AfxMessageBox(msg);
}

//Mode = 0, normal display
//   1,2,3, display grayscale image in red, green, blue colors
void DisplayImage(CDC *pDC,char *pBmpFileBuf,int disp_xL,int disp_yL,int disp_Width,int disp_Height,int mode)
{
	ASSERT( pDC != NULL );
	HDC hDC = pDC->GetSafeHdc();
	ASSERT( hDC != 0 );
	/**/
	int imageWidth  = GetImageWidth(pBmpFileBuf);
	int imageHeight = GetImageHeight(pBmpFileBuf);
	if( disp_Width <= 0 || disp_Height <= 0 )
	{
		disp_Width  = imageWidth;
		disp_Height = imageHeight;
	}
	CRect rect;
	CWnd *pWnd = pDC->GetWindow();
	pWnd->GetClientRect(&rect);
	disp_Width = min(disp_Width, rect.right - disp_xL);
	disp_Height = min(disp_Height, rect.bottom - disp_yL);
	/**/
	BITMAPINFOHEADER *pBitmapInfo = GetDIBINFO(pBmpFileBuf);
	char *pDIBImageData = GetDIBImageData(pBmpFileBuf);
	/**/
	char buf[40+256*4];
	BITMAPINFO *pBitsInfo = (BITMAPINFO *)buf;
	memcpy(&pBitsInfo->bmiHeader,pBitmapInfo,sizeof(BITMAPINFOHEADER));
	/**/
	int palleteNum = 0;
	RGBQUAD *pallete = GetDIBPaletteData(pBmpFileBuf,&palleteNum);
	for(int c = 0; c < 256; c++)
	{
		if( mode == 0 )
		{
			(pBitsInfo->bmiColors[c]).rgbRed   = (pallete!=NULL && c<palleteNum? pallete[c].rgbRed   : c);
			(pBitsInfo->bmiColors[c]).rgbGreen = (pallete!=NULL && c<palleteNum? pallete[c].rgbGreen : c);
			(pBitsInfo->bmiColors[c]).rgbBlue  = (pallete!=NULL && c<palleteNum? pallete[c].rgbBlue  : c);
		}
		else
		{
			(pBitsInfo->bmiColors[c]).rgbRed   = (mode==1? c : 0); 
			(pBitsInfo->bmiColors[c]).rgbGreen = (mode==2? c : 0); 
			(pBitsInfo->bmiColors[c]).rgbBlue  = (mode==3? c : 0); 
		}
	}
	/**/
	SetStretchBltMode(hDC,COLORONCOLOR);
	StretchDIBits(hDC,disp_xL,disp_yL,disp_Width,disp_Height,
	0,0,imageWidth,imageHeight,pDIBImageData,pBitsInfo,DIB_RGB_COLORS,SRCCOPY );
	/**/
	return;
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
//   象素操作

//	 读象素颜色值
//	 返回: >=0 表示象素在位图数据中的偏移值
//		    <0 失败或参数无效
long GetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb[1],bool bGray[1])
{
	int  nColorBits   = GetColorBits(pFileBuffer);
	int  nImageHeight = GetImageHeight(pFileBuffer);
	int  nBytesPerRow = GetWidthBytes(pFileBuffer);
	/**/
	long nOffInImage  = (nImageHeight-1-y) * nBytesPerRow;
	char *p = GetDIBImageData(pFileBuffer) + nOffInImage;
	/**/
	if( bGray != NULL ) *bGray = true;
	if( nColorBits == 8 )
	{
		nOffInImage += x;
		rgb[0].rgbReserved = p[x];
		rgb[0].rgbRed      = p[x];
		rgb[0].rgbGreen    = p[x];
		rgb[0].rgbBlue     = p[x];
	}
	else if( nColorBits == 24 )
	{
		if( bGray != NULL ) *bGray = false;
		nOffInImage += 3 * x;
		p += (3 * x);
		rgb[0].rgbReserved = 0;
		rgb[0].rgbRed      = p[2];
		rgb[0].rgbGreen    = p[1];
		rgb[0].rgbBlue     = p[0];
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
		return( -1L );
	}
	/**/
	return( nOffInImage );
}

//  设置像素(x,y)的颜色值
void SetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb)
{
	int  nColorBits   = GetColorBits(pFileBuffer);
	int  nImageHeight = GetImageHeight(pFileBuffer);
	int  nBytesPerRow = GetWidthBytes(pFileBuffer);
	/**/
	long nOffInImage  = (nImageHeight-1-y) * nBytesPerRow;
	char *p = GetDIBImageData(pFileBuffer) + nOffInImage;
	/**/
	if( nColorBits == 8 )
	{
		 p[ x ] = rgb.rgbReserved;
	}
	else if( nColorBits == 24 )
	{
		p += (3 * x);
		p[0] = rgb.rgbBlue;
		p[1] = rgb.rgbGreen;
		p[2] = rgb.rgbRed;
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
	/**/
	return;
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
// 保存为BMP文件

BOOL SaveDIB(char *pFileBuffer,CString strBmpFile)
{
	CFile hFile;
	if( !hFile.Open(strBmpFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		AfxMessageBox("Failed to create the BMP file");
		return( FALSE );
	}
	/**/
	BITMAPFILEHEADER *pBmpHead = (BITMAPFILEHEADER *)pFileBuffer;
	long lFileSize = pBmpHead->bfSize;
	hFile.Write(pFileBuffer,lFileSize);
	hFile.Close();
	return( TRUE );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
// 图像插值

/**
	 功能: 图像插值
		   nMethod  插值算法
					0 = 最临近插值法
					1 = (双)线性插值法
	 返回: 新图像的BMP文件缓冲区首地址
           NULL 表示失败（内存不足）
**/
char *ImageInterpolation(char *pBmpFileBuf,int newWidth,int newHeight,int nMethod)
{
	BITMAPFILEHEADER *pFileHeader = (BITMAPFILEHEADER *)pBmpFileBuf;
	BITMAPINFOHEADER *pDIBInfo = (BITMAPINFOHEADER *)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));
//	char *pDIBData = pBmpFileBuf + pFileHeader->bfOffBits;
	int  orgWidth  = pDIBInfo->biWidth;
	int  orgHeight = pDIBInfo->biHeight;
	int  colorBits = pDIBInfo->biBitCount;
	/**/
	long bytesPerRow = 4 * ((newWidth * colorBits + 31) / 32);
	long newBmpFileSize = pFileHeader->bfOffBits + bytesPerRow * newHeight;
	char *pNewBmpFileBuf = new char [newBmpFileSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);
	BITMAPFILEHEADER *pNewFileHeader = (BITMAPFILEHEADER *)pNewBmpFileBuf;
	BITMAPINFOHEADER *pNewDIBInfo = (BITMAPINFOHEADER *)(pNewBmpFileBuf + sizeof(BITMAPFILEHEADER));
	pNewFileHeader->bfSize = newBmpFileSize;
	pNewDIBInfo->biWidth = newWidth;
	pNewDIBInfo->biHeight = newHeight;
	pNewDIBInfo->biSizeImage = bytesPerRow * newHeight;
//	char *pNewDIBData = pNewBmpFileBuf + pFileHeader->bfOffBits;
	/**/
	/**/
	float xScale  = (float)orgWidth  / (float)newWidth;
	float yScale  = (float)orgHeight / (float)newHeight;
	for(int y = 0; y < newHeight; y++)
	{
		float fy = y * yScale;
		for(int x = 0; x < newWidth; x++)
		{
			RGBQUAD rgb;
			float fx = x * xScale;
			if( nMethod == 0 )		//最临近插值法
			{
				int xx = min( (int)(fx+0.5), orgWidth - 1 );
				int yy = min( (int)(fy+0.5), orgHeight - 1 );
				GetPixel(pBmpFileBuf, xx, yy, &rgb);
			}
			else
			{						//(双)线性插值法
				RGBQUAD rgbLT,rgbRT,rgbLB,rgbRB;
				int   x1 = (int)fx;
				int   x2 = min(x1+1, orgWidth-1);
				float dx = fx - (float)x1;
				int   y1 = (int)fy;
				int   y2 = min(y1+1, orgHeight-1);
				float dy = fy - (float)y1;
				GetPixel(pBmpFileBuf, x1, y1, &rgbLT);
				GetPixel(pBmpFileBuf, x2, y1, &rgbRT);
				GetPixel(pBmpFileBuf, x1, y2, &rgbLB);
				GetPixel(pBmpFileBuf, x2, y2, &rgbRB);
				for(int N = 0; N < 4; N++)
				{
					float v1 = ((BYTE *)&rgbLT)[N] + dy * (((BYTE *)&rgbLB)[N] - ((BYTE *)&rgbLT)[N]);
					float v2 = ((BYTE *)&rgbRT)[N] + dy * (((BYTE *)&rgbRB)[N] - ((BYTE *)&rgbRT)[N]);
					((BYTE *)&rgb)[N] = (int)(v1 + dx * (v2 - v1) + 0.5);
				}
			}
			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}
	/**/
	return( pNewBmpFileBuf );
}

// Gaussian kernel
std::vector<float> GenerateGaussianKernel(float stdDev, int &kernelSize)
{
    int radius = std::ceil(3 * stdDev);
    kernelSize = 2 * radius + 1;
    std::vector<float> kernel(kernelSize * kernelSize);
    float sum = 0.0f;

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            float value = std::exp(-(x * x + y * y) / (2 * stdDev * stdDev)) / (2 * M_PI * stdDev * stdDev);
            kernel[(y + radius) * kernelSize + (x + radius)] = value;
            sum += value;
        }
    }

    for (auto &v : kernel) v /= sum;

    return kernel;
}

// Gaussian smoothing func
char *GaussianSmooth(char *pBmpFileBuf, float stdDev)
{
    BITMAPFILEHEADER *pFileHeader = (BITMAPFILEHEADER *)pBmpFileBuf;
    BITMAPINFOHEADER *pDIBInfo = (BITMAPINFOHEADER *)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));
    int width = pDIBInfo->biWidth;
    int height = pDIBInfo->biHeight;
    int colorBits = pDIBInfo->biBitCount;

    long bytesPerRow = GetWidthBytes(pBmpFileBuf);
    long imageSize = bytesPerRow * height;
    char *pNewBmpFileBuf = new char[pFileHeader->bfSize];
    memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits + imageSize);

    char *pDIBData = GetDIBImageData(pBmpFileBuf);
    char *pNewDIBData = GetDIBImageData(pNewBmpFileBuf);

    int kernelSize;
    std::vector<float> kernel = GenerateGaussianKernel(stdDev, kernelSize);
    int radius = kernelSize / 2;

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sum[3] = {0, 0, 0};
            for (int ky = -radius; ky <= radius; ky++) {
                for (int kx = -radius; kx <= radius; kx++) {
                    int ix = min((x + kx > 0 ? x + kx : 0), width - 1);
                    int iy = min((y + ky > 0 ? y + ky : 0), height - 1);
                    RGBQUAD rgb;
                    GetPixel(pBmpFileBuf, ix, iy, &rgb);
                    float kernelValue = kernel[(ky + radius) * kernelSize + (kx + radius)];
                    sum[0] += kernelValue * rgb.rgbRed;
                    sum[1] += kernelValue * rgb.rgbGreen;
                    sum[2] += kernelValue * rgb.rgbBlue;
                }
            }
            RGBQUAD rgb;
            rgb.rgbRed = static_cast<BYTE>(sum[0]);
            rgb.rgbGreen = static_cast<BYTE>(sum[1]);
            rgb.rgbBlue = static_cast<BYTE>(sum[2]);
            SetPixel(pNewBmpFileBuf, x, y, rgb);
        }
    }

    return pNewBmpFileBuf;
}

#include <algorithm>

// Median Filter Algo
char* MedianFilter(char* pBmpFileBuf, int filterSize)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));
	int orgWidth = pDIBInfo->biWidth;
	int orgHeight = pDIBInfo->biHeight;
	int colorBits = pDIBInfo->biBitCount;

	if (colorBits != 24) {
		AfxMessageBox("Only 24-bit BMP files are supported for median filtering");
		return nullptr;
	}

	long bytesPerRow = GetWidthBytes(pBmpFileBuf);
	long newBmpFileSize = pFileHeader->bfSize;
	char* pNewBmpFileBuf = new char[newBmpFileSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);
	BITMAPFILEHEADER* pNewFileHeader = (BITMAPFILEHEADER*)pNewBmpFileBuf;
	BITMAPINFOHEADER* pNewDIBInfo = (BITMAPINFOHEADER*)(pNewBmpFileBuf + sizeof(BITMAPFILEHEADER));
	char* pNewDIBData = pNewBmpFileBuf + pNewFileHeader->bfOffBits;

	int radius = filterSize / 2;
	std::vector<BYTE> rValues, gValues, bValues;

	for (int y = 0; y < orgHeight; ++y) {
		for (int x = 0; x < orgWidth; ++x) {
			rValues.clear();
			gValues.clear();
			bValues.clear();

			for (int dy = -radius; dy <= radius; ++dy) {
				for (int dx = -radius; dx <= radius; ++dx) {
					int nx = std::clamp(x + dx, 0, orgWidth - 1);
					int ny = std::clamp(y + dy, 0, orgHeight - 1);

					RGBQUAD pixel;
					GetPixel(pBmpFileBuf, nx, ny, &pixel);

					rValues.push_back(pixel.rgbRed);
					gValues.push_back(pixel.rgbGreen);
					bValues.push_back(pixel.rgbBlue);
				}
			}

			std::nth_element(rValues.begin(), rValues.begin() + rValues.size() / 2, rValues.end());
			std::nth_element(gValues.begin(), gValues.begin() + gValues.size() / 2, gValues.end());
			std::nth_element(bValues.begin(), bValues.begin() + bValues.size() / 2, bValues.end());

			RGBQUAD medianPixel;
			medianPixel.rgbRed = rValues[rValues.size() / 2];
			medianPixel.rgbGreen = gValues[gValues.size() / 2];
			medianPixel.rgbBlue = bValues[bValues.size() / 2];

			SetPixel(pNewBmpFileBuf, x, y, medianPixel);
		}
	}

	return pNewBmpFileBuf;
}

#include <cmath>
#include <vector>

char* BilateralFilter(char* pBmpFileBuf, float sigma_d, float sigma_r)
{
    BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
    BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));
    int width = pDIBInfo->biWidth;
    int height = pDIBInfo->biHeight;
    int colorBits = pDIBInfo->biBitCount;

    long bytesPerRow = GetWidthBytes(pBmpFileBuf);
    long bmpFileSize = pFileHeader->bfSize;
    char* pFilteredBuf = new char[bmpFileSize];
    memcpy(pFilteredBuf, pBmpFileBuf, pFileHeader->bfOffBits);

    const int radius = static_cast<int>(2 * sigma_d);
    const int windowSize = 2 * radius + 1;

    // 高斯函数
    auto gaussian = [](float x, float sigma) {
        return expf(-(x * x) / (2 * sigma * sigma));
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
            float normFactorR = 0.0f, normFactorG = 0.0f, normFactorB = 0.0f;
            RGBQUAD rgbCenter;
            GetPixel(pBmpFileBuf, x, y, &rgbCenter);

            for (int dy = -radius; dy <= radius; dy++) {
                for (int dx = -radius; dx <= radius; dx++) {
                    int nx = min(max(x + dx, 0), width - 1);
                    int ny = min(max(y + dy, 0), height - 1);

                    RGBQUAD rgbNeighbor;
                    GetPixel(pBmpFileBuf, nx, ny, &rgbNeighbor);

                    float spatialWeight = gaussian(sqrtf(dx * dx + dy * dy), sigma_d);
                    float rangeWeightR = gaussian(rgbCenter.rgbRed - rgbNeighbor.rgbRed, sigma_r);
                    float rangeWeightG = gaussian(rgbCenter.rgbGreen - rgbNeighbor.rgbGreen, sigma_r);
                    float rangeWeightB = gaussian(rgbCenter.rgbBlue - rgbNeighbor.rgbBlue, sigma_r);

                    sumR += spatialWeight * rangeWeightR * rgbNeighbor.rgbRed;
                    sumG += spatialWeight * rangeWeightG * rgbNeighbor.rgbGreen;
                    sumB += spatialWeight * rangeWeightB * rgbNeighbor.rgbBlue;

                    normFactorR += spatialWeight * rangeWeightR;
                    normFactorG += spatialWeight * rangeWeightG;
                    normFactorB += spatialWeight * rangeWeightB;
                }
            }

            RGBQUAD rgbFiltered;
            rgbFiltered.rgbRed = static_cast<BYTE>(sumR / normFactorR);
            rgbFiltered.rgbGreen = static_cast<BYTE>(sumG / normFactorG);
            rgbFiltered.rgbBlue = static_cast<BYTE>(sumB / normFactorB);

            SetPixel(pFilteredBuf, x, y, rgbFiltered);
        }
    }

    return pFilteredBuf;
}





char* SharpeningByGradient(char* pBmpFileBuf, float sharpenAmount)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));
	int width = pDIBInfo->biWidth;
	int height = pDIBInfo->biHeight;
	int colorBits = pDIBInfo->biBitCount;

	long bytesPerRow = GetWidthBytes(pBmpFileBuf);
	char* pSharpenedBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pSharpenedBmpFileBuf, pBmpFileBuf, pFileHeader->bfSize);

	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			RGBQUAD rgbCenter, rgbLeft, rgbRight, rgbUp, rgbDown;
			GetPixel(pBmpFileBuf, x, y, &rgbCenter);
			GetPixel(pBmpFileBuf, x - 1, y, &rgbLeft);
			GetPixel(pBmpFileBuf, x + 1, y, &rgbRight);
			GetPixel(pBmpFileBuf, x, y - 1, &rgbUp);
			GetPixel(pBmpFileBuf, x, y + 1, &rgbDown);

			RGBQUAD rgbNew;
			rgbNew.rgbRed = min(max(rgbCenter.rgbRed + sharpenAmount * (4 * rgbCenter.rgbRed - rgbLeft.rgbRed - rgbRight.rgbRed - rgbUp.rgbRed - rgbDown.rgbRed), 0), 255);
			rgbNew.rgbGreen = min(max(rgbCenter.rgbGreen + sharpenAmount * (4 * rgbCenter.rgbGreen - rgbLeft.rgbGreen - rgbRight.rgbGreen - rgbUp.rgbGreen - rgbDown.rgbGreen), 0), 255);
			rgbNew.rgbBlue = min(max(rgbCenter.rgbBlue + sharpenAmount * (4 * rgbCenter.rgbBlue - rgbLeft.rgbBlue - rgbRight.rgbBlue - rgbUp.rgbBlue - rgbDown.rgbBlue), 0), 255);

			SetPixel(pSharpenedBmpFileBuf, x, y, rgbNew);
		}
	}

	return pSharpenedBmpFileBuf;
}


void GaussianBlur(const unsigned char* src, unsigned char* dst, int width, int height, int kernelSize, double sigma)
{
	int halfSize = kernelSize / 2;
	double* kernel = new double[kernelSize * kernelSize];
	double sum = 0.0;

	for (int y = -halfSize; y <= halfSize; ++y)
	{
		for (int x = -halfSize; x <= halfSize; ++x)
		{
			double value = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
			kernel[(y + halfSize) * kernelSize + (x + halfSize)] = value;
			sum += value;
		}
	}


	for (int i = 0; i < kernelSize * kernelSize; ++i)
	{
		kernel[i] /= sum;
	}

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			double value = 0.0;
			for (int ky = -halfSize; ky <= halfSize; ++ky)
			{
				for (int kx = -halfSize; kx <= halfSize; ++kx)
				{
					int px = min(max(x + kx, 0), width - 1);
					int py = min(max(y + ky, 0), height - 1);
					value += src[py * width + px] * kernel[(ky + halfSize) * kernelSize + (kx + halfSize)];
				}
			}
			dst[y * width + x] = static_cast<unsigned char>(value);
		}
	}

	delete[] kernel;
}

void SobelFilter(const unsigned char* src, unsigned char* dst, int width, int height, double* gradientDirection)
{
	const int kernelX[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	const int kernelY[3][3] = {
		{-1, -2, -1},
		{ 0,  0,  0},
		{ 1,  2,  1}
	};

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			double gradientX = 0.0;
			double gradientY = 0.0;

			for (int ky = -1; ky <= 1; ++ky)
			{
				for (int kx = -1; kx <= 1; ++kx)
				{
					int px = min(max(x + kx, 0), width - 1);
					int py = min(max(y + ky, 0), height - 1);
					gradientX += src[py * width + px] * kernelX[ky + 1][kx + 1];
					gradientY += src[py * width + px] * kernelY[ky + 1][kx + 1];
				}
			}

			dst[y * width + x] = static_cast<unsigned char>(std::sqrt(gradientX * gradientX + gradientY * gradientY));
			gradientDirection[y * width + x] = std::atan2(gradientY, gradientX);
		}
	}
}

void NonMaximumSuppression(const unsigned char* src, unsigned char* dst, int width, int height, const double* gradientDirection)
{
	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			double angle = gradientDirection[y * width + x] * 180.0 / M_PI;
			angle = angle < 0 ? angle + 180 : angle;

			unsigned char currentPixel = src[y * width + x];
			unsigned char neighbor1 = 0;
			unsigned char neighbor2 = 0;

			if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <= 180))
			{
				neighbor1 = src[y * width + (x - 1)];
				neighbor2 = src[y * width + (x + 1)];
			}
			else if (angle >= 22.5 && angle < 67.5)
			{
				neighbor1 = src[(y - 1) * width + (x + 1)];
				neighbor2 = src[(y + 1) * width + (x - 1)];
			}
			else if (angle >= 67.5 && angle < 112.5)
			{
				neighbor1 = src[(y - 1) * width + x];
				neighbor2 = src[(y + 1) * width + x];
			}
			else if (angle >= 112.5 && angle < 157.5)
			{
				neighbor1 = src[(y - 1) * width + (x - 1)];
				neighbor2 = src[(y + 1) * width + (x + 1)];
			}

			if (currentPixel >= neighbor1 && currentPixel >= neighbor2)
			{
				dst[y * width + x] = currentPixel;
			}
			else
			{
				dst[y * width + x] = 0;
			}
		}
	}
}

void DoubleThreshold(const unsigned char* src, unsigned char* dst, int width, int height, int lowThreshold, int highThreshold)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			unsigned char pixel = src[y * width + x];

			if (pixel >= highThreshold)
			{
				dst[y * width + x] = 255;
			}
			else if (pixel >= lowThreshold)
			{
				dst[y * width + x] = 128;
			}
			else
			{
				dst[y * width + x] = 0;
			}
		}
	}
}

void EdgeTrackingByHysteresis(unsigned char* src, int width, int height)
{
	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			if (src[y * width + x] == 128)
			{
				if (src[(y - 1) * width + (x - 1)] == 255 || src[(y - 1) * width + x] == 255 ||
					src[(y - 1) * width + (x + 1)] == 255 || src[y * width + (x - 1)] == 255 ||
					src[y * width + (x + 1)] == 255 || src[(y + 1) * width + (x - 1)] == 255 ||
					src[(y + 1) * width + x] == 255 || src[(y + 1) * width + (x + 1)] == 255)
				{
					src[y * width + x] = 255;
				}
				else
				{
					src[y * width + x] = 0;
				}
			}
		}
	}
}

char* CannyEdgeDetection(char* pBmpFileBuf, int lowThreshold, int highThreshold, int kernelSize)
{
	BITMAPFILEHEADER* pFileHeader = GetDIBHEADER(pBmpFileBuf);
	BITMAPINFOHEADER* pDIBInfo = GetDIBINFO(pBmpFileBuf);

	int width = pDIBInfo->biWidth;
	int height = pDIBInfo->biHeight;
	int colorBits = pDIBInfo->biBitCount;
	int bytesPerPixel = colorBits / 8;

	char* pDIBData = GetDIBImageData(pBmpFileBuf);

	unsigned char* grayImage = new unsigned char[width * height];
	unsigned char* blurredImage = new unsigned char[width * height];
	unsigned char* gradientImage = new unsigned char[width * height];
	double* gradientDirection = new double[width * height];
	unsigned char* nonMaxSuppImage = new unsigned char[width * height];
	unsigned char* thresholdImage = new unsigned char[width * height];

	// Convert to grayscale
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			RGBQUAD rgb;
			GetPixel(pBmpFileBuf, x, y, &rgb);
			unsigned char gray = static_cast<unsigned char>(0.299 * rgb.rgbRed + 0.587 * rgb.rgbGreen + 0.114 * rgb.rgbBlue);
			grayImage[y * width + x] = gray;
		}
	}

	GaussianBlur(grayImage, blurredImage, width, height, kernelSize, 1.0);

	SobelFilter(blurredImage, gradientImage, width, height, gradientDirection);

	NonMaximumSuppression(gradientImage, nonMaxSuppImage, width, height, gradientDirection);

	DoubleThreshold(nonMaxSuppImage, thresholdImage, width, height, lowThreshold, highThreshold);

	EdgeTrackingByHysteresis(thresholdImage, width, height);

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);
	unsigned char* pNewDIBData = (unsigned char*)(pNewBmpFileBuf + pFileHeader->bfOffBits);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			unsigned char value = thresholdImage[(height - 1 - y) * width + x];
			for (int c = 0; c < bytesPerPixel; ++c)
			{
				pNewDIBData[y * width * bytesPerPixel + x * bytesPerPixel + c] = value;
			}
		}
	}

	delete[] grayImage;
	delete[] blurredImage;
	delete[] gradientImage;
	delete[] gradientDirection;
	delete[] nonMaxSuppImage;
	delete[] thresholdImage;

	return pNewBmpFileBuf;
}

// Otsu's thresholding method
int OtsuThreshold(unsigned char* data, int width, int height)
{
    int hist[256] = {0};
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int val = data[y * width + x];
            hist[val]++;
        }
    }

    int total = width * height;
    float sum = 0;
    for (int i = 0; i < 256; ++i)
        sum += i * hist[i];

    float sumB = 0;
    int wB = 0;
    int wF = 0;
    float varMax = 0;
    int threshold = 0;

    for (int i = 0; i < 256; ++i) {
        wB += hist[i];
        if (wB == 0) continue;
        wF = total - wB;
        if (wF == 0) break;

        sumB += i * hist[i];
        float mB = sumB / wB;
        float mF = (sum - sumB) / wF;

        float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);
        if (varBetween > varMax) {
            varMax = varBetween;
            threshold = i;
        }
    }

    return threshold;
}

void ApplyThreshold(unsigned char* data, int width, int height, int threshold)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            data[index] = (data[index] > threshold) ? 255 : 0;
        }
    }
}
