//
// Created by the_E on 20/07/26.
//

#ifndef BRANCHES_WINPC3D_H
#define BRANCHES_WINPC3D_H

#define FPS 60

#include <Windows.h>
#include <gdiplus.h>
#include <objidl.h>
#include <vector>
#include <map>
#include <string>
#include "../branches_core/branches_core.h"
#include "point.h"

#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

namespace bPC3d
{
    std::string function;
    const std::map<std::string, T> *symbols;
    int density;
    double zoom;
    bool drawAxis;
    bool drawGrid;
    double intervalFrom;
    double intervalTo;
    HWND lastHandle;
    int rot;
    bool rotating;
    vectorParser *f;
}


VOID OnPaintPC3d(HDC hdc)
{
    Graphics graphics(hdc);

    try
    {
        ///calculate graph constants
        int sizeX;
        int sizeY;
        {
            RECT rcCli;
            GetClientRect(WindowFromDC(hdc), &rcCli);
            // then you might have:
            int nWidth = rcCli.right - rcCli.left;
            int nHeight = rcCli.bottom - rcCli.top;

            sizeX = nWidth;
            sizeY = nHeight;
        }
        ///create buffer
        Bitmap bmp(sizeX, sizeY);
        Graphics *graph = Graphics::FromImage(&bmp);

        SolidBrush bg(Color(0, 0, 0));
        Rect bgR(0, 0, sizeX, sizeY);
        graph->FillRectangle(&bg, bgR);

        if (bPC3d::density == 0)bPC3d::density = (sizeY + sizeX) / 20;
        int samples = bPC3d::density;

        double scale = bPC3d::zoom * double(sizeX + sizeY) / 30.0;
        double dA = 3.5;
        double d = double(bPC3d::intervalTo - bPC3d::intervalFrom) / double(samples);


        ///calculate graph points
        auto *apvs = new PointF[samples];
        for (int i = 0; i < samples; i++)
        {
            auto t = d * i + bPC3d::intervalFrom;

            std::map<std::string, T> M = *bPC3d::symbols;
            M.merge(std::map<std::string, T>({{"t", t}}));
            apvs[i] = PointF(intoFrame(p2d(p3d(
                    bPC3d::f->evaluate(M)[0],
                    bPC3d::f->evaluate(M)[1],
                    bPC3d::f->evaluate(M)[2]),
                                           bPC3d::rot), sizeX, sizeY, scale));

            //std::cout<<bPC3d::f->evaluate(M)[0]<<","<<bPC3d::f->evaluate(M)[1]<<","<<bPC3d::f->evaluate(M)[2]<<" - "<<apvs[i].X<<" "<<apvs[i].Y<<std::endl;
        }

        Pen Gr(Color(150, 150, 150), 2);
        PointF no_no_squareHigh[] = {PointF(intoFrame(p2d(p3d(-dA, -dA, dA), bPC3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(dA, -dA, dA), bPC3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(dA, dA, dA), bPC3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(-dA, dA, dA), bPC3d::rot), sizeX, sizeY, scale))};
        PointF no_no_squareLow[] = {PointF(intoFrame(p2d(p3d(-dA, -dA, -dA), bPC3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(dA, -dA, -dA), bPC3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(dA, dA, -dA), bPC3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(-dA, dA, -dA), bPC3d::rot), sizeX, sizeY, scale))};

        ///draw lower square
        graph->DrawPolygon(&Gr, no_no_squareLow, 4);

        ///draw graph
        Pen pA(Color(255, 0, 255), 2);
        Pen pB(Color(0, 255, 255), 2);

        graph->DrawCurve(&pA, apvs, samples);
        ///draw axis


        double rO = 4;
        ///draw origin
        Pen R(Color(255, 0, 0), 4);
        graph->DrawLine(&R, PointF(intoFrame(p2d(p3d(0, 0, 0), bPC3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(dA / 2, 0, 0), bPC3d::rot), sizeX, sizeY, scale)));
        Pen G(Color(0, 255, 0), 4);
        graph->DrawLine(&G, PointF(intoFrame(p2d(p3d(0, 0, 0), bPC3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(0, dA / 2, 0), bPC3d::rot), sizeX, sizeY, scale)));
        Pen B(Color(0, 0, 255), 4);
        graph->DrawLine(&B, PointF(intoFrame(p2d(p3d(0, 0, 0), bPC3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(0, 0, dA / 2), bPC3d::rot), sizeX, sizeY, scale)));
        auto W = SolidBrush(Color(255, 255, 255));
        graph->FillEllipse(&W, REAL(sizeX / 2.0 - rO), REAL(sizeY / 2.0 - rO), REAL(2 * rO), REAL(2 * rO));

        ///draw higher square
        graph->DrawPolygon(&Gr, no_no_squareHigh, 4);

        graphics.DrawImage(&bmp, 0, 0, sizeX, sizeY);

        delete[] apvs;

        delete graph;
    } catch (algebra_tools_::except &e)
    {
        std::cout << e.what() << std::endl;
        DestroyWindow(bPC3d::lastHandle);
    }


}

LRESULT CALLBACK WndProcPC3d(HWND, UINT, WPARAM, LPARAM);

INT WINAPI drawPC3d(
        const std::string &function,
        const std::map<std::string, T> &symbols,
        int density = 0,
        double zoom = 1,
        bool drawAxis = true,
        bool drawGrid = true,
        bool rotating = true,
        double intervalFrom = 0,
        double intervalTo = 1,
        HINSTANCE hInstance = nullptr, HINSTANCE= nullptr, PSTR= nullptr, INT iCmdShow = 1
)
{

    bPC3d::function = function;
    bPC3d::symbols = new std::map(symbols);
    bPC3d::density = density;
    bPC3d::zoom = zoom;
    bPC3d::drawAxis = drawAxis;
    bPC3d::drawGrid = drawGrid;
    bPC3d::rot = 0;
    bPC3d::rotating = rotating;

    bPC3d::intervalFrom = intervalFrom;
    bPC3d::intervalTo = intervalTo;

    bPC3d::f = new vectorParser(bPC3d::function);

    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProcPC3d;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = TEXT("Branches - DrawCurve3D");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
            TEXT("Branches - DrawCurve3D"),   // window class name
            TEXT("Branches - DrawCurve3D"),  // window caption
            WS_OVERLAPPEDWINDOW,      // window style
            CW_USEDEFAULT,            // initial x position
            CW_USEDEFAULT,            // initial y position
            CW_USEDEFAULT,            // initial x size
            CW_USEDEFAULT,            // initial y size
            nullptr,                  // parent window handle
            nullptr,                  // window menu handle
            hInstance,                // program instance handle
            nullptr);                 // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    if (bPC3d::rotating)
    {
        SetTimer(hWnd,             // handle to main window
                 1,            // timer identifier
                 1000 / FPS,                 // 0.1-second interval
                 (TIMERPROC) nullptr);     // no timer callback
    }
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete bPC3d::f;
    delete bPC3d::symbols;
    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProcPC3d(HWND hWnd, UINT message,
                             WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            bPC3d::lastHandle = hWnd;
            OnPaintPC3d(hdc);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_ERASEBKGND:
            return TRUE;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_TIMER:
            if (wParam == 1)
            {
                InvalidateRect(hWnd, nullptr, FALSE);   // invalidate whole window
                bPC3d::rot++;
                bPC3d::rot %= 360;
            }
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc


#endif //BRANCHES_WINPC3D_H
