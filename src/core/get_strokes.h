#ifndef __GET_STROKES_H__
#define __GET_STROKES_H__

#include <vector>
#include <iostream>

#define DEBUG_INFO(x) cout << __FILE__ << "_" << __LINE__ << " : " << #x << " = " << x << endl

struct PointI2D{
    int x;
    int y;
    PointI2D(const int _x, const int _y);
    PointI2D();

    PointI2D& operator=(const PointI2D &A);
};

std::vector<std::pair<PointI2D, PointI2D>> get_strokes(const std::vector<std::vector<int>> & img, const std::vector<PointI2D> & pointList);

#endif

