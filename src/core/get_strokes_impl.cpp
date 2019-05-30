#include "get_strokes.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;
using namespace std;

PointI2D::PointI2D(const int _x, const int _y): x(_x), y(_y){}
PointI2D::PointI2D(): x(0), y(0){}

const int thresh = 200;

double distance2(const PointI2D &A, const PointI2D &B){
    const int dx = A.x - B.x;
    const int dy = A.y - B.y;
    return dx * dx + dy * dy;
}

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

PointI2D& PointI2D::operator=(const PointI2D &A){
    x = A.x;
    y = A.y;
    return *this;
}

int smaller_point(const PointI2D &A, const PointI2D &B){
    if (A.x != B.x)
        return A.x < B.x;
    return A.y < B.y;
}

int used_image(vector<vector<bool>> & used, const vector<PointI2D> &pointList, PointI2D A, PointI2D B){
    int x1 = A.x,
        y1 = A.y,
        x2 = B.x,
        y2 = B.y;
	int dx = abs(x2 - x1),
		dy = abs(y2 - y1),
		yy = 0;
	if (dx < dy) {
		yy = 1;
		swap(x1, y1);
		swap(x2, y2);
		swap(dx, dy);
	}
	int ix = (x2 - x1) > 0 ? 1 : -1,
        iy = (y2 - y1) > 0 ? 1 : -1,
        cx = x1,
        cy = y1,
        n2dy = dy * 2,
        n2dydx = (dy - dx) * 2,
        d = dy * 2 - dx;
	if (yy) { 
		while (cx != x2) {
			if (d < 0) {
				d += n2dy;
			} else {
				cy += iy;
				d += n2dydx;
			}
            //points.push_back(vector<long>({(long) cy, (long) cx}));
            for (auto A : pointList){
                if (distance2(A, PointI2D(cy, cx)) < 10){
                    used[A.x][A.y] = true;
                }
            }
			cx += ix;
		}
	} else { 
		while (cx != x2) {
			if (d < 0) {
				d += n2dy;
			} else {
				cy += iy;
				d += n2dydx;
			}
            //points.push_back(vector<long>({(long) cx, (long) cy}));
            //used[cx][cy] = true;
            for (auto A : pointList){
                if (distance2(A, PointI2D(cx, cy)) < 10){
                    used[A.x][A.y] = true;
                }
            }
			cx += ix;
		}
	}
    return true;
}

/* 
 * If Points not in image, this ugly code can help it quit quickly.
 * */

//
//
//    int x1 = p1[0],
//        y1 = p1[1],
//        x2 = p2[0],
//        y2 = p2[1];
//	int dx = abs(x2 - x1),
//		dy = abs(y2 - y1),
//		yy = 0;
//	if (dx < dy) {
//		yy = 1;
//		swap_int(&x1, &y1);
//		swap_int(&x2, &y2);
//		swap_int(&dx, &dy);
//	}
//	int ix = (x2 - x1) > 0 ? 1 : -1,
//        iy = (y2 - y1) > 0 ? 1 : -1,
//        cx = x1,
//        cy = y1,
//        n2dy = dy * 2,
//        n2dydx = (dy - dx) * 2,
//        d = dy * 2 - dx;
//	if (yy) { 
//		while (cx != x2) {
//			if (d < 0) {
//				d += n2dy;
//			} else {
//				cy += iy;
//				d += n2dydx;
//			}
//            points.push_back(vector<long>({(long) cy, (long) cx}));
//			cx += ix;
//		}
//	} else { 
//		while (cx != x2) {
//			if (d < 0) {
//				d += n2dy;
//			} else {
//				cy += iy;
//				d += n2dydx;
//			}
//            points.push_back(vector<long>({(long) cx, (long) cy}));
//			cx += ix;
//		}
//	}
int line_in_image(const vector<vector<int>> & img, PointI2D A, PointI2D B){
    if (point_in_image(img, A) == false or point_in_image(img, B) == false){
        return false;
    }
    int x1 = A.x,
        y1 = A.y,
        x2 = B.x,
        y2 = B.y;
	int dx = abs(x2 - x1),
		dy = abs(y2 - y1),
		yy = 0;
	if (dx < dy) {
		yy = 1;
		swap(x1, y1);
		swap(x2, y2);
		swap(dx, dy);
	}
	int ix = (x2 - x1) > 0 ? 1 : -1,
        iy = (y2 - y1) > 0 ? 1 : -1,
        cx = x1,
        cy = y1,
        n2dy = dy * 2,
        n2dydx = (dy - dx) * 2,
        d = dy * 2 - dx;
	if (yy) { 
		while (cx != x2) {
			if (d < 0) {
				d += n2dy;
			} else {
				cy += iy;
				d += n2dydx;
			}
            //points.push_back(vector<long>({(long) cy, (long) cx}));
            if (point_in_image(img, PointI2D(cy, cx)) == false){
                return false;
            }
			cx += ix;
		}
	} else { 
		while (cx != x2) {
			if (d < 0) {
				d += n2dy;
			} else {
				cy += iy;
				d += n2dydx;
			}
            //points.push_back(vector<long>({(long) cx, (long) cy}));
            if (point_in_image(img, PointI2D(cy, cx)) == false){
                return false;
            }
			cx += ix;
		}
	}
    return point_in_image(img, A);
}

vector<pair<PointI2D, PointI2D>> get_strokes(const vector<vector<int>> & img, const vector<PointI2D> & pointList_) {
    vector<vector<bool>> used(img.size(), vector<bool>(img[0].size(), false));
    vector<pair<PointI2D, PointI2D>> ret;
    vector<PointI2D> pointList = pointList_;
    sort(pointList.begin(), pointList.end(), smaller_point);
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
                const double points_dist = distance2(pointList[i], pointList[j]);
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
            used_image(used, pointList, pointList[i], pointList[max_dist_index]);
            ret.push_back(make_pair(pointList[i], pointList[max_dist_index]));
        }
    }
    return ret;
}

double dot(const PointI2D &A, const PointI2D &B){
    return A.x * B.x + A.y * B.y;
}

double norm(const PointI2D &A){
    return sqrt(dot(A, A));
}

PointI2D operator-(const PointI2D &A, const PointI2D &B){
    return PointI2D(A.x - B.x, A.y - B.y);
}

vector<vector<int>> strokes_merge(vector<pair<PointI2D, PointI2D>> &pointList){
    vector<bool> used(pointList.size(), false);
    vector<vector<int>> strokes;
    
    const double DIST_THRESH2 = 10;

    for (unsigned int i = 0; i < pointList.size(); ++i){
        if (used[i]){
            continue;
        }
        strokes.push_back(vector<int>(1, i));
        int cur_point_id = i;
        bool continue_flag = true;
        while (continue_flag){
            continue_flag = false;
            int candidate = -1;
            double min_angle = -1000;
            bool need_swap = false;
            PointI2D curP = pointList[cur_point_id].second;
            for (unsigned int j = i + 1; j < pointList.size(); ++j){
                bool swaped = false;
                double dist1 = sqrt(distance2(curP, pointList[j].first));
                double dist2 = sqrt(distance2(curP, pointList[j].second));
                double dist = dist1;
                if (dist1 > dist2){
                    swap(pointList[j].first, pointList[j].second);
                    dist = dist2;
                    swaped = true;
                }
                if (dist * dist < DIST_THRESH2){
                    PointI2D AVec = curP - pointList[cur_point_id].first;
                    PointI2D BVec = pointList[j].second - pointList[j].first;
                    const double cos_ang = dot(AVec, BVec) / norm(AVec) / norm(BVec);
                    const double ang = acos(cos_ang);
                    if (ang > min_angle){
                        min_angle = ang;
                        candidate = j;
                        need_swap = swaped;
                    }
                }
                if (swaped){
                    swap(pointList[j].first, pointList[j].second);
                }
            }
            if (candidate != -1){
                continue_flag = true;
                if (need_swap){
                    swap(pointList[candidate].first, pointList[candidate].second);
                }
                cur_point_id = candidate;
                strokes.back().push_back(candidate);
            }
        }
    }
    return strokes;
}

