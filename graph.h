//
// Created by the_E on 20/05/24.
//

#ifndef BRANCHES_GRAPH_H
#define BRANCHES_GRAPH_H

#include <Windows.h>
#include "branches_core.h"
#include <random>

void drawGraph
        (
                const std::map<std::string, std::string> &functions,
                int fromX = 0,
                int fromY = 0,
                int sizeX = 0,
                int sizeY = 0,
                int density = 0,
                double zoom = 1,
                bool drawAxis = true,
                bool drawGrid = true,
                bool drawText = false
        )
//set fromX=-1 for half screen (vertical), fromY=-1 for half screen (horizontal), leave to 0 for max size in that direction
{


    // Get window handle to console, and device context
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);
    if (sizeX == 0 || sizeY == 0)
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

        if (sizeX == 0)
        {
            sizeX = nWidth - fromX;
        }
        if (sizeY == 0)
        {
            sizeY = nHeight - fromY;
        }

        if (sizeX <= 0)
        {
            sizeX = 400;
        }
        if (sizeY <= 0)
        {
            sizeY = 400;
        }
    }


    if (density == 0)density = sizeX;


    int deltaY = sizeY / 2;
    int deltaX = sizeX / 2;

    double scale = zoom * double(sizeY) / 5.0;

    double dx = double(sizeX) / double(density);

    HPEN pen;


    std::vector<algebraParser> parsers;
    parsers.reserve(functions.size());
    for (auto const&[key, val] : functions)
    {
        //parse function
        //std::cout<<"adding "<<val<<std::endl;
        parsers.emplace_back(val);
    }

    std::vector<POINT *> apvs;
    for (unsigned i = 0; i < functions.size(); i++)
    {
        apvs.push_back(new POINT[density]);
    }
    for (unsigned fNum = 0; fNum < functions.size(); fNum++)
    {
        //calculate function points
        for (int i = 0; i <= density; i++)
        {
            apvs[fNum][i] = POINT({
                                          int(dx * i) + fromX,
                                          int(-scale *
                                              parsers[fNum].evaluate(
                                                      std::map<std::string, double>{{"x", (dx * i - deltaX) /
                                                                                          scale}})) +
                                          deltaY + fromY
                                  });
        }
    }

    //draw background
    auto brush = CreateSolidBrush(RGB(0, 0, 0));
    auto rect = RECT({fromX, fromY, sizeX + fromX, sizeY + fromY});
    FillRect(device_context, &rect, brush);


    if (drawGrid)
    {
        //draw grid
        pen = CreatePen(PS_SOLID, 1, RGB(70, 70, 70));
        SelectObject(device_context, pen);


        for (int i = 0; i <= (deltaX / int(scale)); i++)
        {
            {
                POINT ax[] = {{deltaX + int(i * scale) + fromX, fromY},
                              {deltaX + int(i * scale) + fromX, sizeY + fromY}};
                Polyline(device_context, ax, 2);
            }
            {
                POINT ax[] = {{deltaX - int(i * scale) + fromX, fromY},
                              {deltaX - int(i * scale) + fromX, sizeY + fromY}};
                Polyline(device_context, ax, 2);
            }
        }
        for (int i = 0; i <= (deltaY / int(scale)); i++)
        {
            {
                POINT ax[] = {{fromX,         deltaY + int(i * scale) + fromY},
                              {sizeX + fromX, deltaY + int(i * scale) + fromY}};
                Polyline(device_context, ax, 2);
            }
            {
                POINT ax[] = {{fromX,         deltaY - int(i * scale) + fromY},
                              {sizeX + fromX, deltaY - int(i * scale) + fromY}};
                Polyline(device_context, ax, 2);
            }
        }
    }

    if (drawAxis)
    {
        //draw axis
        pen = CreatePen(PS_SOLID, 2, RGB(140, 140, 140));
        SelectObject(device_context, pen);

        POINT xAxis[] = {{fromX,         deltaY + fromY},
                         {sizeX + fromX, deltaY + fromY}};
        POINT yAxis[] = {{deltaX + fromX, fromY},
                         {deltaX + fromX, sizeY + fromY}};
        Polyline(device_context, xAxis, 2);
        Polyline(device_context, yAxis, 2);
    }


    std::vector<COLORREF> colors = {
            RGB(0, 127, 255),//light blue
            RGB(255, 0, 0),//red
            RGB(255, 255, 0),//yellow
            RGB(255, 0, 255),//purple
            RGB(0, 255, 127),//emerald
            RGB(255, 127, 0),//orange
            RGB(0, 255, 0),//green
            RGB(0, 255, 255)//cyan
    };

    for (auto &apv: apvs)
    {
        //draw graph
        pen = CreatePen(PS_SOLID, 3, colors[rand() % colors.size()]);
        SelectObject(device_context, pen);

        Polyline(device_context, apv, density);
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

        ReleaseDC(console_handle, device_context);
    }*/
    //delete [] apv;
}

#endif //BRANCHES_GRAPH_H
