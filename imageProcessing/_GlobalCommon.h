//GlobalCommon.h
#include "afxwin.h"

char    *OpenBMPfile(CString strBmpFile);

BITMAPFILEHEADER *GetDIBHEADER(char *pFileBuffer);
BITMAPINFOHEADER *GetDIBINFO(char *pFileBuffer);
char    *GetDIBImageData(char *pFileBuffer);
RGBQUAD *GetDIBPaletteData(char *pFileBuffer,int nEntryNumber[1]);
int     GetImageWidth(char *pFileBuffer);
int     GetImageHeight(char *pFileBuffer);
int     GetColorBits(char *pFileBuffer);
long    GetUsedColors(char *pFileBuffer);
long    GetWidthBytes(char *pFileBuffer);

void    DisplayHeaderMessage(char *pBmpFileBuf);
void	DisplayImage(CDC *pDC,char *pBmpFileBuf,
		int disp_xL=0,int disp_yL=0,int disp_Width=0,int disp_Height=0,int mode=0 );

long    GetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb[1],bool bGray[1]=NULL);
void    SetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb);

BOOL    SaveDIB(char *pFileBuffer,CString strBmpFile);

char    *ImageInterpolation(char *pBmpFileBuf,int newWidth,int newHeight,int nMethod=0);
char *GaussianSmooth(char *pBmpFileBuf, float stdDev);
char* MedianFilter(char* pBmpFileBuf, int filterSize);
char* BilateralFilter(char* pBmpFileBuf, float sigma_d, float sigma_r);
char *SharpeningByGradient(char *pBmpFileBuf, float sharpenFactor);
void GaussianBlur(const unsigned char* src, unsigned char* dst, int width, int height, int kernelSize, double sigma);
void SobelFilter(const unsigned char* src, unsigned char* dst, int width, int height, double* gradientDirection);
void NonMaximumSuppression(const unsigned char* src, unsigned char* dst, int width, int height, const double* gradientDirection);
void DoubleThreshold(const unsigned char* src, unsigned char* dst, int width, int height, int lowThreshold, int highThreshold);
void EdgeTrackingByHysteresis(unsigned char* src, int width, int height);
char* CannyEdgeDetection(char* pBmpFileBuf, int lowThreshold, int highThreshold, int kernelSize);
int OtsuThreshold(unsigned char* data, int width, int height);
void ApplyThreshold(unsigned char* data, int width, int height, int threshold);