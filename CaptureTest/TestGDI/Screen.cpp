#include "Screen.h"

CScreen::~CScreen()
{
	hBmpScreen = (HBITMAP)SelectObject(hmdc, holdbmp);
	delete[dwLen_2]hDib;
	DeleteDC(hdc);
	DeleteDC(hmdc);
	if (m_hWnd != 0) {
		DeleteDC(hhh);
	}
	DeleteObject(hBmpScreen);
	DeleteObject(holdbmp);
}
double CScreen::GetDpi(HWND hd=NULL) {
	if(hd==NULL) hd = GetDesktopWindow();
	int zoom = GetDpiForWindow(hd);	
	double dpi = 0;	
	switch (zoom) { 
		case 96:		
			dpi = 1;		
			break;	
		case 120:		
			dpi = 1.25;		
			break;	
		case 144:		
			dpi = 1.5;
			break;	
		case 192:		
			dpi = 2;	
			break;	
		default:
			dpi = 0;
			break; 
	}
	return dpi;
}

void CScreen::GetScreen()
{
	//对指定的源设备环境区域中的像素进行位块（bit_block）转换
	BitBlt(hmdc, 0, 0, ScrWidth, ScrHeight, hdc, 0, 0, SRCCOPY);

	//函数获取指定兼容位图的位，然后将其作一个DIB—设备无关位图（Device-Independent Bitmap）使用的指定格式复制到一个缓冲区中
	GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)ScrHeight, (LPBYTE)hDib + dwLen_1, (LPBITMAPINFO)hDib, (DWORD)DIB_RGB_COLORS);
}

void CScreen::GetScreen(int x, int y, int w, int h)
{
	//对指定的源设备环境（hdc）中的像素进行位块转换，以传送至目标设备环境（hmdc）中
	BitBlt(hmdc, 0, 0, w, h, hdc, x, y, SRCCOPY);

	//函数获取指定兼容位图的位，然后将其作一个DIB—设备无关位图使用的指定格式复制到一个缓冲区中
	GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)ScrHeight, (LPBYTE)hDib + dwLen_1, (LPBITMAPINFO)hDib, (DWORD)DIB_RGB_COLORS);
}

void CScreen::SetScreentoPath(char* path)
{
	BITMAPFILEHEADER bfh;
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfSize = 54 + dwLen_2;
	bfh.bfOffBits = 54;
	HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dwWrite;
	WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
	WriteFile(hFile, hDib, bfh.bfSize, &dwWrite, NULL);
	CloseHandle(hFile);
}
void CScreen::SetScreen()
{
	//使用DIB位图和颜色数据对与目标设备环境相关的设备上的指定矩形中的像素进行设置
	::SetDIBitsToDevice(hhh, 0, 0, ScrWidth, ScrHeight, 0, 0, 0, ScrHeight, (LPBYTE)hDib + dwLen_1, (LPBITMAPINFO)hDib, (DWORD)DIB_RGB_COLORS);
}
void CScreen::SetScreen(int x, int y, int w, int h)
{
	//使用指定的DIB位图中发现的颜色数据来设置位图中的像素
	::SetDIBits(hhh, hBmpScreen, ScrWidth, ScrHeight, (LPBYTE)hDib + dwLen_1, (LPBITMAPINFO)hDib, (DWORD)DIB_RGB_COLORS);
	//从源矩形中复制一个位图到目标矩形
	StretchBlt(hhh, x, y, w, h, hdc, 0, 0, ScrWidth, ScrHeight, SRCCOPY);
}
void CScreen::JoinScrData(char* data) //加载位图数据
{
	memcpy(hDib + dwLen_1, data, dwLen_2 - dwLen_1);
}

//————————————————
//版权声明：本文为CSDN博主「dazhong159」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/dazhong159/article/details/7909964