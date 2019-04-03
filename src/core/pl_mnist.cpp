#include "memread.hpp"
#include "data_patch.hpp"
#include "utils.hpp"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <mlpack/core.hpp>

using namespace std;
using namespace arma;

/* swap int for bresenham algorithm */
inline void swap_int(int *a, int *b);

/* mnist_create_mask(Centers, Neighbor_size, Mask)
 * Centers: a group of 2d positions, [[x1, y1], [x2, y2], ...]
 * Neighbor_size: size of neighborhood
 * Mask: an arma::mat(784 by 1), each element is 1 or 0
 */
PREDICATE(mnist_create_mask, 3) {
    vector<vector<double>> points = list2vecvec<double>(A1, -1, 2);
    mat mask(28, 28);
    mask.zeros(); // set mask to all zero
    int ns = (int) A2;
    for (auto pt = points.begin(); pt != points.end(); ++pt) {
        int x = ((vector<double>) *pt)[0];
        int y = ((vector<double>) *pt)[1];
        // left up most point
        int lu_x = x - ns > 0 ? x - ns : 0;
        int lu_y = y - ns > 0 ? y - ns : 0;
        // right dowm most point
        int rd_x = x + ns < 27 ? x + ns : 27;
        int rd_y = y + ns < 27 ? y + ns : 27;
        for (int xx = lu_x; xx <= rd_x; xx++)
            for (int yy = lu_y; yy <= rd_y; yy++)
                mask(xx, yy) = 1.0;
    }
    mask.reshape(784, 1);
    Col<double> *re = new Col<double>(mask.col(0));
    string addr = ptr2str(re);
    return A3 = (char *) addr.c_str();
}

/* ink_(Mat, [X1, Y1]).
 * determine whether there is ink between P1 and P2
 */
PREDICATE(ink_, 2) {
    char *add = (char*) A1;
    vec *data = str2ptr<vec>(add);
    mat img(*data);
    img.reshape(28, 28);
    vector<int> p = list2vec<int>(A2, 2);
    int x = p[0];
    int y = p[1];
    return img(x, y) >= 0.5;
}

/* line_points(START, END, POINTS)
 * use bresenham algorithm to get points between two points
 */
PREDICATE(line_points, 3) {
    vector<int> p1 = list2vec<int>(A1);
    vector<int> p2 = list2vec<int>(A2);
    
    vector<vector<long>> points;

    int x1 = p1[0],
        y1 = p1[1],
        x2 = p2[0],
        y2 = p2[1];
	int dx = abs(x2 - x1),
		dy = abs(y2 - y1),
		yy = 0;
	if (dx < dy) {
		yy = 1;
		swap_int(&x1, &y1);
		swap_int(&x2, &y2);
		swap_int(&dx, &dy);
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
            points.push_back(vector<long>({(long) cy, (long) cx}));
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
            points.push_back(vector<long>({(long) cx, (long) cy}));
			cx += ix;
		}
	}
    return A3 = vecvec2list<long>(points);
}

/*
PREDICATE(above_of, 2) {
    return FALSE;
}

PREDICATE(left_of, 2) {
    return FALSE;
}
*/

inline void swap_int(int *a, int *b) {
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}
