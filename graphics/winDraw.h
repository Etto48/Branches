//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_WINDRAW_H
#define BRANCHES_WINDRAW_H

#include <Windows.h>
#include <gdiplus.h>
#include <objidl.h>
#include <vector>
#include <map>
#include <string>
#include "../branches_core/branches_core.h"

#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

namespace bDraw
{
    std::vector<Color> *fCols;
    const std::map<std::string, std::string> *functions;
    int fromX;
    int fromY;
    int sizeX;
    int sizeY;
    int density;
    double zoom;
    bool drawAxis;
    bool drawGrid;
    HWND lastHandle;
}

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
//set fromX=-1 for 2/3 screen (vertical), fromY=-1 for 2/3 screen (horizontal), leave to 0 for max size in that direction
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

            bDraw::sizeX = nWidth - bDraw::fromX;
            bDraw::sizeY = nHeight - bDraw::fromY;

        }


        if (bDraw::density == 0)bDraw::density = bDraw::sizeX;


        double deltaY = bDraw::sizeY / 2.0;
        double deltaX = bDraw::sizeX / 2.0;

        double scale = bDraw::zoom * double(bDraw::sizeX + bDraw::sizeY) / 15.0;

        double dx = double(bDraw::sizeX) / double(bDraw::density);


        std::vector<algebraParser> parsers;
        parsers.reserve(bDraw::functions->size());
        for (auto const&[key, val] : *bDraw::functions)
        {
            //parse function
            //std::cout<<"adding "<<val<<std::endl;
            parsers.emplace_back(val);
        }

        std::vector<PointF *> apvs;
        apvs.reserve(bDraw::functions->size());

        std::vector<std::vector<double>> zeroes;
        zeroes.reserve(bDraw::functions->size());

        for (unsigned i = 0; i < bDraw::functions->size(); i++)
        {
            apvs.push_back(new PointF[bDraw::density]);
            zeroes.emplace_back();
        }
        for (unsigned fNum = 0; fNum < bDraw::functions->size(); fNum++)
        {
            double lastY = 0;
            double newY;
            //calculate function points
            for (int i = 0; i <= bDraw::density; i++)
            {
                //find zeroes
                newY = parsers[fNum].evaluate(std::map<std::string, double>{{"x", (dx * i - deltaX) / scale}});
                if (!isnan(newY) && !isinf(newY))
                {
                    if (newY * lastY < 0)
                    {
                        zeroes[fNum].push_back(dx * (i + 0.5) + bDraw::fromX);
                    } else if (newY == 0)
                    {
                        zeroes[fNum].push_back(dx * i + bDraw::fromX);
                    }


                    //find function points
                    apvs[fNum][i] = PointF(REAL(dx * i + bDraw::fromX),
                                           REAL(min(max(-scale * lastY + deltaY + bDraw::fromY, -4),
                                                    bDraw::sizeY + 4)));
                } else
                    apvs[fNum][i] = PointF(REAL(dx * i + bDraw::fromX), REAL(0));
                lastY = newY;
            }
        }

        //draw background
        /*auto brush = CreateSolidBrush(RGB(0, 0, 0));
        auto rect = RECT({bDraw::fromX, bDraw::fromY, bDraw::sizeX + bDraw::fromX, bDraw::sizeY + bDraw::fromY});
        FillRect(device_context, &rect, brush);*/


        if (bDraw::drawGrid)
        {
            //draw grid
            Pen p(Color(70, 70, 70), 1);


            int divX = bDraw::zoom > 0.2 ? 1 : (bDraw::zoom > 0.05 ? 2 : 5);
            int todrawX = int(deltaX / scale) / divX;
            for (int i = 0; i <= todrawX; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(deltaX + i * scale * divX + bDraw::fromX), REAL(bDraw::fromY)),
                                  PointF(REAL(deltaX + i * scale * divX + bDraw::fromX),
                                         REAL(bDraw::sizeY + bDraw::fromY)));
                graphics.DrawLine(&p, PointF(REAL(deltaX - i * scale * divX + bDraw::fromX), REAL(bDraw::fromY)),
                                  PointF(REAL(deltaX - i * scale * divX + bDraw::fromX),
                                         REAL(bDraw::sizeY + bDraw::fromY)));
            }
            int divY = bDraw::zoom > 0.2 ? 1 : (bDraw::zoom > 0.05 ? 2 : 5);
            int todrawY = int(deltaY / scale) / divY;
            for (int i = 0; i <= todrawY; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(bDraw::fromX), REAL(deltaY + i * scale * divY + bDraw::fromY)),
                                  PointF(REAL(bDraw::sizeX + bDraw::fromX),
                                         REAL(deltaY + i * scale * divY + bDraw::fromY)));
                graphics.DrawLine(&p, PointF(REAL(bDraw::fromX), REAL(deltaY - i * scale * divY + bDraw::fromY)),
                                  PointF(REAL(bDraw::sizeX + bDraw::fromX),
                                         REAL(deltaY - i * scale * divY + bDraw::fromY)));
            }
        }

        if (bDraw::drawAxis)
        {
            //draw axis
            Pen p(Color(140, 140, 140), 2);
            graphics.DrawLine(&p, PointF(REAL(bDraw::fromX), REAL(deltaY + bDraw::fromY)),
                              PointF(REAL(bDraw::sizeX + bDraw::fromX), REAL(deltaY + bDraw::fromY)));
            graphics.DrawLine(&p, PointF(REAL(deltaX + bDraw::fromX), REAL(bDraw::fromY)),
                              PointF(REAL(deltaX + bDraw::fromX), REAL(bDraw::sizeY + bDraw::fromY)));
        }


        for (unsigned i = 0; i < apvs.size(); i++)
        {
            //draw graph
            //auto currentColor=colors[rand() % colors.size()];
            auto currentColor = (*bDraw::fCols)[i];

            Pen p(currentColor, 3);

            graphics.DrawLines(&p, apvs[i], bDraw::density);

            Pen p2(Color(255, 255, 255, 255), 2);

            int r = 4;
            for (auto &z:zeroes[i])
            {
                graphics.DrawEllipse(&p2, REAL(z - r), REAL(deltaY + bDraw::fromY - r), REAL(2 * r), REAL(2 * r));
            }
        }
        /*
        if(drawText)
        {
            //draw text
            char *str = new char[6 + function.length()];
            std::string textToDraw = "f(x)=" + function;

            strncpy_s(str, 6 + function.length(), textToDraw.c_str(), 6 + function.length());


            int len = int(7 * (textToDraw.length()));
            auto text = RECT({10 + bDraw::fromX, 10 + bDraw::fromY, len + bDraw::fromX, 24 + bDraw::fromX});
            DrawTextA(device_context, LPCSTR(str), int(6 + function.length()), &text, DT_LEFT);


        }*/
        //delete [] apv;
        //ReleaseDC(console_handle, device_context);
    } catch (algebra_tools_::except &e)
    {
        std::cout << e.what() << std::endl;
        DestroyWindow(bDraw::lastHandle);
    }

}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI drawGraph(
        const std::map<std::string, std::string> &functions,
        int fromX = 0,
        int fromY = 0,
        int sizeX = 0,
        int sizeY = 0,
        int density = 0,
        double zoom = 1,
        bool drawAxis = true,
        bool drawGrid = true,

        HINSTANCE hInstance = nullptr, HINSTANCE= nullptr, PSTR= nullptr, INT iCmdShow = 1
)
{
    bDraw::fCols = new std::vector<Color>();
    bDraw::fCols->reserve(functions.size());
    bDraw::functions = new std::map(functions);
    bDraw::fromX = fromX;
    bDraw::fromY = fromY;
    bDraw::sizeX = sizeX;
    bDraw::sizeY = sizeY;
    bDraw::density = density;
    bDraw::zoom = zoom;
    bDraw::drawAxis = drawAxis;
    bDraw::drawGrid = drawGrid;

    for (const auto &x : *(bDraw::functions))
    {
        bDraw::fCols->emplace_back(rand() % 244 + 11, rand() % 244 + 11, rand() % 244 + 11);
    }

    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = TEXT("Branches - Draw");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
            TEXT("Branches - Draw"),   // window class name
            TEXT("Branches - Draw"),  // window caption
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
    delete bDraw::functions;
    delete bDraw::fCols;
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            bDraw::lastHandle = hWnd;
            OnPaint(hdc);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

#endif //BRANCHES_WINDRAW_H
