//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_WIN3D_H
#define BRANCHES_WIN3D_H

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

namespace b3d
{
    std::string function;
    int density;
    double zoom;
    bool drawAxis;
    bool drawGrid;
    HWND lastHandle;
    int rot;
    bool rotating;
}


VOID OnPaint3d(HDC hdc)
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

        if (b3d::density == 0)b3d::density = 1;
        int samples = b3d::density * (sizeX + sizeY) / 80;
        auto f = algebraParser(b3d::function);
        double scale = b3d::zoom * double(sizeX + sizeY) / 30.0;
        double d = (sizeX + sizeY) / (4.0 * samples);



        ///calculate graph points
        std::vector<PointF *> apvsA;
        apvsA.reserve(samples);
        std::vector<PointF *> apvsB;
        apvsB.reserve(samples);
        for (int i = 0; i < samples; i++)
        {
            apvsA.emplace_back(new PointF[samples]);
            apvsB.emplace_back(new PointF[samples]);
            for (int j = 0; j < samples; j++)
            {
                auto a = d * (i - samples / 2.0) / scale;
                auto b = d * (j - samples / 2.0) / scale;

                apvsA[i][j] = PointF(intoFrame(p2d(p3d(a, b, f.evaluate({{"x", a},
                                                                         {"y", b}})), b3d::rot), sizeX, sizeY, scale));
                //apvsB[i][j] = PointF(intoFrame(p2d(p3d(b, a, f.evaluate({{"x", b},
                //                                                         {"y", a}})), b3d::rot), sizeX, sizeY, scale));

            }
        }
        for (int i = 0; i < samples; i++)
        {
            for (int j = 0; j < samples; j++)
            {
                apvsB[j][i] = apvsA[i][j];
            }
        }


        double dA = 3.5;

        Pen Gr(Color(150, 150, 150), 2);
        PointF no_no_squareHigh[] = {PointF(intoFrame(p2d(p3d(-dA, -dA, dA), b3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(dA, -dA, dA), b3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(dA, dA, dA), b3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(-dA, dA, dA), b3d::rot), sizeX, sizeY, scale))};
        PointF no_no_squareLow[] = {PointF(intoFrame(p2d(p3d(-dA, -dA, -dA), b3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(dA, -dA, -dA), b3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(dA, dA, -dA), b3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(-dA, dA, -dA), b3d::rot), sizeX, sizeY, scale))};

        ///draw lower square
        graph->DrawPolygon(&Gr, no_no_squareLow, 4);

        ///draw graph
        Pen pA(Color(255, 0, 255), 2);
        Pen pB(Color(0, 255, 255), 2);
        for (int i = 0; i < samples; i++)
        {
            graph->DrawCurve(&pA, apvsA[i], samples);
            graph->DrawCurve(&pB, apvsB[i], samples);
            ///draw axis

        }

        double rO = 4;
        ///draw origin
        Pen R(Color(255, 0, 0), 4);
        graph->DrawLine(&R, PointF(intoFrame(p2d(p3d(0, 0, 0), b3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(dA / 2, 0, 0), b3d::rot), sizeX, sizeY, scale)));
        Pen G(Color(0, 255, 0), 4);
        graph->DrawLine(&G, PointF(intoFrame(p2d(p3d(0, 0, 0), b3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(0, dA / 2, 0), b3d::rot), sizeX, sizeY, scale)));
        Pen B(Color(0, 0, 255), 4);
        graph->DrawLine(&B, PointF(intoFrame(p2d(p3d(0, 0, 0), b3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(0, 0, dA / 2), b3d::rot), sizeX, sizeY, scale)));
        auto W = SolidBrush(Color(255, 255, 255));
        graph->FillEllipse(&W, REAL(sizeX / 2.0 - rO), REAL(sizeY / 2.0 - rO), REAL(2 * rO), REAL(2 * rO));

        ///draw higher square
        graph->DrawPolygon(&Gr, no_no_squareHigh, 4);

        graphics.DrawImage(&bmp, 0, 0, sizeX, sizeY);
        for (int i = 0; i < samples; i++)
        {
            delete[] apvsA[i];
            delete[] apvsB[i];
        }
        delete graph;
    } catch (algebra_tools_::except &e)
    {
        std::cout << e.what() << std::endl;
        DestroyWindow(b3d::lastHandle);
    }


}

LRESULT CALLBACK WndProc3d(HWND, UINT, WPARAM, LPARAM);

INT WINAPI draw3d(
        const std::string &function,
        int density = 0,
        double zoom = 1,
        bool drawAxis = true,
        bool drawGrid = true,
        bool rotating = true,
        HINSTANCE hInstance = nullptr, HINSTANCE= nullptr, PSTR= nullptr, INT iCmdShow = 1
)
{

    b3d::function = function;
    b3d::density = density;
    b3d::zoom = zoom;
    b3d::drawAxis = drawAxis;
    b3d::drawGrid = drawGrid;
    b3d::rot = 0;
    b3d::rotating = rotating;

    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc3d;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = TEXT("Branches - Draw3D");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
            TEXT("Branches - Draw3D"),   // window class name
            TEXT("Branches - Draw3D"),  // window caption
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
    if (b3d::rotating)
    {
        SetTimer(hWnd,             // handle to main window
                 1,            // timer identifier
                 100,                 // 0.1-second interval
                 (TIMERPROC) nullptr);     // no timer callback
    }
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc3d(HWND hWnd, UINT message,
                           WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            b3d::lastHandle = hWnd;
            OnPaint3d(hdc);
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
                b3d::rot++;
                b3d::rot %= 360;
            }
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

#endif //BRANCHES_WIN3D_H
