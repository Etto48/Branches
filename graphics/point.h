//
// Created by the_E on 20/05/29.
//

#ifndef BRANCHES_POINT_H
#define BRANCHES_POINT_H

#include <cmath>

class p3d;

class p2d;

class p3d
{
public:
    double x;
    double y;
    double z;

    p3d(double X, double Y, double Z) : x(X), y(Y), z(Z)
    {}
};

class p2d
{
public:
    double x;
    double y;

    p2d(double X, double Y) : x(X), y(Y)
    {}

    explicit p2d(const p3d &p, int rot = 0)
    {
        //isometric dumb
        double pi = acos(0);
        double a = rot / 90.0 * pi;
        p3d pRot = p;

        //rotation around z
        pRot.x = p.x * cos(a) - p.y * sin(a);
        pRot.y = p.x * sin(a) + p.y * cos(a);
        pRot.z = p.z;


        x = -pRot.x * cos(pi / 6) + pRot.y * cos(pi / 6);
        y = -pRot.x * sin(pi / 6) - pRot.y * sin(pi / 6) + pRot.z;

    }

    explicit operator PointF() const
    {
        return PointF(REAL(x), REAL(y));
    }
};

p2d intoFrame(p2d p, int sizeX, int sizeY, double scal)//assuming from 0
{
    double deltaX = sizeX / 2.0;
    double deltaY = sizeY / 2.0;
    double off = 8;
    return p2d(max(min(p.x * scal + deltaX, sizeX + off), -off), max(min(-p.y * scal + deltaY, sizeY + off), -off));
}

#endif //BRANCHES_POINT_H
