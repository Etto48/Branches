//
// Created by the_E on 20/07/27.
//

#ifndef BRANCHES_WINPS3D_H
#define BRANCHES_WINPS3D_H

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

namespace bPS3d
{
    std::string function;
    const std::map<std::string, T> *symbols;
    int density;
    double zoom;
    bool drawAxis;
    bool drawGrid;
    HWND lastHandle;
    int rot;
    bool rotating;
    double uFrom;
    double uTo;
    double vFrom;
    double vTo;
    vectorParser *f;
}


VOID OnPaintPS3d(HDC hdc)
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

        if (bPS3d::density == 0)bPS3d::density = 1;
        int samples = bPS3d::density * (sizeX + sizeY) / 80;


        double scale = bPS3d::zoom * double(sizeX + sizeY) / 30.0;
        double dA = 3.5;
        double du = double(bPS3d::uTo-bPS3d::uFrom) / double(samples);
        double dv = double(bPS3d::vTo-bPS3d::vFrom) / double(samples);


        ///calculate graph points
        std::vector<PointF *> apvsU;
        apvsU.reserve(samples+1);
        std::vector<PointF *> apvsV;
        apvsV.reserve(samples+1);
        for (int i = 0; i <= samples; i++)
        {
            apvsU.emplace_back(new PointF[samples+1]);
            apvsV.emplace_back(new PointF[samples+1]);
            for (int j = 0; j <= samples; j++)
            {
                auto u = i*du+bPS3d::uFrom;
                auto v = j*dv+bPS3d::vFrom;

                std::map<std::string, T> M = *bPS3d::symbols;
                M.merge(std::map<std::string, T>({{"u", u},
                                                  {"v", v}}));
                apvsU[i][j] = PointF(intoFrame(p2d(p3d(
                        bPS3d::f->evaluate(M)[0],
                        bPS3d::f->evaluate(M)[1],
                        bPS3d::f->evaluate(M)[2])
                                ,bPS3d::rot), sizeX, sizeY, scale));

                //std::cout<<bPS3d::f->evaluate(M)[0]<<","<<bPS3d::f->evaluate(M)[1]<<","<<bPS3d::f->evaluate(M)[2]<<" - "<<apvsU[i][j].X<<" "<<apvsU[i][j].Y<<std::endl;
            }
        }
        for (int i = 0; i <= samples; i++)
        {
            for (int j = 0; j <= samples; j++)
            {
                apvsV[j][i] = apvsU[i][j];
            }
        }


        Pen Gr(Color(150, 150, 150), 2);
        PointF no_no_squareHigh[] = {PointF(intoFrame(p2d(p3d(-dA, -dA, dA), bPS3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(dA, -dA, dA), bPS3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(dA, dA, dA), bPS3d::rot), sizeX, sizeY, scale)),
                                     PointF(intoFrame(p2d(p3d(-dA, dA, dA), bPS3d::rot), sizeX, sizeY, scale))};
        PointF no_no_squareLow[] = {PointF(intoFrame(p2d(p3d(-dA, -dA, -dA), bPS3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(dA, -dA, -dA), bPS3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(dA, dA, -dA), bPS3d::rot), sizeX, sizeY, scale)),
                                    PointF(intoFrame(p2d(p3d(-dA, dA, -dA), bPS3d::rot), sizeX, sizeY, scale))};

        ///draw lower square
        graph->DrawPolygon(&Gr, no_no_squareLow, 4);

        ///draw graph
        Pen pA(Color(255, 0, 255), 2);
        Pen pB(Color(0, 255, 255), 2);
        for (int i = 0; i <= samples; i++)
        {
            graph->DrawCurve(&pA, apvsU[i], samples+1);
            graph->DrawCurve(&pB, apvsV[i], samples+1);
            ///draw axis

        }

        double rO = 4;
        ///draw origin
        Pen R(Color(255, 0, 0), 4);
        graph->DrawLine(&R, PointF(intoFrame(p2d(p3d(0, 0, 0), bPS3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(dA / 2, 0, 0), bPS3d::rot), sizeX, sizeY, scale)));
        Pen G(Color(0, 255, 0), 4);
        graph->DrawLine(&G, PointF(intoFrame(p2d(p3d(0, 0, 0), bPS3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(0, dA / 2, 0), bPS3d::rot), sizeX, sizeY, scale)));
        Pen B(Color(0, 0, 255), 4);
        graph->DrawLine(&B, PointF(intoFrame(p2d(p3d(0, 0, 0), bPS3d::rot), sizeX, sizeY, scale)),
                        PointF(intoFrame(p2d(p3d(0, 0, dA / 2), bPS3d::rot), sizeX, sizeY, scale)));
        auto W = SolidBrush(Color(255, 255, 255));
        graph->FillEllipse(&W, REAL(sizeX / 2.0 - rO), REAL(sizeY / 2.0 - rO), REAL(2 * rO), REAL(2 * rO));

        ///draw higher square
        graph->DrawPolygon(&Gr, no_no_squareHigh, 4);

        graphics.DrawImage(&bmp, 0, 0, sizeX, sizeY);
        for (int i = 0; i <= samples; i++)
        {
            delete[] apvsU[i];
            delete[] apvsV[i];
        }
        delete graph;
    } catch (algebra_tools_::except &e)
    {
        std::cout << e.what() << std::endl;
        DestroyWindow(bPS3d::lastHandle);
    }

}

LRESULT CALLBACK WndProcPS3d(HWND, UINT, WPARAM, LPARAM);

INT WINAPI drawPS3d(
        const std::string &function,
        const std::map<std::string, T> &symbols,
        int density = 0,
        double zoom = 1,
        bool drawAxis = true,
        bool drawGrid = true,
        bool rotating = true,
        double uFrom = 0,
        double uTo = 1,
        double vFrom = 0,
        double vTo = 1,
        HINSTANCE hInstance = nullptr, HINSTANCE= nullptr, PSTR= nullptr, INT iCmdShow = 1
)
{

    bPS3d::function = function;
    bPS3d::symbols = new std::map(symbols);
    bPS3d::density = density;
    bPS3d::zoom = zoom;
    bPS3d::drawAxis = drawAxis;
    bPS3d::drawGrid = drawGrid;
    bPS3d::rot = 0;
    bPS3d::rotating = rotating;
    bPS3d::uFrom = uFrom;
    bPS3d::uTo = uTo;
    bPS3d::vFrom = vFrom;
    bPS3d::vTo = vTo;

    bPS3d::f = new vectorParser(bPS3d::function);

    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProcPS3d;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = TEXT("Branches - DrawSurface");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
            TEXT("Branches - DrawSurface"),   // window class name
            TEXT("Branches - DrawSurface"),  // window caption
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
    if (bPS3d::rotating)
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

    delete bPS3d::f;
    delete bPS3d::symbols;
    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProcPS3d(HWND hWnd, UINT message,
                           WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            bPS3d::lastHandle = hWnd;
            OnPaintPS3d(hdc);
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
                bPS3d::rot++;
                bPS3d::rot %= 360;
            }
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

#endif //BRANCHES_WINPS3D_H
