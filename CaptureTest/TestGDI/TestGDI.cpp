// TestGDI.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include<Gdiplus.h>
#include "Screen.h"
using   namespace   Gdiplus;



void CaptureScreen()
{
    //获取屏幕大小
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    //得到桌面窗口句柄
    HWND hDesktopWnd = GetDesktopWindow();
    //得到桌面设备句柄
    HDC hDesktopDC = GetDC(hDesktopWnd);
    //创建一个与桌面设备兼容的设备句柄
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    //创建一个与桌面设备兼容的位图
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
    //把位图选择进设备
    SelectObject(hCaptureDC, hCaptureBitmap);
    //拷贝屏幕到位图中
    BitBlt(hCaptureDC, 0, 0, nScreenWidth, nScreenHeight, hDesktopDC, 0, 0, SRCCOPY);
    //SaveCapturedBitmap(hCaptureBitmap); //Place holder - Put your code
                                //here to save the captured image to disk
    //释放创建的资源
    ReleaseDC(hDesktopWnd, hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}



void ShootScreen(const char* filename, HWND hWnd)
{
    HDC hdc;
    int scrWidth = 0, scrHeight = 0;
    RECT rect = { 0 };
    HWND hDesktopWnd = NULL;
    if (hWnd == NULL)
    {
        //获取屏幕大小
        scrWidth = GetSystemMetrics(SM_CXSCREEN);
        scrHeight = GetSystemMetrics(SM_CYSCREEN);
        printf("screen width:%d\n", scrWidth);
        printf("screen height:%d\n", scrHeight);
        //得到桌面窗口句柄
        hDesktopWnd = GetDesktopWindow();
        //得到桌面设备句柄
        hdc = GetDC(hDesktopWnd);
        
    }
    else
    {
        
        //hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        GetWindowRect(hWnd, &rect);
        scrWidth = rect.right - rect.left;
        scrHeight = rect.bottom - rect.top;
        hdc = GetDC(hWnd);
    }
    HDC hCaptureDC = CreateCompatibleDC(hdc);

    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hdc, scrWidth, scrHeight);
    HBITMAP hHoldBitmap = (HBITMAP)SelectObject(hCaptureDC, hCaptureBitmap);

   
    //拷贝窗口设备的Bit到内存设备
    
    BitBlt(hCaptureDC, 0, 0, scrWidth, scrHeight, hdc, rect.left, rect.top, SRCCOPY);

    //可以通过下面这一行直接获取到位图数据
    //GetBitmapBits(hBitmap,nWidth*nHeight*4,screenCaptureData);

    //根据位图句柄获取位图对象
    BITMAP bm;
    GetObject(hCaptureBitmap, sizeof(bm), &bm);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = bm.bmPlanes;
    bi.biBitCount = bm.bmBitsPixel;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = bm.bmHeight * bm.bmWidthBytes;
    char* buf = new char[bi.biSizeImage];
    GetDIBits(hCaptureDC, hCaptureBitmap, 0L, (DWORD)scrHeight, buf, (LPBITMAPINFO)&bi, (DWORD)DIB_RGB_COLORS);

    BITMAPFILEHEADER bfh = { 0 };
    bfh.bfType = ((WORD)('M' << 8) | 'B');
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD dwWrite;
    WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
    WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
    WriteFile(hFile, buf, bi.biSizeImage, &dwWrite, NULL);
    CloseHandle(hFile);
    //释放创建的资源
    if(hDesktopWnd!=NULL)
        ReleaseDC(hDesktopWnd, hdc);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}


void ShootMain() {
    char name[256] = { 0 };
    for (int32_t i = 0; i < 15; ++i)
    {
        sprintf_s(name, 256, "%d.bmp", i);
        printf("shooting %s\n", name);
        ShootScreen(name, NULL);
        Sleep(1000);
    }
}

void ScreenMain() {
    //https://blog.csdn.net/dazhong159/article/details/7903382
    //上面那个文章说明如何获取句柄
    RECT rect;
    HWND m_hWnd = GetDesktopWindow();
    ::GetWindowRect(m_hWnd, &rect);
    
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    //double dpi = CScreen::GetDpi(m_hWnd);
    //if (dpi != NULL) {
    //    w *= dpi; h *= dpi;
    //}
    CScreen* screen = new CScreen(16, NULL, w,h);
    //CScreen* screen = new CScreen(16, NULL, 0, 0);
    

    char fname[256] = { 0 };
    for (int32_t i = 0; i < 15; ++i)
    {
        sprintf_s(fname, 256, "%d.bmp", i);
        printf("shooting %s\n", fname);
        screen->GetScreen(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
        screen->SetScreentoPath(fname);
        Sleep(1000);
    }
    
}


/*
//获取Encoder的CLSID的方法
BOOL GetEncoderClsid(const WCHAR* format, CLSID* pCLSID)
{
    UINT num = 0;
    UINT size = 0;
    ImageCodecInfo* pImageCodecInfo = NULL;
    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return FALSE;
    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return FALSE;
    GetImageEncoders(num, size, pImageCodecInfo);
    // Find for the support of format for image in the windows
    for (UINT i = 0; i < num; ++i)
    {
        //MimeType: Depiction for the program image 
        if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
        {
            *pCLSID = pImageCodecInfo[i].Clsid;
            free(pImageCodecInfo);
            return TRUE;
        }
    }
    free(pImageCodecInfo);
    return FALSE;
}

HRESULT GetScreenSnap()
{
    ///////////////////////////////////////////
    LPCWSTR     pszFileName = L"d:\\1.jpg";
    int     xs = 200;
    int     ys = 200;
    int     quality = 80;
    ///////////////////////////
    //得到屏幕设备HDC以及长宽      
    HDC hdc = GetWindowDC(NULL);
    int x = GetDeviceCaps(hdc, HORZRES);
    int y = GetDeviceCaps(hdc, VERTRES);
    //创建与屏幕设备兼容的内存区域HDC
    HDC hmemdc = ::CreateCompatibleDC(hdc);

    //创建相关位图,把位图与内存区域HDC绑定
    HBITMAP hbmp = ::CreateCompatibleBitmap(hdc, x, y), hold;
    hold = (HBITMAP)::SelectObject(hmemdc, hbmp);
    //把屏幕设备传到内存区域（位图）中
    BitBlt(hmemdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

    //添加鼠标到图片里
    POINT l_pt;
    ::GetCursorPos(&l_pt);
    HCURSOR l_hCursor = ::GetCursor();
    ::DrawIcon(hmemdc, l_pt.x, l_pt.y, l_hCursor);

    //还原内存区域
    SelectObject(hmemdc, hold);

    //::ShowWindow(m_hWnd, SW_SHOWMAXIMIZED);//SW_SHOWMAXIMIZED
    //得到显示对话框的HDC
    //HDC hdc1 = GetDC(m_hWnd);
    //穿件与对话框兼容的内存区域的DC
    //HDC hmemdc1 = ::CreateCompatibleDC(hdc1);
    //把位图与对话框兼容内存绑定
    //hold = (HBITMAP)::SelectObject(hmemdc1, hbmp);
    //把对话框兼容内存的内容（位图）传至对话框
    //BitBlt(hdc1, 0, 0, x, y, hmemdc1, 0, 0, SRCCOPY);

    //还原内存区域
    //SelectObject(hmemdc1, hold);
    //::UpdateWindow(m_hWnd);
    {

        //使用GDI+保存位图
        Bitmap bit(hbmp, NULL);
        CLSID encoderClsid;
        EncoderParameters encoderParameters;

        encoderParameters.Count = 1;
        encoderParameters.Parameter[0].Guid = EncoderQuality;
        encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
        encoderParameters.Parameter[0].NumberOfValues = 1;
        encoderParameters.Parameter[0].Value = &quality;

        GetEncoderClsid(L"image/jpeg", &encoderClsid);
        bit.Save(pszFileName, &encoderClsid, &encoderParameters);
    }
    ::DeleteObject(hbmp);
    ::DeleteObject(hmemdc);
    //::DeleteObject(hmemdc1);
    //::DeleteObject(hdc1);
    ::DeleteObject(hdc);
    return 0;
}*/


int32_t main()
{
    ScreenMain();

    return 0;
}


