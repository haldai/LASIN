#include "get_strokes.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
using namespace std;

PointI2D::PointI2D(const int _x, const int _y): x(_x), y(_y){}
PointI2D::PointI2D(): x(0), y(0){}

const int thresh = 127;

int point_in_image(const vector<vector<int>> & img, const PointI2D & A){
    const int x = A.x;
    const int y = A.y;
    return img[x][y] > thresh;
}

int equal_point(const PointI2D & A, const PointI2D & B){
    if (A.x != B.x)
        return false;
    if (A.y != B.y)
        return false;
    return true;
}

int used_image(vector<vector<bool>> & used, PointI2D A, PointI2D B){
    while (equal_point(A, B) == false){
        const int dx = B.x - A.x;
        const int dy = B.y - A.y;
        int ty = 0, tx = 0, mx = 0, my = 0;
    
        if (abs(dx) == abs(dy)){
            mx = 1;
            my = 1;
        } else if (abs(dx) > abs(dy)){
            mx = 1;
            my = 0;
        } else {
            mx = 0;
            my = 1;
        }
        if (dx > 0)
            tx = 1;
        else
            tx = -1;

        if (dy > 0)
            ty = 1;
        else
            ty = -1;

        const int x = A.x;
        const int y = A.y;
        A.x = (x + tx * mx);
        A.y = (y + ty * my);
        used[x + tx * mx][y + ty * my] = true;
    }
    return true;
}

/* 
 * If Points not in image, this ugly code can help it quit quickly.
 * */
int line_in_image(const vector<vector<int>> & img, PointI2D A, PointI2D B){
    if (point_in_image(img, A) == false or point_in_image(img, B) == false){
        return false;
    }
    while (equal_point(A, B) == false){
        const int dx = B.x - A.x;
        const int dy = B.y - A.y;
        int ty = 0, tx = 0, mx = 0, my = 0;
    
        if (abs(dx) == abs(dy)){
            mx = 1;
            my = 1;
        } else if (abs(dx) > abs(dy)){
            mx = 1;
            my = 0;
        } else {
            mx = 0;
            my = 1;
        }
        if (dx > 0)
            tx = 1;
        else
            tx = -1;

        if (dy > 0)
            ty = 1;
        else
            ty = -1;

        const int x = A.x;
        const int y = A.y;
        A.x = (x + tx * mx);
        A.y = (y + ty * my);
        if (point_in_image(img, A) == false){
            return false;
        }
    }
    return point_in_image(img, A);
}

double distance(const PointI2D &A, const PointI2D &B){
    const int dx = A.x - B.x;
    const int dy = A.y - B.y;
    return dx * dx + dy * dy;
}

vector<pair<PointI2D, PointI2D>> get_strokes(const vector<vector<int>> & img, const vector<PointI2D> & pointList) {
    vector<vector<bool>> used(img.size(), vector<bool>(img[0].size(), false));
    vector<pair<PointI2D, PointI2D>> ret;
    for (unsigned int i = 0; i < pointList.size(); ++i){
        const int x = pointList[i].x;
        const int y = pointList[i].y;
        if (used[x][y]){
            continue;
        }
        used[x][y] = true;
        double max_dist = 0;
        int max_dist_index = -1;
        for (unsigned int j = i + 1; j < pointList.size(); ++j){
            if (line_in_image(img, pointList[i], pointList[j])){
                const double points_dist = distance(pointList[i], pointList[j]);
                if (points_dist > max_dist){
                    max_dist = points_dist;
                    max_dist_index = j;
                }
            }
        }
        if (max_dist_index != -1) {
            const int jx = pointList[max_dist_index].x;
            const int jy = pointList[max_dist_index].y;
            used[jx][jy] = true;
            used_image(used, pointList[i], pointList[max_dist_index]);
            ret.push_back(make_pair(pointList[i], pointList[max_dist_index]));
        }
    }
    return ret;
}

