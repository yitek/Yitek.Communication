﻿// DirectScreenCapture.cpp : 定义应用程序的入口点。

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>
#include <WinError.h>
#include<iostream>

IDirect3D9* g_pD3DScreenCapture = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
IDirect3DSurface9* g_pSurface = NULL;

LPVOID    screenCaptureData = NULL;

RECT        gScreenRect = { 0,0,0,0 };

BITMAPINFO    bmpInfo;

HRESULT    InitD3D()
{


    
    ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);
    bmpInfo.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biSizeImage = abs(bmpInfo.bmiHeader.biHeight) * bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biBitCount / 8;
    
    HDC    hdc = GetDC(GetDesktopWindow());
    HDC        hBackDC = NULL;
    HBITMAP    hBackBitmap = NULL;
    hBackDC = CreateCompatibleDC(hdc);
    hBackBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, &screenCaptureData, NULL, 0);
    if (hBackBitmap == NULL)
    {
        return 0;
    }
    ReleaseDC(GetDesktopWindow(), hdc);
    


    HWND hWnd = 0;
    D3DDISPLAYMODE    ddm;
    D3DPRESENT_PARAMETERS    d3dpp;

    if ((g_pD3DScreenCapture = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
    {
        return E_FAIL;
    }

    if (FAILED(g_pD3DScreenCapture->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &ddm)))
    {
        return E_FAIL;
    }

    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

    d3dpp.Windowed = true;
    d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    d3dpp.BackBufferFormat = ddm.Format;
    d3dpp.BackBufferHeight = gScreenRect.bottom = ddm.Height;
    d3dpp.BackBufferWidth = gScreenRect.right = ddm.Width;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    if (FAILED(g_pD3DScreenCapture->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    if (FAILED(g_pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &g_pSurface, NULL)))
    {
        return E_FAIL;
    }

    return S_OK;
}
//direct实现的截图
void directScreenCapture(LPVOID screenCaptureData) {
    g_pd3dDevice->GetFrontBufferData(0, g_pSurface);
    D3DLOCKED_RECT    lockedRect;
    if (FAILED(g_pSurface->LockRect(&lockedRect, &gScreenRect, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
    {
        return;
    }
    for (int i = 0; i < gScreenRect.bottom; i++)
    {
        int y = gScreenRect.bottom - i - 1;
        //memcpy((BYTE*)screenCaptureData + (i)*gScreenRect.right * 32 / 8, (BYTE*)lockedRect.pBits + i * lockedRect.Pitch, gScreenRect.right * 32 / 8);
        memcpy((BYTE*)screenCaptureData + (i)*gScreenRect.right * 32 / 8, (BYTE*)lockedRect.pBits + i * lockedRect.Pitch, gScreenRect.right * 32 / 8);
    }
    g_pSurface->UnlockRect();
    //printf("%d\n" , l3);
}

void SaveBitmap(const char* filename) {
    BITMAPINFOHEADER bi = bmpInfo.bmiHeader;

    BITMAPFILEHEADER bfh = { 0 };
    bfh.bfType = ((WORD)('M' << 8) | 'B');
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD dwWrite;
    WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
    WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
    WriteFile(hFile, screenCaptureData, bi.biSizeImage, &dwWrite, NULL);
    CloseHandle(hFile);
}


int main()
{
    //初始化direct3d,用户截屏
    if (FAILED(InitD3D())) {
        std::cout << "初始化失败"<<std::endl;
        return -1; 
    }
    char name[256] = { 0 };
    for (int32_t i = 0; i < 15; ++i)
    {
        sprintf_s(name, 256, "%d.bmp", i);
        printf("shooting %s\n", name);
        std::cout << "开始截屏" << std::endl;
        directScreenCapture(screenCaptureData);
        std::cout << "截屏完成" << std::endl;
        SaveBitmap(name);
        Sleep(1000);
    }

    
    unsigned char* screenData = (unsigned char*)screenCaptureData;
    for (int y = 0; y < 1080; y++) {
        for (int x = 0; x < 1920; x++) {
            int offset = (x + y * 1080) * 4;
            unsigned char b = *(screenData + offset);
            unsigned char g = *(screenData + offset + 1);
            unsigned char r = *(screenData + offset + 2);
            unsigned char a = *(screenData + offset + 3);
        }
    }
    return 0;
}