#pragma once
#ifndef __CSCREEN_H__
#define __CSCREEN_H__

#include <windows.h>

class CScreen
{
public:
	char* buf;    //可供网络传输的位图数据指针
	int buf_len; //可供网络传输的位图数据长度

	CScreen(int color_deep = 16, HWND hWnd = 0, int w = 0, int h = 0)
	{
		if (hWnd != 0) {
			m_hWnd = hWnd;
			hhh = ::GetDC(m_hWnd);      //检索一指定窗口的客户区域或整个屏幕的显示设备上下文环境的句柄
									  //以后可以在GDI函数中使用该句柄来在设备上下文环境中绘图
		}

		hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);  //使用指定的名字为一个设备创建设备上下文环境		
		ScrWidth = GetDeviceCaps(hdc, HORZRES);    //屏幕宽度
		ScrHeight = GetDeviceCaps(hdc, VERTRES);   //屏幕高度	
		
		hmdc = CreateCompatibleDC(hdc); //创建一个与指定设备兼容的内存设备上下文环境		
		if (w == 0 && h == 0) {
			hBmpScreen = CreateCompatibleBitmap(hdc, ScrWidth, ScrHeight); //创建与指定的设备环境相关的设备兼容的位图
		}
		else {
			hBmpScreen = CreateCompatibleBitmap(hdc, w, h);
			ScrWidth = w;
			ScrHeight = h;
		}
		holdbmp = (HBITMAP)SelectObject(hmdc, hBmpScreen); //选择一对象到指定的设备上下文环境中

		BITMAP            bm;    //bmp图像效果好，非压缩；jpg压缩，便于网络传输
		BITMAPINFOHEADER    bi;  //位图信息头，大小固定40字节		
		GetObject(hBmpScreen, sizeof(bm), (LPSTR)&bm); //得到指定图形对象的信息		
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bm.bmWidth;
		bi.biHeight = bm.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = color_deep;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) * bi.biHeight;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;
		int ncolors = (1 << bi.biBitCount);
		if (ncolors > 256)
			ncolors = 0;
		dwLen_1 = bi.biSize + ncolors * sizeof(RGBQUAD);
		dwLen_2 = dwLen_1 + bi.biSizeImage;
		hDib = new char[dwLen_2];
		memcpy(hDib, &bi, sizeof(bi));

		buf = hDib + dwLen_1;
		buf_len = bi.biSizeImage;
		//	GetScreen(100, 200, w, h); //初始化调色板数据
	}

	~CScreen();
	void GetScreen();
	void GetScreen(int x, int y, int w, int h);
	void SetScreentoPath(char* path);
	void SetScreen();
	void SetScreen(int x, int y, int w, int h);
	void JoinScrData(char* data); //加载位图数据
	static double GetDpi(HWND hd);

private:
	int 			ScrWidth;
	int 			ScrHeight;
	int 			dwLen_1;
	int 			dwLen_2;
	char* hDib;
	HDC 			hmdc;
	HDC 			hdc;
	HDC 			hhh;
	HBITMAP 		hBmpScreen;
	HBITMAP 		holdbmp;
	HWND 			m_hWnd;
	
};

#endif
//————————————————
//版权声明：本文为CSDN博主「dazhong159」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/dazhong159/article/details/7909964