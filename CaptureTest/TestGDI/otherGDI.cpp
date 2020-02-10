// TTTest.cpp : 定义应用程序的入口点。
//

#include "windows.h"
#include <stdio.h>

LPVOID    screenCaptureData = NULL;
HBITMAP hBitmap;
HDC hDDC;
HDC hCDC;
int nWidth;
int nHeight;
void initGDI() {
    // Get screen size
    nWidth = GetSystemMetrics(SM_CXSCREEN);
    nHeight = GetSystemMetrics(SM_CYSCREEN);

    screenCaptureData = new char[nWidth * nHeight * 4];
    memset(screenCaptureData, 0, nWidth);

    // Get desktop DC, create a compatible dc, create a comaptible bitmap and select into compatible dc.
    hDDC = GetDC(GetDesktopWindow());
    hCDC = CreateCompatibleDC(hDDC);
    hBitmap = CreateCompatibleBitmap(hDDC, nWidth, nHeight);
    SelectObject(hCDC, hBitmap);
}
void gdiScreenCapture(LPVOID screenCaptureData) {


    BitBlt(hCDC, 0, 0, nWidth, nHeight, hDDC, 0, 0, SRCCOPY);

    GetBitmapBits(hBitmap, nWidth * nHeight * 4, screenCaptureData);

}

int APIENTRY _tWinMainx()
{
    initGDI();
    gdiScreenCapture(screenCaptureData);

    for (int i = 0; i < nWidth * nHeight; i++) {
        unsigned char b = *((char*)screenCaptureData + i * 4);
        unsigned char g = *((char*)screenCaptureData + i * 4 + 1);
        unsigned char r = *((char*)screenCaptureData + i * 4 + 2);
        unsigned char a = *((char*)screenCaptureData + i * 4 + 3);

        char chInput[10];
        sprintf(chInput, "x:%d y:%d A:%d R:%d G:%d B:%d\n", i % nWidth, i / nWidth, a, r, g, b);
        //OutputDebugString(chInput);
    }
    return 0;
}