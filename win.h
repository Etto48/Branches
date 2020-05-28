//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_WIN_H
#define BRANCHES_WIN_H

#include <Windows.h>
#include <gdiplus.h>
#include <objidl.h>
#include <vector>
#include <map>
#include <string>
#include "branches_core/branches_core.h"

#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

const std::map<std::string, std::string> *functions;
int fromX;
int fromY;
int sizeX;
int sizeY;
int density;
double zoom;
bool drawAxis;
bool drawGrid;

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
//set fromX=-1 for 2/3 screen (vertical), fromY=-1 for 2/3 screen (horizontal), leave to 0 for max size in that direction
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

            if (fromX == -1)
            {
                fromX = nWidth / 3;
            }
            if (fromY == -1)
            {
                fromY = nHeight / 3;
            }

            sizeX = nWidth - fromX;
            sizeY = nHeight - fromY;

        }


        if (density == 0)density = sizeX;


        double deltaY = sizeY / 2.0;
        double deltaX = sizeX / 2.0;

        double scale = zoom * double(sizeX + sizeY) / 15.0;

        double dx = double(sizeX) / double(density);


        std::vector<algebraParser> parsers;
        parsers.reserve(functions->size());
        for (auto const&[key, val] : *functions)
        {
            //parse function
            //std::cout<<"adding "<<val<<std::endl;
            parsers.emplace_back(val);
        }

        std::vector<PointF *> apvs;
        apvs.reserve(functions->size());

        std::vector<std::vector<double>> zeroes;
        zeroes.reserve(functions->size());

        for (unsigned i = 0; i < functions->size(); i++)
        {
            apvs.push_back(new PointF[density]);
            zeroes.emplace_back();
        }
        for (unsigned fNum = 0; fNum < functions->size(); fNum++)
        {
            double lastY = 0;
            double newY;
            //calculate function points
            for (int i = 0; i <= density; i++)
            {
                //find zeroes
                newY = parsers[fNum].evaluate(std::map<std::string, double>{{"x", (dx * i - deltaX) / scale}});
                if (newY * lastY < 0)
                {
                    zeroes[fNum].push_back(dx * (i + 0.5) + fromX);
                } else if (newY == 0)
                {
                    zeroes[fNum].push_back(dx * i + fromX);
                }
                lastY = newY;
                //find function points
                apvs[fNum][i] = PointF(REAL(dx * i + fromX), REAL(-scale * lastY + deltaY + fromY));
            }
        }

        //draw background
        /*auto brush = CreateSolidBrush(RGB(0, 0, 0));
        auto rect = RECT({fromX, fromY, sizeX + fromX, sizeY + fromY});
        FillRect(device_context, &rect, brush);*/


        if (drawGrid)
        {
            //draw grid
            Pen p(Color(70, 70, 70), 1);


            int divX = zoom > 0.2 ? 1 : (zoom > 0.05 ? 2 : 5);
            int todrawX = int(deltaX / scale) / divX;
            for (int i = 0; i <= todrawX; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(deltaX + i * scale * divX + fromX), REAL(fromY)),
                                  PointF(REAL(deltaX + i * scale * divX + fromX), REAL(sizeY + fromY)));
                graphics.DrawLine(&p, PointF(REAL(deltaX - i * scale * divX + fromX), REAL(fromY)),
                                  PointF(REAL(deltaX - i * scale * divX + fromX), REAL(sizeY + fromY)));
            }
            int divY = zoom > 0.2 ? 1 : (zoom > 0.05 ? 2 : 5);
            int todrawY = int(deltaY / scale) / divY;
            for (int i = 0; i <= todrawY; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(fromX), REAL(deltaY + i * scale * divY + fromY)),
                                  PointF(REAL(sizeX + fromX), REAL(deltaY + i * scale * divY + fromY)));
                graphics.DrawLine(&p, PointF(REAL(fromX), REAL(deltaY - i * scale * divY + fromY)),
                                  PointF(REAL(sizeX + fromX), REAL(deltaY - i * scale * divY + fromY)));
            }
        }

        if (drawAxis)
        {
            //draw axis
            Pen p(Color(140, 140, 140), 2);
            graphics.DrawLine(&p, PointF(REAL(fromX), REAL(deltaY + fromY)),
                              PointF(REAL(sizeX + fromX), REAL(deltaY + fromY)));
            graphics.DrawLine(&p, PointF(REAL(deltaX + fromX), REAL(fromY)),
                              PointF(REAL(deltaX + fromX), REAL(sizeY + fromY)));
        }


        for (unsigned i = 0; i < apvs.size(); i++)
        {
            //draw graph
            //auto currentColor=colors[rand() % colors.size()];
            auto currentColor = Color(rand() % 244 + 11, rand() % 244 + 11, rand() % 244 + 11);

            Pen p(currentColor, 3);

            graphics.DrawLines(&p, apvs[i], density);

            Pen p2(Color(255, 255, 255, 255), 2);

            int r = 4;
            for (auto &z:zeroes[i])
            {
                graphics.DrawEllipse(&p2, REAL(z - r), REAL(deltaY + fromY - r), REAL(2 * r), REAL(2 * r));
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
            auto text = RECT({10 + fromX, 10 + fromY, len + fromX, 24 + fromX});
            DrawTextA(device_context, LPCSTR(str), int(6 + function.length()), &text, DT_LEFT);


        }*/
        //delete [] apv;
        //ReleaseDC(console_handle, device_context);
    }

}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI drawGraph(
        const std::map<std::string, std::string> &lfunctions,
        int lfromX = 0,
        int lfromY = 0,
        int lsizeX = 0,
        int lsizeY = 0,
        int ldensity = 0,
        double lzoom = 1,
        bool ldrawAxis = true,
        bool ldrawGrid = true,

        HINSTANCE hInstance = nullptr, HINSTANCE= nullptr, PSTR= nullptr, INT iCmdShow = 1
)
{
    functions = new std::map(lfunctions);
    fromX = lfromX;
    fromY = lfromY;
    sizeX = lsizeX;
    sizeY = lsizeY;
    density = ldensity;
    zoom = lzoom;
    drawAxis = ldrawAxis;
    drawGrid = ldrawGrid;

    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = TEXT("Branches");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
            TEXT("Branches"),   // window class name
            TEXT("Branches"),  // window caption
            WS_OVERLAPPEDWINDOW,      // window style
            CW_USEDEFAULT,            // initial x position
            CW_USEDEFAULT,            // initial y position
            CW_USEDEFAULT,            // initial x size
            CW_USEDEFAULT,            // initial y size
            nullptr,                     // parent window handle
            nullptr,                     // window menu handle
            hInstance,                // program instance handle
            nullptr);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    delete functions;
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

#endif //BRANCHES_WIN_H
