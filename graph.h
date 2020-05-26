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
                bool drawGrid = true
                //,bool drawText = false
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
    apvs.reserve(functions.size());

    std::vector<std::vector<int>> zeroes;
    zeroes.reserve(functions.size());

    for (unsigned i = 0; i < functions.size(); i++)
    {
        apvs.push_back(new POINT[density]);
        zeroes.emplace_back();
    }
    for (unsigned fNum = 0; fNum < functions.size(); fNum++)
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
                zeroes[fNum].push_back(int(dx * (i + 0.5)) + fromX);
            } else if (newY == 0)
            {
                zeroes[fNum].push_back(int(dx * i) + fromX);
            }
            lastY = newY;
            //find function points
            apvs[fNum][i] = POINT
                    ({
                             int(dx * i) + fromX,
                             int(-scale * lastY) + deltaY + fromY
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


    /*std::vector<COLORREF> colors = {
            RGB(0, 127, 255),//light blue
            RGB(255, 0, 0),//red
            RGB(255, 255, 0),//yellow
            RGB(255, 0, 255),//purple
            RGB(0, 255, 127),//emerald
            RGB(255, 127, 0),//orange
            RGB(0, 255, 0),//green
            RGB(0, 255, 255)//cyan
    };*/
    /*std::vector<COLORREF> colors =
            {
                    RGB(128,0,0),
                    RGB(139,0,0),
                    RGB(165,42,42),
                    RGB(178,34,34),
                    RGB(220,20,60),
                    RGB(255,0,0),
                    RGB(255,99,71),
                    RGB(255,127,80),
                    RGB(205,92,92),
                    RGB(240,128,128),
                    RGB(233,150,122),
                    RGB(250,128,114),
                    RGB(255,160,122),
                    RGB(255,69,0),
                    RGB(255,140,0),
                    RGB(255,165,0),
                    RGB(255,215,0),
                    RGB(184,134,11),
                    RGB(218,165,32),
                    RGB(238,232,170),
                    RGB(189,183,107),
                    RGB(240,230,140),
                    RGB(128,128,0),
                    RGB(255,255,0),
                    RGB(154,205,50),
                    RGB(85,107,47),
                    RGB(107,142,35),
                    RGB(124,252,0),
                    RGB(127,255,0),
                    RGB(173,255,47),
                    RGB(0,100,0),
                    RGB(0,128,0),
                    RGB(34,139,34),
                    RGB(0,255,0),
                    RGB(50,205,50),
                    RGB(144,238,144),
                    RGB(152,251,152),
                    RGB(143,188,143),
                    RGB(0,250,154),
                    RGB(0,255,127),
                    RGB(46,139,87),
                    RGB(102,205,170),
                    RGB(60,179,113),
                    RGB(32,178,170),
                    RGB(47,79,79),
                    RGB(0,128,128),
                    RGB(0,139,139),
                    RGB(0,255,255),
                    RGB(0,255,255),
                    RGB(224,255,255),
                    RGB(0,206,209),
                    RGB(64,224,208),
                    RGB(72,209,204),
                    RGB(175,238,238),
                    RGB(127,255,212),
                    RGB(176,224,230),
                    RGB(95,158,160),
                    RGB(70,130,180),
                    RGB(100,149,237),
                    RGB(0,191,255),
                    RGB(30,144,255),
                    RGB(173,216,230),
                    RGB(135,206,235),
                    RGB(135,206,250),
                    RGB(25,25,112),
                    RGB(0,0,128),
                    RGB(0,0,139),
                    RGB(0,0,205),
                    RGB(0,0,255),
                    RGB(65,105,225),
                    RGB(138,43,226),
                    RGB(75,0,130),
                    RGB(72,61,139),
                    RGB(106,90,205),
                    RGB(123,104,238),
                    RGB(147,112,219),
                    RGB(139,0,139),
                    RGB(148,0,211),
                    RGB(153,50,204),
                    RGB(186,85,211),
                    RGB(128,0,128),
                    RGB(216,191,216),
                    RGB(221,160,221),
                    RGB(238,130,238),
                    RGB(255,0,255),
                    RGB(218,112,214),
                    RGB(199,21,133),
                    RGB(219,112,147),
                    RGB(255,20,147),
                    RGB(255,105,180),
                    RGB(255,182,193),
                    RGB(255,192,203),
                    RGB(250,235,215),
                    RGB(245,245,220),
                    RGB(255,228,196),
                    RGB(255,235,205),
                    RGB(245,222,179),
                    RGB(255,248,220),
                    RGB(255,250,205),
                    RGB(250,250,210),
                    RGB(255,255,224),
                    RGB(139,69,19),
                    RGB(160,82,45),
                    RGB(210,105,30),
                    RGB(205,133,63),
                    RGB(244,164,96),
                    RGB(222,184,135),
                    RGB(210,180,140),
                    RGB(188,143,143),
                    RGB(255,228,181),
                    RGB(255,222,173),
                    RGB(255,218,185),
                    RGB(255,228,225),
                    RGB(255,240,245),
                    RGB(250,240,230),
                    RGB(253,245,230),
                    RGB(255,239,213),
                    RGB(255,245,238),
                    RGB(245,255,250),
                    RGB(112,128,144),
                    RGB(119,136,153),
                    RGB(176,196,222),
                    RGB(230,230,250),
                    RGB(255,250,240),
                    RGB(240,248,255),
                    RGB(248,248,255),
                    RGB(240,255,240),
                    RGB(255,255,240),
                    RGB(240,255,255),
                    RGB(255,250,250),
                    RGB(105,105,105),
                    RGB(128,128,128),
                    RGB(169,169,169),
                    RGB(192,192,192),
                    RGB(211,211,211),
                    RGB(220,220,220),
                    RGB(245,245,245)
            };*/


    for (unsigned i = 0; i < apvs.size(); i++)
    {
        //draw graph
        //auto currentColor=colors[rand() % colors.size()];
        auto currentColor = RGB(rand() % 254 + 1, rand() % 254 + 1, rand() % 254 + 1);

        pen = CreatePen(PS_SOLID, 3, currentColor);
        SelectObject(device_context, pen);

        Polyline(device_context, apvs[i], density);

        SelectObject(device_context, CreateSolidBrush(currentColor));
        pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
        SelectObject(device_context, pen);

        int r = 4;
        for (auto &z:zeroes[i])
        {
            Ellipse(device_context, z - r, deltaY + fromY - r, z + r, deltaY + fromY + r);
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
    ReleaseDC(console_handle, device_context);
}

#endif //BRANCHES_GRAPH_H
