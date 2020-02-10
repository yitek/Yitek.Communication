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
	//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת��
	BitBlt(hmdc, 0, 0, ScrWidth, ScrHeight, hdc, 0, 0, SRCCOPY);

	//������ȡָ������λͼ��λ��Ȼ������һ��DIB���豸�޹�λͼ��Device-Independent Bitmap��ʹ�õ�ָ����ʽ���Ƶ�һ����������
	GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)ScrHeight, (LPBYTE)hDib + dwLen_1, (LPBITMAPINFO)hDib, (DWORD)DIB_RGB_COLORS);
}

void CScreen::GetScreen(int x, int y, int w, int h)
{
	//��ָ����Դ�豸������hdc���е����ؽ���λ��ת�����Դ�����Ŀ���豸������hmdc����
	BitBlt(hmdc, 0, 0, w, h, hdc, x, y, SRCCOPY);

	//������ȡָ������λͼ��λ��Ȼ������һ��DIB���豸�޹�λͼʹ�õ�ָ����ʽ���Ƶ�һ����������
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
	//ʹ��DIBλͼ����ɫ���ݶ���Ŀ���豸������ص��豸�ϵ�ָ�������е����ؽ�������
	::SetDIBitsToDevice(hhh, 0, 0, ScrWidth, ScrHeight, 0, 0, 0, ScrHeight, (LPBYTE)hDib + dwLen_1, (LPBITMAPINFO)hDib, (DWORD)DIB_RGB_COLORS);
}
void CScreen::SetScreen(int x, int y, int w, int h)
{
	//ʹ��ָ����DIBλͼ�з��ֵ���ɫ����������λͼ�е�����
	::SetDIBits(hhh, hBmpScreen, ScrWidth, ScrHeight, (LPBYTE)hDib + dwLen_1, (LPBITMAPINFO)hDib, (DWORD)DIB_RGB_COLORS);
	//��Դ�����и���һ��λͼ��Ŀ�����
	StretchBlt(hhh, x, y, w, h, hdc, 0, 0, ScrWidth, ScrHeight, SRCCOPY);
}
void CScreen::JoinScrData(char* data) //����λͼ����
{
	memcpy(hDib + dwLen_1, data, dwLen_2 - dwLen_1);
}

//��������������������������������
//��Ȩ����������ΪCSDN������dazhong159����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https ://blog.csdn.net/dazhong159/article/details/7909964