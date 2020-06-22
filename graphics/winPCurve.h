//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_WINPCURVE_H
#define BRANCHES_WINPCURVE_H

#include <Windows.h>
#include <gdiplus.h>
#include <objidl.h>
#include <vector>
#include <map>
#include <string>
#include "../branches_core/branches_core.h"

#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

namespace bPCurve
{
    std::vector<Color> *fCols;
    std::string function;
    int fromX;
    int fromY;
    int sizeX;
    int sizeY;
    int density;
    double zoom;
    bool drawAxis;
    bool drawGrid;
    double intervalFrom;
    double intervalTo;
    HWND lastHandle;
}

VOID OnPaintCurve(HDC hdc)
{
    Graphics graphics(hdc);
    try
    {


        // Get window handle to console, and device contex
        //HWND console_handle = GetWindow;
        HDC device_context = hdc;
        {
            RECT rcCli;
            GetClientRect(WindowFromDC(device_context), &rcCli);
            // then you might have:
            int nWidth = rcCli.right - rcCli.left;
            int nHeight = rcCli.bottom - rcCli.top;

            bPCurve::sizeX = nWidth - bPCurve::fromX;
            bPCurve::sizeY = nHeight - bPCurve::fromY;

        }


        if (bPCurve::density == 0)bPCurve::density = bPCurve::sizeX;


        double deltaY = bPCurve::sizeY / 2.0;
        double deltaX = bPCurve::sizeX / 2.0;

        double scale = bPCurve::zoom * double(bPCurve::sizeX + bPCurve::sizeY) / 15.0;

        double dx = double(bPCurve::intervalTo - bPCurve::intervalFrom) / double(bPCurve::density);


        //std::vector<vectorParser> parsers;
        //parsers.reserve(bPCurve::functions->size());
        /*for (auto const&[key, val] : *bPCurve::functions)
        {
            //parse function
            //std::cout<<"adding "<<val<<std::endl;
            parsers.emplace_back(val);
        }
        */
        vectorParser parser(bPCurve::function);

        PointF *apv;
        apv = new PointF[bPCurve::density];
        //apvs.reserve(bPCurve::functions->size());
/*
        std::vector<std::vector<double>> zeroes;
        zeroes.reserve(bPCurve::functions->size());

        for (unsigned i = 0; i < bPCurve::functions->size(); i++)
        {
            apvs.push_back(new PointF[bPCurve::density]);
            zeroes.emplace_back();
        }*/
        //for (unsigned fNum = 0; fNum < bPCurve::functions->size(); fNum++)
        //{
        DT v = {0, 0};
        //calculate function points
        for (int i = 0; i <= bPCurve::density; i++)
        {
            //find zeroes
            v = parser.evaluate(std::map<std::string, T>{{"t", (dx * i + bPCurve::intervalFrom)}});
            if ((!isnan(v[0]) && !isnan(v[1])) && (!isinf(v[0]) && !isinf(v[1])))
            {
                /*
                if (newY * lastY < 0)
                {
                    zeroes[fNum].push_back(dx * (i + 0.5) + bPCurve::fromX);
                } else if (newY == 0)
                {
                    zeroes[fNum].push_back(dx * i + bPCurve::fromX);
                }
*/
                //find function points
                apv[i] = PointF(REAL(min(max(scale * v[0] + deltaX + bPCurve::fromX, -4),
                                         bPCurve::sizeX + 4)),
                                REAL(min(max(-scale * v[1] + deltaY + bPCurve::fromY, -4),
                                         bPCurve::sizeY + 4)));
            } else
                apv[i] = PointF(REAL(0), REAL(0));
        }
        //}

        //draw background
        /*auto brush = CreateSolidBrush(RGB(0, 0, 0));
        auto rect = RECT({bPCurve::fromX, bPCurve::fromY, bPCurve::sizeX + bPCurve::fromX, bPCurve::sizeY + bPCurve::fromY});
        FillRect(device_context, &rect, brush);*/


        if (bPCurve::drawGrid)
        {
            //draw grid
            Pen p(Color(70, 70, 70), 1);


            int divX = bPCurve::zoom > 0.2 ? 1 : (bPCurve::zoom > 0.05 ? 2 : 5);
            int todrawX = int(deltaX / scale) / divX;
            for (int i = 0; i <= todrawX; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(deltaX + i * scale * divX + bPCurve::fromX), REAL(bPCurve::fromY)),
                                  PointF(REAL(deltaX + i * scale * divX + bPCurve::fromX),
                                         REAL(bPCurve::sizeY + bPCurve::fromY)));
                graphics.DrawLine(&p, PointF(REAL(deltaX - i * scale * divX + bPCurve::fromX), REAL(bPCurve::fromY)),
                                  PointF(REAL(deltaX - i * scale * divX + bPCurve::fromX),
                                         REAL(bPCurve::sizeY + bPCurve::fromY)));
            }
            int divY = bPCurve::zoom > 0.2 ? 1 : (bPCurve::zoom > 0.05 ? 2 : 5);
            int todrawY = int(deltaY / scale) / divY;
            for (int i = 0; i <= todrawY; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(bPCurve::fromX), REAL(deltaY + i * scale * divY + bPCurve::fromY)),
                                  PointF(REAL(bPCurve::sizeX + bPCurve::fromX),
                                         REAL(deltaY + i * scale * divY + bPCurve::fromY)));
                graphics.DrawLine(&p, PointF(REAL(bPCurve::fromX), REAL(deltaY - i * scale * divY + bPCurve::fromY)),
                                  PointF(REAL(bPCurve::sizeX + bPCurve::fromX),
                                         REAL(deltaY - i * scale * divY + bPCurve::fromY)));
            }
        }

        if (bPCurve::drawAxis)
        {
            //draw axis
            Pen p(Color(140, 140, 140), 2);
            graphics.DrawLine(&p, PointF(REAL(bPCurve::fromX), REAL(deltaY + bPCurve::fromY)),
                              PointF(REAL(bPCurve::sizeX + bPCurve::fromX), REAL(deltaY + bPCurve::fromY)));
            graphics.DrawLine(&p, PointF(REAL(deltaX + bPCurve::fromX), REAL(bPCurve::fromY)),
                              PointF(REAL(deltaX + bPCurve::fromX), REAL(bPCurve::sizeY + bPCurve::fromY)));
        }


        //for (unsigned i = 0; i < apvs.size(); i++)
        //{
        //draw graph
        //auto currentColor=colors[rand() % colors.size()];
        auto currentColor = (*bPCurve::fCols)[0];

        Pen p(currentColor, 3);

        graphics.DrawLines(&p, apv, bPCurve::density);
/*
            Pen p2(Color(255, 255, 255, 255), 2);

            int r = 4;
            for (auto &z:zeroes[i])
            {
                graphics.DrawEllipse(&p2, REAL(z - r), REAL(deltaY + bPCurve::fromY - r), REAL(2 * r), REAL(2 * r));
            }
            */
        //}
        /*
        if(drawText)
        {
            //draw text
            char *str = new char[6 + function.length()];
            std::string textToDraw = "f(x)=" + function;

            strncpy_s(str, 6 + function.length(), textToDraw.c_str(), 6 + function.length());


            int len = int(7 * (textToDraw.length()));
            auto text = RECT({10 + bPCurve::fromX, 10 + bPCurve::fromY, len + bPCurve::fromX, 24 + bPCurve::fromX});
            DrawTextA(device_context, LPCSTR(str), int(6 + function.length()), &text, DT_LEFT);


        }*/
        //delete [] apv;
        //ReleaseDC(console_handle, device_context);
    } catch (algebra_tools_::except &e)
    {
        std::cout << e.what() << std::endl;
        DestroyWindow(bPCurve::lastHandle);
    }

}

LRESULT CALLBACK WndProcCurve(HWND, UINT, WPARAM, LPARAM);

INT WINAPI drawPCurve(
        const std::string &function,
        int fromX = 0,
        int fromY = 0,
        int sizeX = 0,
        int sizeY = 0,
        int density = 0,
        double zoom = 1,
        bool drawAxis = true,
        bool drawGrid = true,
        double intervalFrom = 0,
        double intervalTo = 1,
        HINSTANCE hInstance = nullptr, HINSTANCE= nullptr, PSTR= nullptr, INT iCmdShow = 1
)
{
    bPCurve::fCols = new std::vector<Color>();
    bPCurve::fCols->reserve(1);
    bPCurve::function = function;
    bPCurve::fromX = fromX;
    bPCurve::fromY = fromY;
    bPCurve::sizeX = sizeX;
    bPCurve::sizeY = sizeY;
    bPCurve::density = density;
    bPCurve::zoom = zoom;
    bPCurve::drawAxis = drawAxis;
    bPCurve::drawGrid = drawGrid;
    bPCurve::intervalFrom = intervalFrom;
    bPCurve::intervalTo = intervalTo;

    //for (const auto &x : *(bPCurve::functions))
    //{
    bPCurve::fCols->emplace_back(rand() % 244 + 11, rand() % 244 + 11, rand() % 244 + 11);
    //}

    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProcCurve;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = TEXT("Branches - DrawCurve");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
            TEXT("Branches - DrawCurve"),   // window class name
            TEXT("Branches - DrawCurve"),  // window caption
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


    while (GetMessage(&msg, nullptr, 0, 0))
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    //delete bPCurve::functions;
    delete bPCurve::fCols;
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProcCurve(HWND hWnd, UINT message,
                              WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            bPCurve::lastHandle = hWnd;
            OnPaintCurve(hdc);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProcCurve

#endif //BRANCHES_WINPCURVE_H
