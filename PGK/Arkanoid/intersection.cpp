#include "intersection.hpp"
#include "extraFunctions.hpp"

int onSegment(double ax, double ay, double bx, double by, double zx, double zy)
{
    if(bx <= mymax(ax, zx) && bx >= mymin(ax, zx) &&
        by <= mymax(ay, zy) && by >= mymin(ay, zy))
        return true;

    return false;
}

double orientation(double ax, double ay, double bx, double by, double zx, double zy)
{
    double val = (by - ay) * (zx - bx) - (bx - ax) * (zy - by);

    if (val == 0) return 0;

    return (val > 0)? 1: 2;

}

int doIntersect(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy)
{
    double o1 = orientation(ax,ay, bx,by, cx, cy);
    double o2 = orientation(ax,ay,bx,by,dx,dy);
    double o3 = orientation(cx,cy,dx,dy,ax,ay);
    double o4 = orientation(cx,cy,dx,dy,bx,by);

    if(o1 != o2 && o3 != o4)
        return true;

    if(o1 == 0 && onSegment(ax,ay,cx,cy,bx,by)) return true;
    if(o2 == 0 && onSegment(ax,ay,dx,dy,bx,by)) return true;
    if(o3 == 0 && onSegment(cx,cy,ax,ay,dx,dy)) return true;
    if(o4 == 0 && onSegment(cx,cy,bx,by,dx,dy)) return true;

    return false;

}

/*int trianglesIntersection(double ax, double ay, double bx, double by, double cx, double cy,
                          double dx, double dy, double ex, double ey, double fx, double fy)
{
    if(doIntersect(ax,ay,bx,by,dx,dy,ex,ey) || doIntersect(ax,ay,bx,by,dx,dy,fx,fy) || doIntersect(ax,ay,bx,by,ex,ey,fx,fy) ||
       doIntersect(ax,ay,cx,cy,dx,dy,ex,ey) || doIntersect(ax,ay,cx,cy,dx,dy,fx,fy) || doIntersect(ax,ay,cx,cy,ex,ey,fx,fy) ||
       doIntersect(bx,by,cx,cy,dx,dy,ex,ey) || doIntersect(bx,by,cx,cy,dx,dy,fx,fy) || doIntersect(bx,by,cx,cy,ex,ey,fx,fy))
        return true;

    return false;
}
*/

