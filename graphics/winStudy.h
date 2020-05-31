//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_WINSTUDY_H
#define BRANCHES_WINSTUDY_H

#include <Windows.h>
#include <gdiplus.h>
#include <objidl.h>
#include <vector>
#include <map>
#include <string>
#include "../branches_core/branches_core.h"

#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

namespace bStudy
{
    std::string function;
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

VOID OnPaintS(HDC hdc)
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

            bStudy::sizeX = nWidth - bStudy::fromX;
            bStudy::sizeY = nHeight - bStudy::fromY;

        }


        if (bStudy::density == 0)bStudy::density = 10 * bStudy::sizeX;


        double deltaY = bStudy::sizeY / 2.0;
        double deltaX = bStudy::sizeX / 2.0;

        double scale = bStudy::zoom * double(bStudy::sizeX + bStudy::sizeY) / 15.0;

        double dx = double(bStudy::sizeX) / double(bStudy::density);

        double margin = 10;

        int DER = 3;
        std::vector<algebraParser> fs;
        fs.reserve(DER);//max capacity, don't make more derivatives until copy constructor is done
        fs.emplace_back(bStudy::function);
        for (int i = 1; i < DER; i++)
        {
            fs.emplace_back(fs[i - 1].derivative("x"));
        }

        std::vector<PointF *> apvs;
        apvs.reserve(fs.size());

        std::vector<std::vector<double>> zeroes;
        zeroes.reserve(fs.size());

        struct neInterval
        {
            double from;
            double to;

            neInterval(double f, double t) : from(f), to(t)
            {}
        };
        std::vector<neInterval> neInts;

        for (unsigned i = 0; i < fs.size(); i++)
        {
            apvs.push_back(new PointF[bStudy::density]);
            zeroes.emplace_back();
        }

        for (unsigned fNum = 0; fNum < fs.size(); fNum++)
        {
            bool ne = false;
            double lastNeFrom;
            double lastY = 0;
            double newY;
            //calculate function points
            for (int i = 0; i <= bStudy::density; i++)
            {
                //find zeroes
                newY = fs[fNum].evaluate(std::map<std::string, double>{{"x", (dx * i - deltaX) / scale}});
                if ((isnan(newY) || isinf(newY)) && !ne && fNum == 0)
                {
                    lastNeFrom = (dx * i);
                    ne = true;
                } else
                {
                    if ((!(isnan(newY) || isinf(newY))) && ne && fNum == 0)
                    {
                        neInts.emplace_back(lastNeFrom, (dx * i));
                        ne = false;
                    }
                    if (newY * lastY < 0)
                    {
                        zeroes[fNum].push_back(dx * (i + 0.5) + bStudy::fromX);
                    } else if (newY == 0)
                    {
                        zeroes[fNum].push_back(dx * i + bStudy::fromX);
                    }
                }
                lastY = newY;
                //find function points
                apvs[fNum][i] = PointF(REAL(dx * i + bStudy::fromX),
                                       REAL(min(max(-scale * lastY + deltaY + bStudy::fromY, -margin),
                                                bStudy::sizeY + margin)));
            }
            if (ne && fNum == 0)
            {
                neInts.emplace_back(lastNeFrom, (dx * bStudy::density));
            }
        }

        /*
        for(auto& [from,to]:neInts)
        {
            std::cout<<from<<" "<<to<<std::endl;
        }*/

        Color black(0, 0, 0);
        Color red(255, 0, 0);
        auto neBrush = HatchBrush(HatchStyleCross, red, black);

        Color growing(127, 20, 20);
        Color falling(20, 20, 127);
        auto growingB = HatchBrush(HatchStyleBackwardDiagonal, falling, growing);
        auto fallingB = HatchBrush(HatchStyleForwardDiagonal, growing, falling);

        Color convex(127, 127, 10);
        Color concave(10, 127, 127);
        auto convexB = SolidBrush(convex);
        auto concaveB = SolidBrush(concave);


        for (unsigned i = 0; i <= zeroes[1].size(); i++)
        {
            double now = i == 0 ? 0 : zeroes[1][i - 1];
            double next = (i == zeroes[1].size()) ? bStudy::sizeX : zeroes[1][i];
            HatchBrush *b;
            auto middlepoint = int(
                    scale *
                    fs[1].evaluate(std::map<std::string, double>{{"x", ((now + next) / 2.0 - deltaX) / scale}}));
            if (middlepoint >= 0)
                b = &growingB;
            else if (middlepoint < 0)
                b = &fallingB;

            auto r = RectF(REAL(now + bStudy::fromX), REAL(bStudy::fromY), REAL(next - now), REAL(bStudy::sizeY));
            graphics.FillRectangle(b, r);
        }

        for (unsigned i = 0; i <= zeroes[2].size(); i++)
        {
            double now = i == 0 ? 0 : zeroes[2][i - 1];
            double next = (i == zeroes[2].size()) ? bStudy::sizeX : zeroes[2][i];
            Brush *b;
            auto middlepoint = int(
                    scale *
                    fs[2].evaluate(std::map<std::string, double>{{"x", ((now + next) / 2.0 - deltaX) / scale}}));
            if (middlepoint >= 0)
                b = &concaveB;
            else if (middlepoint < 0)
                b = &convexB;

            auto r = RectF(REAL(now + bStudy::fromX), REAL(deltaY + bStudy::fromY), REAL(next - now),
                           REAL(bStudy::sizeY - deltaY));
            graphics.FillRectangle(b, r);
        }

        if (bStudy::drawGrid)
        {
            //draw grid
            Pen p(Color(70, 70, 70), 1);


            int divX = bStudy::zoom > 0.2 ? 1 : (bStudy::zoom > 0.05 ? 2 : 5);
            int todrawX = int(deltaX / scale) / divX;
            for (int i = 0; i <= todrawX; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(deltaX + i * scale * divX + bStudy::fromX), REAL(bStudy::fromY)),
                                  PointF(REAL(deltaX + i * scale * divX + bStudy::fromX),
                                         REAL(bStudy::sizeY + bStudy::fromY)));
                graphics.DrawLine(&p, PointF(REAL(deltaX - i * scale * divX + bStudy::fromX), REAL(bStudy::fromY)),
                                  PointF(REAL(deltaX - i * scale * divX + bStudy::fromX),
                                         REAL(bStudy::sizeY + bStudy::fromY)));
            }
            int divY = bStudy::zoom > 0.2 ? 1 : (bStudy::zoom > 0.05 ? 2 : 5);
            int todrawY = int(deltaY / scale) / divY;
            for (int i = 0; i <= todrawY; i++)
            {
                graphics.DrawLine(&p, PointF(REAL(bStudy::fromX), REAL(deltaY + i * scale * divY + bStudy::fromY)),
                                  PointF(REAL(bStudy::sizeX + bStudy::fromX),
                                         REAL(deltaY + i * scale * divY + bStudy::fromY)));
                graphics.DrawLine(&p, PointF(REAL(bStudy::fromX), REAL(deltaY - i * scale * divY + bStudy::fromY)),
                                  PointF(REAL(bStudy::sizeX + bStudy::fromX),
                                         REAL(deltaY - i * scale * divY + bStudy::fromY)));
            }
        }

        if (bStudy::drawAxis)
        {
            //draw axis
            Pen p(Color(140, 140, 140), 2);
            graphics.DrawLine(&p, PointF(REAL(bStudy::fromX), REAL(deltaY + bStudy::fromY)),
                              PointF(REAL(bStudy::sizeX + bStudy::fromX), REAL(deltaY + bStudy::fromY)));
            graphics.DrawLine(&p, PointF(REAL(deltaX + bStudy::fromX), REAL(bStudy::fromY)),
                              PointF(REAL(deltaX + bStudy::fromX), REAL(bStudy::sizeY + bStudy::fromY)));
        }


        Pen ps[] = {
                Pen(Color(0, 255, 0), 3),
                Pen(Color(150, 0, 0, 0), 2),
                Pen(Color(80, 0, 0, 0), 2)
        };

        for (unsigned i = 0; i < apvs.size(); i++)
        {
            //draw graph
            //auto currentColor=colors[rand() % colors.size()];


            graphics.DrawLines(&ps[i], apvs[i], bStudy::density);

            Pen p2(Color(255, 255, 255, 255), 2);

            int r = 4;
            for (auto &z:zeroes[i])
            {
                switch (i)
                {
                    case 0:
                        graphics.DrawEllipse(&p2, REAL(z - r), REAL(deltaY + bStudy::fromY - r), REAL(2 * r),
                                             REAL(2 * r));
                        break;
                    case 1:
                        graphics.DrawRectangle(&p2,
                                               REAL(z - r),
                                               REAL(min(max(-scale * fs[0].evaluate(
                                                       std::map<std::string, double>{{"x", (z - deltaX) / scale}}) +
                                                            deltaY + bStudy::fromY - r, -margin),
                                                        bStudy::sizeY + margin)),
                                               REAL(2 * r),
                                               REAL(2 * r));
                        break;
                    case 2:
                        graphics.DrawRectangle(&p2,
                                               REAL(z - r + 1),
                                               REAL(min(max(-scale * fs[0].evaluate(
                                                       std::map<std::string, double>{{"x", (z - deltaX) / scale}}) +
                                                            deltaY + bStudy::fromY - r + 1, -margin),
                                                        bStudy::sizeY + margin)),
                                               REAL(2 * (r - 1)),
                                               REAL(2 * (r - 1)));
                        break;
                    default:
                        throw algebra_tools_::except("Unexpected Error");
                }
            }
        }

        //cover ne part
        for (auto&[from, to]:neInts)
        {
            auto r = RectF(REAL(from + bStudy::fromX), REAL(bStudy::fromY), REAL(to - from), REAL(bStudy::sizeY));
            graphics.FillRectangle(&neBrush, r);
        }

    } catch (algebra_tools_::except &e)
    {
        std::cout << e.what() << std::endl;
        DestroyWindow(bStudy::lastHandle);
    }

}

LRESULT CALLBACK WndProcS(HWND, UINT, WPARAM, LPARAM);

INT WINAPI drawStudy(
        const std::string &function,
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
    bStudy::function = std::string(function);
    bStudy::fromX = fromX;
    bStudy::fromY = fromY;
    bStudy::sizeX = sizeX;
    bStudy::sizeY = sizeY;
    bStudy::density = density;
    bStudy::zoom = zoom;
    bStudy::drawAxis = drawAxis;
    bStudy::drawGrid = drawGrid;


    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProcS;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = TEXT("Branches - Study");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
            TEXT("Branches - Study"),   // window class name
            TEXT(("Branches - Study of " + function).c_str()),  // window caption
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
    //delete bStudy::function;
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProcS(HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            bStudy::lastHandle = hWnd;
            OnPaintS(hdc);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

#endif //BRANCHES_WINSTUDY_H
