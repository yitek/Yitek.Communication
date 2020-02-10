#pragma once
#ifndef __CSCREEN_H__
#define __CSCREEN_H__

#include <windows.h>

class CScreen
{
public:
	char* buf;    //�ɹ����紫���λͼ����ָ��
	int buf_len; //�ɹ����紫���λͼ���ݳ���

	CScreen(int color_deep = 16, HWND hWnd = 0, int w = 0, int h = 0)
	{
		if (hWnd != 0) {
			m_hWnd = hWnd;
			hhh = ::GetDC(m_hWnd);      //����һָ�����ڵĿͻ������������Ļ����ʾ�豸�����Ļ����ľ��
									  //�Ժ������GDI������ʹ�øþ�������豸�����Ļ����л�ͼ
		}

		hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);  //ʹ��ָ��������Ϊһ���豸�����豸�����Ļ���		
		ScrWidth = GetDeviceCaps(hdc, HORZRES);    //��Ļ���
		ScrHeight = GetDeviceCaps(hdc, VERTRES);   //��Ļ�߶�	
		
		hmdc = CreateCompatibleDC(hdc); //����һ����ָ���豸���ݵ��ڴ��豸�����Ļ���		
		if (w == 0 && h == 0) {
			hBmpScreen = CreateCompatibleBitmap(hdc, ScrWidth, ScrHeight); //������ָ�����豸������ص��豸���ݵ�λͼ
		}
		else {
			hBmpScreen = CreateCompatibleBitmap(hdc, w, h);
			ScrWidth = w;
			ScrHeight = h;
		}
		holdbmp = (HBITMAP)SelectObject(hmdc, hBmpScreen); //ѡ��һ����ָ�����豸�����Ļ�����

		BITMAP            bm;    //bmpͼ��Ч���ã���ѹ����jpgѹ�����������紫��
		BITMAPINFOHEADER    bi;  //λͼ��Ϣͷ����С�̶�40�ֽ�		
		GetObject(hBmpScreen, sizeof(bm), (LPSTR)&bm); //�õ�ָ��ͼ�ζ������Ϣ		
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
		//	GetScreen(100, 200, w, h); //��ʼ����ɫ������
	}

	~CScreen();
	void GetScreen();
	void GetScreen(int x, int y, int w, int h);
	void SetScreentoPath(char* path);
	void SetScreen();
	void SetScreen(int x, int y, int w, int h);
	void JoinScrData(char* data); //����λͼ����
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
//��������������������������������
//��Ȩ����������ΪCSDN������dazhong159����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https ://blog.csdn.net/dazhong159/article/details/7909964