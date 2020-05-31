#ifndef CGEOMETRY_H
#define CGEOMETRY_H

#include <math.h>
#include <algorithm>
#include <string>
#include <vector>
#include "Primitive.hpp"
namespace nest {
struct LocationEnum{
    enum State{None=0,In,Out,On,Count};
    const std::string name[Count]{"None","In","Out","On"};
    std::string operator[](int i){
        return name[i];
    }
};
template<typename NestPath>
struct UtilGeometry
{
    using coord_type=typename NestPath::coord_type;
    using value_type=typename coord_type::value_type;
    using type = UtilGeometry;
    using rect_type=typename RectangleType<coord_type>::type;
    using vector_type=typename VectorType<coord_type>::type;
    using path_type= typename NestPath::type;
    using path_ptr = typename path_type::Ptr;

    static value_type tol(){return 1e-9;}
    static bool almostEqual(const value_type &a,const value_type &b,const value_type&tolerance=tol())
    {
        return std::abs(a-b)<tolerance;
    }
    static bool onSegment(const coord_type &A,const coord_type &B,const coord_type &p)
    {
        // vertical line
        if (almostEqual(A.x, B.x) && almostEqual(p.x, A.x))
        {
            if (!almostEqual(p.y, B.y) && !almostEqual(p.y, A.y) &&
                    p.y < std::max(B.y, A.y) && p.y > std::min(B.y, A.y))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // horizontal line
        if (almostEqual(A.y, B.y) && almostEqual(p.y, A.y))
        {
            if (!almostEqual(p.x, B.x) && !almostEqual(p.x, A.x) &&
                    p.x < std::max(B.x, A.x) && p.x > std::min(B.x, A.x))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        //range check
        if ((p.x < A.x && p.x < B.x) || (p.x > A.x && p.x > B.x) ||
                (p.y < A.y && p.y < B.y) || (p.y > A.y && p.y > B.y))
        {
            return false;
        }


        // exclude end points
        if ((almostEqual(p.x, A.x) && almostEqual(p.y, A.y)) ||
                (almostEqual(p.x, B.x) && almostEqual(p.y, B.y)))
        {
            return false;
        }

        value_type cross = (p.y - A.y) * (B.x - A.x) - (p.x - A.x) * (B.y - A.y);

        if (std::abs(cross) > tol())
        {
            return false;
        }

        value_type dot = (p.x - A.x) * (B.x - A.x) + (p.y - A.y) * (B.y - A.y);



        if (dot < 0 || almostEqual(dot, 0))
        {
            return false;
        }

        value_type len2 = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);



        if (dot > len2 || almostEqual(dot, len2))
        {
            return false;
        }

        return true;

    }
    static int pointInPolygon(const coord_type &point, const path_type& polygon)
    {
        using State=LocationEnum::State;
        bool inside = false;
        value_type offsetx = polygon.offset.x;
        value_type offsety = polygon.offset.y;

        for (int i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
        {
            value_type xi = polygon[i].x + offsetx;
            value_type yi = polygon[i].y + offsety;
            value_type xj = polygon[j].x + offsetx;
            value_type yj = polygon[j].y + offsety;

            if (almostEqual(xi, point.x) && almostEqual(yi, point.y))
            {
                return State::On; // no result
            }

            if (onSegment(coord_type(xi, yi), coord_type(xj, yj), point))
            {
                return State::On; // exactly on the segment
            }

            if (almostEqual(xi, xj) && almostEqual(yi, yj))
            { // ignore very small lines
                continue;
            }

            bool intersect = ((yi > point.y) != (yj > point.y)) && (point.x < (xj - xi) * (point.y - yi) / (yj - yi) + xi);
            if (intersect) inside = !inside;
        }

        return inside?State::In:State::Out;
    }
    static value_type polygonArea(const path_type& polygon)
    {
        value_type area = 0;
        for (int i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
        {
            coord_type si = polygon[i];
            coord_type sj = polygon[j];
            area += (sj.x + si.x) * (sj.y - si.y);
        }
        return 0.5 * area;
    }
    static rect_type getPolygonBounds(const path_type &polygon)
    {

        value_type xmin = polygon[0].x;
        value_type xmax = polygon[0].x;
        value_type ymin = polygon[0].y;
        value_type ymax = polygon[0].y;

        for (int i = 1; i < polygon.size(); i++)
        {
            value_type x = polygon[i].x;
            value_type y = polygon[i].y;
            if (x > xmax)
            {
                xmax = x;
            }
            else if (x < xmin)
            {
                xmin = x;
            }

            if (y > ymax)
            {
                ymax = y;
            }
            else if (y < ymin)
            {
                ymin = y;
            }
        }
        return rect_type(xmin, ymin, xmax - xmin, ymax - ymin);
    }
    static bool isRectangle(const path_type &poly, value_type tolerance)
    {
        rect_type bb = getPolygonBounds(poly);

        for (int i = 0; i < poly.size(); i++)
        {
            if (!almostEqual(poly[i].x, bb.x, tolerance) && !almostEqual(poly[i].x, bb.x + bb.w, tolerance))
            {
                return false;
            }
            if (!almostEqual(poly[i].y, bb.y, tolerance) && !almostEqual(poly[i].y, bb.y + bb.h, tolerance))
            {
                return false;
            }
        }
        return true;
    }
    static bool intersect(const path_type &CA,const path_type &CB)
    {
        value_type Aoffsetx = CA.offset.x;
        value_type Aoffsety = CA.offset.y;

        value_type Boffsetx = CB.offset.x;
        value_type Boffsety = CB.offset.y;

        path_type A(CA);
        path_type B(CB);

        for (int i = 0; i < A.size() - 1; i++)
        {
            for (int j = 0; j < B.size() - 1; j++)
            {
                coord_type a1(A[i].x + Aoffsetx, A[i].y + Aoffsety);
                coord_type a2(A[i + 1].x + Aoffsetx, A[i + 1].y + Aoffsety);
                coord_type b1(B[j].x + Boffsetx, B[j].y + Boffsety);
                coord_type b2(B[j + 1].x + Boffsetx, B[j + 1].y + Boffsety);


                int prevbindex = (j == 0) ? B.size() - 1 : j - 1;
                int prevaindex = (i == 0) ? A.size() - 1 : i - 1;
                int nextbindex = (j + 1 == B.size() - 1) ? 0 : j + 2;
                int nextaindex = (i + 1 == A.size() - 1) ? 0 : i + 2;

                // go even further back if we happen to hit on a loop end point
                if (B[prevbindex] == B[j] || (almostEqual(B[prevbindex].x, B[j].x) &&
                                              almostEqual(B[prevbindex].y, B[j].y)))
                {
                    prevbindex = (prevbindex == 0) ? B.size() - 1 : prevbindex - 1;
                }

                if (A[prevaindex] == A[i] || (almostEqual(A[prevaindex].x, A[i].x) &&
                                              almostEqual(A[prevaindex].y, A[i].y)))
                {
                    prevaindex = (prevaindex == 0) ? A.size() - 1 : prevaindex - 1;
                }

                // go even further forward if we happen to hit on a loop end point
                if (B[nextbindex] == B[j + 1] || (almostEqual(B[nextbindex].x, B[j + 1].x) &&
                                                  almostEqual(B[nextbindex].y, B[j + 1].y)))
                {
                    nextbindex = (nextbindex == B.size() - 1) ? 0 : nextbindex + 1;
                }

                if (A[nextaindex] == A[i + 1] || (almostEqual(A[nextaindex].x, A[i + 1].x) &&
                                                  almostEqual(A[nextaindex].y, A[i + 1].y)))
                {
                    nextaindex = (nextaindex == A.size() - 1) ? 0 : nextaindex + 1;
                }

                coord_type a0(A[prevaindex].x + Aoffsetx, A[prevaindex].y + Aoffsety);
                coord_type b0(B[prevbindex].x + Boffsetx, B[prevbindex].y + Boffsety);

                coord_type a3(A[nextaindex].x + Aoffsetx, A[nextaindex].y + Aoffsety);
                coord_type b3(B[nextbindex].x + Boffsetx, B[nextbindex].y + Boffsety);

                if (onSegment(a1, a2, b1) || (almostEqual(a1.x, b1.x, 0.01) && almostEqual(a1.y, b1.y, 0.01)))
                {
                    // if a point is on a segment, it could intersect or it could not. Check via the neighboring points
                    int b0in = pointInPolygon(b0, A);
                    int b2in = pointInPolygon(b2, A);
                    if (b0in == LocationEnum::On || b2in == LocationEnum::On)
                    {
                        continue;
                    }
                    if ((b0in ==  LocationEnum::In && b2in == LocationEnum::Out) ||
                            (b0in == LocationEnum::Out && b2in == LocationEnum::In))
                    {

                        return true;
                    }
                    else
                    {
                        continue;
                    }
                }

                if (onSegment(a1, a2, b2) || (almostEqual(a2.x, b2.x) && almostEqual(a2.y, b2.y)))
                {
                    // if a point is on a segment, it could intersect or it could not. Check via the neighboring points
                    int b1in = pointInPolygon(b1, A);
                    int b3in = pointInPolygon(b3, A);
                    if (b1in == LocationEnum::On || b3in == LocationEnum::On)
                    {
                        continue;
                    }
                    if ((b1in == LocationEnum::In && b3in == LocationEnum::Out) ||
                            (b1in == LocationEnum::Out && b3in == LocationEnum::In))
                    {

                        return true;
                    }
                    else
                    {
                        continue;
                    }
                }

                if (onSegment(b1, b2, a1) || (almostEqual(a1.x, b2.x) && almostEqual(a1.y, b2.y)))
                {
                    // if a point is on a segment, it could intersect or it could not. Check via the neighboring points
                    int a0in = pointInPolygon(a0, B);
                    int a2in = pointInPolygon(a2, B);
                    if (a0in == LocationEnum::On || a2in == LocationEnum::On)
                    {
                        continue;
                    }
                    if ((a0in == LocationEnum::In && a2in == LocationEnum::Out) ||
                            (a0in == LocationEnum::Out && a2in == LocationEnum::In))
                    {

                        return true;
                    }
                    else
                    {
                        continue;
                    }
                }

                if (onSegment(b1, b2, a2) || (almostEqual(a2.x, b1.x) &&
                                              almostEqual(a2.y, b1.y)))
                {
                    // if a point is on a segment, it could intersect or
                    // it could not. Check via the neighboring points
                    int a1in = pointInPolygon(a1, B);
                    int a3in = pointInPolygon(a3, B);
                    if (a1in == LocationEnum::On || a3in == LocationEnum::On)
                    {
                        continue;
                    }

                    if ((a1in == LocationEnum::In && a3in == LocationEnum::Out) ||
                            (a1in == LocationEnum::Out && a3in == LocationEnum::In))
                    {

                        return true;
                    }
                    else
                    {
                        continue;
                    }
                }
                coord_type p;
                if(lineIntersect(b1, b2, a1, a2, false,p))
                {
                    return true;
                }
            }
        }

        return false;
    }
    static bool lineIntersect(coord_type A, coord_type B, coord_type E, coord_type F, bool infinite,coord_type&out)
    {
        value_type a1, a2, b1, b2, c1, c2, x, y;

        a1 = B.y - A.y;
        b1 = A.x - B.x;
        c1 = B.x * A.y - A.x * B.y;
        a2 = F.y - E.y;
        b2 = E.x - F.x;
        c2 = F.x * E.y - E.x * F.y;

        value_type denom = a1 * b2 - a2 * b1;

        x = (b1 * c2 - b2 * c1) / denom;
        y = (a2 * c1 - a1 * c2) / denom;


        if (std::isfinite(x) || std::isfinite(y))
        {
            //            System.out.println(" not infi ");
            return false;
        }

        if (!infinite)
        {
            // coincident points do not count as intersecting
            if (std::abs(A.x - B.x) > tol() && ((A.x < B.x) ? x < A.x || x > B.x : x > A.x || x < B.x)) return false;
            if (std::abs(A.y - B.y) > tol() && ((A.y < B.y) ? y < A.y || y > B.y : y > A.y || y < B.y)) return false;

            if (std::abs(E.x - F.x) > tol() && ((E.x < F.x) ? x < E.x || x > F.x : x > E.x || x < F.x)) return false;
            if (std::abs(E.y - F.y) > tol() && ((E.y < F.y) ? y < E.y || y > F.y : y > E.y || y < F.y)) return false;
        }
        out=coord_type(x,y);
        return true;
    }
    static bool searchStartPoint(const path_type &CA, const path_type &CB, bool inside, std::vector<path_ptr> NFP,coord_type&out)
    {
        path_type A(CA),B(CB);
        if (A[0] != A[A.size() - 1])
        {
            A.add(A[0]);
        }

        if (B[0] != B[B.size() - 1])
        {
            B.add(B[0]);
        }

        for (int i = 0; i < A.size() - 1; i++)
        {
            if (!A[i].marked)
            {
                A[i].marked = true;
                for (int j = 0; j < B.size(); j++)
                {
                    B.offset.x = A[i].x - B[j].x;
                    B.offset.y = A[i].y - B[j].y;
                    int Binside;
                    for (int k = 0; k < B.size(); k++)
                    {
                        int inpoly = pointInPolygon(coord_type(B[k].x + B.offset.x, B[k].y + B.offset.y), A);
                        if (inpoly != LocationEnum::On)
                        {
                            Binside = inpoly;
                            break;
                        }
                    }

                    if (Binside == LocationEnum::On)
                    { // A and B are the same
                        return false;
                    }

                    coord_type startPoint(B.offset);

                    if (((Binside != LocationEnum::On && inside) || (Binside == LocationEnum::On && !inside)) &&
                            !intersect(A, B) && !inNfp(startPoint, NFP))
                    {
                        out=startPoint;
                        return true;
                    }

                    // slide B along vector
                    double vx = A[i + 1].x - A[i].x;
                    double vy = A[i + 1].y - A[i].y;

                    value_type d1;
                    bool bdd1 = polygonProjectionDistance(A, B, vector_type(vx, vy),d1);
                    value_type d2;
                    bool bdd2 = polygonProjectionDistance(B, A, vector_type(-vx, -vy),d2);

                    value_type d = 0;

                    // todo: clean this up
                    if (almostEqual(d1,0)&&almostEqual(d2,0))
                    {
                        // nothin
                    }
                    else if (almostEqual(d1,0))
                    {
                        d = d2;
                    }
                    else if (almostEqual(d2,0))
                    {
                        d = d1;
                    }
                    else
                    {
                        d = std::min(d1,d2);
                    }

                    // only slide until no longer negative
                    // todo: clean this up
                    if (!almostEqual(d, 0) && d > 0)
                    {

                    }
                    else
                    {
                        continue;
                    }

                    value_type vd2 = vx * vx + vy * vy;

                    if (d * d < vd2 && !almostEqual((double)d *(double) d, vd2))
                    {
                        value_type vd = std::sqrt(vx * vx + vy * vy);
                        vx *= d / vd;
                        vy *= d / vd;
                    }

                    B.offset.x += vx;
                    B.offset.y += vy;

                    for (int k = 0; k < B.size(); k++)
                    {
                        int inpoly = pointInPolygon(coord_type(B[k].x + B.offset.x, B[k].y + B.offset.y), A);
                        if (inpoly != LocationEnum::On)
                        {
                            Binside = inpoly;
                            break;
                        }
                    }
                    startPoint = coord_type(B.offset.x, B.offset.y);
                    if (((Binside==LocationEnum::In && inside) || (Binside==LocationEnum::Out && !inside)) &&
                            !intersect(A, B) && !inNfp(startPoint, NFP))
                    {
                        out=startPoint;
                        return true;
                    }
                }
            }
        }
        return false;
    }
    static bool pointDistance(coord_type p, coord_type s1, coord_type s2, vector_type normal, bool infinite,value_type &out)
    {
        normal = normal.normalized();
        vector_type dir(normal.y, -normal.x);

        value_type pdot = p.x * dir.x + p.y * dir.y;
        value_type s1dot = s1.x * dir.x + s1.y * dir.y;
        value_type s2dot = s2.x * dir.x + s2.y * dir.y;

        value_type pdotnorm = p.x * normal.x + p.y * normal.y;
        value_type s1dotnorm = s1.x * normal.x + s1.y * normal.y;
        value_type s2dotnorm = s2.x * normal.x + s2.y * normal.y;


        if (!infinite)
        {
            if (((pdot < s1dot || almostEqual(pdot, s1dot)) && (pdot < s2dot || almostEqual(pdot, s2dot))) ||
                    ((pdot > s1dot || almostEqual(pdot, s1dot)) && (pdot > s2dot || almostEqual(pdot, s2dot))))
            {
                return false; // dot doesn't collide with segment, or lies directly on the vertex
            }
            if ((almostEqual(pdot, s1dot) && almostEqual(pdot, s2dot)) &&
                    (pdotnorm > s1dotnorm && pdotnorm > s2dotnorm))
            {
                return std::min(pdotnorm - s1dotnorm, pdotnorm - s2dotnorm);
            }
            if ((almostEqual(pdot, s1dot) && almostEqual(pdot, s2dot)) &&
                    (pdotnorm < s1dotnorm && pdotnorm < s2dotnorm))
            {
                return -std::min(s1dotnorm - pdotnorm, s2dotnorm - pdotnorm);
            }
        }

        out = -(pdotnorm - s1dotnorm + (s1dotnorm - s2dotnorm) * (s1dot - pdot) / (s1dot - s2dot));
        return true;
    }
    static bool segmentDistance(coord_type A, coord_type B, coord_type E, coord_type F, vector_type direction,value_type&out)
    {
        value_type SEGTOL = 1e-9;
        vector_type normal(direction.y, -direction.x);

        vector_type reverse(-direction.x, -direction.y);

        value_type dotA = A.x * normal.x + A.y * normal.y;
        value_type dotB = B.x * normal.x + B.y * normal.y;
        value_type dotE = E.x * normal.x + E.y * normal.y;
        value_type dotF = F.x * normal.x + F.y * normal.y;

        value_type crossA = A.x * direction.x + A.y * direction.y;
        value_type crossB = B.x * direction.x + B.y * direction.y;
        value_type crossE = E.x * direction.x + E.y * direction.y;
        value_type crossF = F.x * direction.x + F.y * direction.y;
        value_type crossABmin = std::min(crossA, crossB);
        value_type crossABmax = std::max(crossA, crossB);

        value_type crossEFmax = std::max(crossE, crossF);
        value_type crossEFmin = std::min(crossE, crossF);

        value_type ABmin = std::min(dotA, dotB);
        value_type ABmax = std::max(dotA, dotB);

        value_type EFmax = std::max(dotE, dotF);
        value_type EFmin = std::min(dotE, dotF);

        if (almostEqual(ABmax, EFmin, SEGTOL) || almostEqual(ABmin, EFmax, SEGTOL))
        {
            return false;
        }
        // segments miss eachother completely
        if (ABmax < EFmin || ABmin > EFmax)
        {
            return false;
        }
        value_type overlap;
        if ((ABmax > EFmax && ABmin < EFmin) || (EFmax > ABmax && EFmin < ABmin))
        {
            overlap = 1;
        }
        else
        {
            value_type minMax = std::min(ABmax, EFmax);
            value_type maxMin = std::max(ABmin, EFmin);

            value_type maxMax = std::max(ABmax, EFmax);
            value_type minMin = std::min(ABmin, EFmin);

            overlap = (minMax - maxMin) / (maxMax - minMin);
        }
        value_type crossABE = (E.y - A.y) * (B.x - A.x) - (E.x - A.x) * (B.y - A.y);
        value_type crossABF = (F.y - A.y) * (B.x - A.x) - (F.x - A.x) * (B.y - A.y);

        if (almostEqual(crossABE, 0) && almostEqual(crossABF, 0))
        {

            vector_type ABnorm(B.y - A.y, A.x - B.x);
            vector_type EFnorm(F.y - E.y, E.x - F.x);

            value_type ABnormlength = std::sqrt(ABnorm.x * ABnorm.x + ABnorm.y * ABnorm.y);
            ABnorm.x /= ABnormlength;
            ABnorm.y /= ABnormlength;

            value_type EFnormlength = std::sqrt(EFnorm.x * EFnorm.x + EFnorm.y * EFnorm.y);
            EFnorm.x /= EFnormlength;
            EFnorm.y /= EFnormlength;

            // segment normals must point in opposite directions
            if (std::abs(ABnorm.y * EFnorm.x - ABnorm.x * EFnorm.y) < SEGTOL && ABnorm.y * EFnorm.y + ABnorm.x * EFnorm.x < 0)
            {
                // normal of AB segment must point in same direction as given direction vector
                double normdot = ABnorm.y * direction.y + ABnorm.x * direction.x;
                // the segments merely slide along eachother
                if (almostEqual(normdot, 0, SEGTOL))
                {
                    return false;
                }
                if (normdot < 0)
                {
                    out=0;
                    return true;
                }
            }
            return false;
        }
        std::vector<value_type> distances;

        // coincident points
        if (almostEqual(dotA, dotE))
        {
            distances.push_back(crossA - crossE);
        }
        else if (almostEqual(dotA, dotF))
        {
            distances.push_back(crossA - crossF);
        }
        else if (dotA > EFmin && dotA < EFmax)
        {
            value_type d ;
            if(pointDistance(A, E, F, reverse, false,d))
            {
                bool push=true;
                if(almostEqual(d,0))
                {//  A currently touches EF, but AB is moving away from EF
                    value_type db;
                    pointDistance(B,E,F,reverse,true,db);
                    if(db<0||almostEqual(db*overlap,0))
                    {
                        push=false;
                    }
                }
                if(push)
                {
                    distances.push_back(d);
                }
            }
        }

        if (almostEqual(dotB, dotE))
        {
            distances.push_back(crossB - crossE);
        }
        else if (almostEqual(dotB, dotF))
        {
            distances.push_back(crossB - crossF);
        }
        else if (dotB > EFmin && dotB < EFmax)
        {
            value_type d;

            if(pointDistance(B, E, F, reverse, false,d))
            {
                bool push=true;
                if(almostEqual(d,0))
                {
                    value_type da;
                    pointDistance(A,E,F,reverse,true,da);
                    if(da<0||almostEqual(da*overlap,0))
                    {
                        push=false;
                    }
                }
                if(push)distances.push_back(d);
            }
        }

        if (dotE > ABmin && dotE < ABmax)
        {
            value_type d;
            if(pointDistance(E, A, B, direction, false,d))
            {
                bool push=true;
                if(almostEqual(d,0))
                {// crossF<crossE A currently touches EF, but AB is moving away from EF
                    value_type df;
                    pointDistance(F,A,B,direction,true,df);
                    if(df<0||almostEqual(df*overlap,0))
                    {
                        push=false;
                    }
                }
                if(push)distances.push_back(d);
            }
        }

        if (dotF > ABmin && dotF < ABmax)
        {
            value_type d;
            if(pointDistance(F,A,B,direction,false,d))
            {// && crossE<crossF A currently touches EF, but AB is moving away from EF
                bool push=true;
                if(almostEqual(d,0))
                {
                    value_type de;
                    pointDistance(E,A,B,direction,true,de);
                    if(de<0||almostEqual(de,overlap,0))
                    {push=false;}
                }
                if(push)distances.push_back(d);
            }
        }

        if (distances.size() == 0)
        {
            return false;
        }

        value_type minElement = std::numeric_limits<value_type>::max();
        for(auto d: distances)
        {
            if (d < minElement)
            {
                minElement = d;
            }
        }
        out=minElement;
        return true;
    }
    static bool polygonSlideDistance(const path_type &TA , const path_type &TB , vector_type direction, bool ignoreNegative,value_type&out)
    {
        double Aoffsetx = TA.offset.x;
        double Aoffsety = TA.offset.y;

        double Boffsetx = TB.offset.x;
        double BoffsetY = TB.offset.y;

        path_type A(TA);
        path_type B(TB);

        if (A[0] != A[A.size() - 1])
        {
            A.add(A[0]);
        }
        if (B[0] != B[B.size() - 1])
        {
            B.add(B[0]);
        }

        path_type edgeA(A);
        path_type edgeB(B);

        out=0;
        vector_type dir = direction.normalized();
        vector_type normal(dir.y, -dir.x);
        vector_type reverse(-dir.x, -dir.y);
        coord_type A1, A2, B1, B2;
        for (int i = 0; i < edgeB.size() - 1; i++)
        {
            for (int j = 0; j < edgeA.size() - 1; j++)
            {
                A1 = coord_type(edgeA[j].x + Aoffsetx, edgeA[j].y + Aoffsety);
                A2 = coord_type(edgeA[j + 1].x + Aoffsetx, edgeA[j + 1].y + Aoffsety);
                B1 = coord_type(edgeB[i].x + Boffsetx, edgeB[i].y + BoffsetY);
                B2 = coord_type(edgeB[i + 1].x + Boffsetx, edgeB[i + 1].y + BoffsetY);

                if ((almostEqual(A1.x, A2.x) && almostEqual(A1.y, A2.y)) || (almostEqual(B1.x, B2.x) && almostEqual(B1.y, B2.y)))
                {
                    continue;
                }
                value_type d;
                if(segmentDistance(A1,A2,B1,B2,dir,d))
                {
                    if(d<out)
                    {
                        if(!ignoreNegative||d>0||almostEqual(d,0))
                        {
                            out=d;
                        }
                    }
                }
            }
        }
        return true;
    }
    static std::vector<path_ptr> noFitPolygon(const path_type &CA ,  const path_type &CB , bool inside, bool searchEdges)
    {
        path_type A(CA),B(CB);
        A.offset.x=(0);
        A.offset.y=(0);

        value_type minA = A[0].y;
        int minAIndex = 0;
        value_type currentAX = A[0].x;
        value_type maxB = B[0].y;
        int maxBIndex = 0;

        for (int i = 1; i < A.size(); i++)
        {
            A[i].marked = false;
            if (almostEqual(A[i].y, minA) && A[i].x < currentAX)
            {
                minA = A[i].y;
                minAIndex = i;
                currentAX = A[i].x;
            }
            else if (A[i].y < minA)
            {
                minA = A[i].y;
                minAIndex = i;
                currentAX = A[i].x;
            }
        }
        for (int i = 1; i < B.size(); i++)
        {
            B[i].marked = false;
            if (B[i].y > maxB)
            {
                maxB = B[i].y;
                maxBIndex = i;
            }
        }
        bool haveStartPoint=true;
        coord_type startPoint(A[minAIndex].x-B[maxBIndex].x,A[minAIndex].y-B[maxBIndex].y);
        if(inside)
        {
            //TODO heuristic for inside
            haveStartPoint = searchStartPoint(A, B, true, std::vector<path_ptr>(),startPoint);

        }

        std::vector<path_ptr> NFPlist;
        struct SegmentRelation{
            int type,A,B;
            SegmentRelation(int t,int a,int b):type(t),A(a),B(b){}
        };
        while (haveStartPoint)
        {
            vector_type* prevvector=0;
            B.offset=startPoint;

            std::vector<SegmentRelation> touching;
            path_ptr NFP=std::make_shared<path_type>();
            NFP->add(coord_type(B[0].x + B.offset.x,
                    B[0].y + B.offset.y));

            value_type referenceX = B[0].x + B.offset.x;
            value_type referenceY = B[0].y + B.offset.y;
            value_type startX = referenceX;
            value_type startY = referenceY;
            int counter = 0;

            // sanity check  , prevent infinite loop
            while (counter < 10 * (A.size() + B.size()))
            {
                touching.clear();
                for (int i = 0; i < A.size(); i++)
                {
                    int nexti = (i == A.size() - 1) ? 0 : i + 1;
                    for (int j = 0; j < B.size(); j++)
                    {
                        int nextj = (j == B.size() - 1) ? 0 : j + 1;
                        if (almostEqual(A[i].x, B[j].x + B.offset.x) && almostEqual(A[i].y, B[j].y + B.offset.y))
                        {
                            touching.push_back(SegmentRelation(0, i, j));
                        }
                        else if (onSegment(A[i], A[nexti], coord_type(B[j].x + B.offset.x, B[j].y + B.offset.y)))
                        {
                            touching.push_back(SegmentRelation(1, nexti, j));
                        }
                        else if (onSegment(coord_type(B[j].x + B.offset.x, B[j].y + B.offset.y),
                                           coord_type(B[nextj].x + B.offset.x, B[nextj].y + B.offset.y),
                                           A[i]))
                        {
                            touching.push_back(SegmentRelation(2, i, nextj));
                        }
                    }
                }


                std::vector<vector_type> vectors;
                for (int i = 0; i < touching.size(); i++)
                {
                    coord_type vertexA(A[touching[i].A]);
                    vertexA.marked = true;

                    int prevAIndex = touching[i].A - 1;
                    int nextAIndex = touching[i].A + 1;

                    prevAIndex = (prevAIndex < 0) ? A.size() - 1 : prevAIndex; // loop
                    nextAIndex = (nextAIndex >= A.size()) ? 0 : nextAIndex; // loop

                    coord_type prevA(A[prevAIndex]);
                    coord_type nextA(A[nextAIndex]);

                    coord_type vertexB( B[touching[i].B]);

                    int prevBIndex = touching[i].B - 1;
                    int nextBIndex = touching[i].B + 1;

                    prevBIndex = (prevBIndex < 0) ? B.size() - 1 : prevBIndex; // loop
                    nextBIndex = (nextBIndex >= B.size()) ? 0 : nextBIndex; // loop

                    coord_type prevB = B[prevBIndex];
                    coord_type nextB = B[nextBIndex];

                    if (touching[i].type == 0)
                    {
                        vector_type vA1(prevA.x - vertexA.x, prevA.y - vertexA.y);
                        vA1.start = vertexA; vA1.end = prevA;

                        vector_type vA2(nextA.x - vertexA.x, nextA.y - vertexA.y);
                        vA2.start = vertexA; vA2.end = nextA;

                        vector_type vB1(vertexB.x - prevB.x, vertexB.y - prevB.y);
                        vB1.start = prevB; vB1.end = vertexB;

                        vector_type vB2(vertexB.x - nextB.x, vertexB.y - nextB.y);
                        vB2.start = nextB; vB2.end = vertexB;

                        vectors.push_back(vA1);
                        vectors.push_back(vA2);
                        vectors.push_back(vB1);
                        vectors.push_back(vB2);
                    }
                    else if (touching[i].type == 1)
                    {

                        vector_type tmp(vertexA.x - (vertexB.x + B.offset.x),
                                        vertexA.y - (vertexB.y + B.offset.y));
                        tmp.start = prevA;
                        tmp.end = vertexA;

                        vector_type tmp2(prevA.x - (vertexB.x + B.offset.x),
                                         prevA.y - (vertexB.y + B.offset.y));
                        tmp2.start = vertexA; tmp2.end = prevA;
                        vectors.push_back(tmp);
                        vectors.push_back(tmp2);

                    }
                    else if (touching[i].type == 2)
                    {
                        vector_type tmp1(vertexA.x - (vertexB.x + B.offset.x),
                                         vertexA.y - (vertexB.y + B.offset.y));
                        tmp1.start = prevB;
                        tmp1.end = vertexB;

                        vector_type tmp2(vertexA.x - (prevB.x + B.offset.x),
                                         vertexA.y - (prevB.y + B.offset.y));
                        tmp2.start = vertexB;
                        tmp2.end = prevB;

                        vectors.push_back(tmp1);
                        vectors.push_back(tmp2);
                    }
                }

                vector_type* translate=0;

                value_type maxd = 0.0;
                for (int i = 0; i < vectors.size(); i++)
                {
                    if (almostEqual(vectors[i].x, 0) && almostEqual(vectors[i].y, 0))
                    {
                        continue;
                    }

                    if (prevvector != 0 && vectors[i].y * prevvector->y + vectors[i].x * prevvector->x < 0)
                    {

                        double vectorlength = std::sqrt(vectors[i].x * vectors[i].x + vectors[i].y * vectors[i].y);
                        coord_type unitv(vectors[i].x / vectorlength, vectors[i].y / vectorlength);


                        double prevlength = std::sqrt(prevvector->x * prevvector->x + prevvector->y * prevvector->y);
                        coord_type prevunit(prevvector->x / prevlength, prevvector->y / prevlength);


                        // we need to scale down to unit vectors to normalize vector length. Could also just do a tan here
                        if (std::abs(unitv.y * prevunit.x - unitv.x * prevunit.y) < 0.0001)
                        {

                            continue;
                        }
                    }
                    //todo polygonSlideDistance
                    value_type d ;
                    bool slided= polygonSlideDistance(A, B, vectors[i], true,d);

                    value_type vecd2 = vectors[i].x * vectors[i].x + vectors[i].y * vectors[i].y;

                    if (!slided || d * d > vecd2)
                    {
                        value_type vecd = std::sqrt(vectors[i].x * vectors[i].x + vectors[i].y * vectors[i].y);
                        d = vecd;
                    }

                    if (slided && d > maxd)
                    {
                        maxd = d;
                        translate = &vectors[i];
                    }

                }

                if (translate == 0 || almostEqual(maxd, 0))
                {
                    // didn't close the loop, something went wrong here
                    if (translate == 0)
                    {

                    }
                    if (almostEqual(maxd, 0))
                    {
                    }
                    NFP->clear();
                    break;
                }

                translate->start.marked = true;
                translate->end.marked = true;

                prevvector = translate;


                // trim
                value_type vlength2 = translate->x * translate->x + translate->y * translate->y;
                if (maxd * maxd < vlength2 && !almostEqual(maxd * maxd, vlength2))
                {
                    value_type scale = std::sqrt((maxd * maxd) / vlength2);
                    translate->x *= scale;
                    translate->y *= scale;
                }

                referenceX += translate->x;
                referenceY += translate->y;


                if (almostEqual(referenceX, startX) && almostEqual(referenceY, startY))
                {
                    // we've made a full loop
                    break;
                }

                // if A and B start on a touching horizontal line, the end point may not be the start point
                bool looped = false;
                if (NFP->size() > 0)
                {
                    for (int i = 0; i < NFP->size() - 1; i++)
                    {
                        if (almostEqual(referenceX, (*NFP)[i].x) && almostEqual(referenceY, (*NFP)[i].y))
                        {
                            looped = true;
                        }
                    }
                }

                if (looped)
                {
                    // we've made a full loop
                    break;
                }

                NFP->add(coord_type(referenceX, referenceY));

                B.offset.x += translate->x;
                B.offset.y += translate->y;
                counter++;
            }

            if (NFP->size() > 0)
            {
                NFPlist.push_back(NFP);
            }

            if (!searchEdges)
            {
                // only get outer NFP or first inner NFP
                break;
            }
            haveStartPoint = searchStartPoint(A, B, inside, NFPlist,startPoint);
        }
        return NFPlist;
    }
    static std::vector<path_ptr> noFitPolygonRectangle(const path_type &A, const path_type &B)
    {
        value_type minAx = A[0].x;
        value_type minAy = A[0].y;
        value_type maxAx = A[0].x;
        value_type maxAy = A[0].y;

        for (int i = 1; i < A.size(); i++)
        {
            if (A[i].x < minAx)
            {
                minAx = A[i].x;
            }
            if (A[i].y < minAy)
            {
                minAy = A[i].y;
            }
            if (A[i].x > maxAx)
            {
                maxAx = A[i].x;
            }
            if (A[i].y > maxAy)
            {
                maxAy = A[i].y;
            }
        }

        value_type minBx = B[0].x;
        value_type minBy = B[0].y;
        value_type maxBx = B[0].x;
        value_type maxBy = B[0].y;
        for (int i = 1; i < B.size(); i++)
        {
            if (B[i].x < minBx)
            {
                minBx = B[i].x;
            }
            if (B[i].y < minBy)
            {
                minBy = B[i].y;
            }
            if (B[i].x > maxBx)
            {
                maxBx = B[i].x;
            }
            if (B[i].y > maxBy)
            {
                maxBy = B[i].y;
            }
        }

        if (maxBx - minBx > maxAx - minAx)
        {
            return std::vector<path_ptr>();
        }
        value_type diffBy = maxBy - minBy;
        value_type diffAy = maxAy - minAy;

        if (diffBy > diffAy)
        {
            return std::vector<path_ptr>();
        }

        std::vector<path_ptr> nfpRect ;
        path_ptr res =std::make_shared<path_type>();
        res->add(minAx - minBx + B[0].x, minAy - minBy + B[0].y);
        res->add(maxAx - maxBx + B[0].x, minAy - minBy + B[0].y);
        res->add(maxAx - maxBx + B[0].x, maxAy - maxBy + B[0].y);
        res->add(minAx - minBx + B[0].x, maxAy - maxBy + B[0].y);
        nfpRect.push_back(res);
        return nfpRect;
    }
    static bool polygonProjectionDistance(const path_type &CA, const path_type &CB, vector_type direction,value_type &out)
    {
        value_type Boffsetx = CB.offset.x;
        value_type Boffsety = CB.offset.y;

        value_type Aoffsetx = CA.offset.x;
        value_type Aoffsety = CA.offset.y;

        path_type A(CA);
        path_type B(CB);

        if (A[0] != A[A.size() - 1])
        {
            A.add(A[0]);
        }

        if (B[0] != B[B.size() - 1])
        {
            B.add(B[0]);
        }

        path_type edgeA = A;
        path_type edgeB = B;
        bool haveDistance=false;
        coord_type p, s1, s2;
        value_type d,distance=std::numeric_limits<value_type>::min();
        for (int i = 0; i < edgeB.size(); i++)
        {
            // the shortest/most negative projection of B onto A
            value_type minprojection=std::numeric_limits<value_type>::max();
            coord_type minp ;
            bool haveminp=false;
            for (int j = 0; j < edgeA.size() - 1; j++)
            {
                p = coord_type(edgeB[i].x + Boffsetx, edgeB[i].y + Boffsety);
                s1 = coord_type(edgeA[j].x + Aoffsetx, edgeA[j].y + Aoffsety);
                s2 = coord_type(edgeA[j+1].x + Aoffsetx, edgeA[j+1].y + Aoffsety);
                if (std::abs((s2.y - s1.y) * direction.x - (s2.x - s1.x) * direction.y) < tol())
                {
                    continue;
                }

                // project point, ignore edge boundaries
                if(pointDistance(p, s1, s2, direction, false,d))
                    if ( d < minprojection)
                    {
                        minprojection = d;
                        minp = p;
                        haveminp=true;
                    }
            }
            if (haveminp && ( minprojection > distance))
            {
                distance = minprojection;
                haveDistance=true;
            }
        }
        out=distance;
        return haveDistance;
    }
    static bool inNfp(coord_type p, std::vector<path_ptr> nfp)
    {
        if (nfp.size()==0)
        {
            return false;
        }
        for (int i = 0; i < nfp.size(); i++)
        {
            for (int j = 0; j < nfp[i]->size(); j++)
            {
                if (almostEqual(p.x, (*nfp[i])[j].x) && almostEqual(p.y, (*nfp[i])[j].y))
                {
                    return true;
                }
            }
        }
        return false;
    }
    static value_type pi(){return value_type(3.14159265358979323846264338327950288);}
    static value_type degress(value_type rad){return value_type(rad*pi()/180);}
    static value_type radius(value_type deg){return value_type(deg*180/pi());}
    static path_ptr rotatePolygon2Polygon(path_ptr ptr, value_type degrees)
    {
        const path_type&polygon=*ptr;
        path_ptr rotated =std::make_shared<path_type>();
        double angle = degress(degrees);
        for (int i = 0; i < polygon.size(); i++)
        {
            double x = polygon[i].x;
            double y = polygon[i].y;
            double x1 = x * std::cos(angle) - y * std::sin(angle);
            double y1 = x * std::sin(angle) + y * std::cos(angle);
            rotated->add(coord_type(x1, y1));
        }
        rotated->id=(polygon.id);
        rotated->source=(polygon.source);
        if (polygon.children.size() > 0)
        {
            for (int j = 0; j < polygon.children.size(); j++)
            {
                rotated->children.push_back(rotatePolygon2Polygon(polygon.children[j], degrees));
            }
        }
        return rotated;
    }
};
}

#endif // CGEOMETRY_H
