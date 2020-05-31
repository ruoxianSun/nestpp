#ifndef COMMONUTIL_H
#define COMMONUTIL_H

#include <clipper/clipper.hpp>
#include <QTime>
#include <QDebug>
namespace nest{
template<typename NestPath,typename Util>
struct UtilCommon
{
    using Path=ClipperLib::Path;
    using Paths=ClipperLib::Paths;
    using IntPoint=ClipperLib::IntPoint;
    using coord_type=typename NestPath::coord_type;
    using value_type=typename coord_type::value_type;
    using cInt=ClipperLib::cInt;
    using util_type=typename Util::type;
    using type=UtilCommon;
    using path_type=NestPath;
    using path_ptr=typename path_type::Ptr;
    static double clipperScaler(){return 10000.0;}
    static void Minkowski(const Path& poly, const Path& path,
      Paths& solution, bool isSum, bool isClosed)
    {
      int delta = (isClosed ? 1 : 0);
      size_t polyCnt = poly.size();
      size_t pathCnt = path.size();
      Paths pp;
      pp.reserve(pathCnt);
      if (isSum)
        for (size_t i = 0; i < pathCnt; ++i)
        {
          Path p;
          p.reserve(polyCnt);
          for (size_t j = 0; j < poly.size(); ++j)
            p.push_back(IntPoint(path[i].X + poly[j].X, path[i].Y + poly[j].Y));
          pp.push_back(p);
        }
      else
        for (size_t i = 0; i < pathCnt; ++i)
        {
          Path p;
          p.reserve(polyCnt);
          for (size_t j = 0; j < poly.size(); ++j)
            p.push_back(IntPoint(path[i].X - poly[j].X, path[i].Y - poly[j].Y));
          pp.push_back(p);
        }
#if 1
      Paths quads;
      quads.reserve((pathCnt + delta) * (polyCnt + 1));
      for (size_t i = 0; i < pathCnt - 1 + delta; ++i)
        for (size_t j = 0; j < polyCnt; ++j)
        {
          Path quad;
          quad.reserve(4);
          quad.push_back(pp[i % pathCnt][j % polyCnt]);
          quad.push_back(pp[(i + 1) % pathCnt][j % polyCnt]);
          quad.push_back(pp[(i + 1) % pathCnt][(j + 1) % polyCnt]);
          quad.push_back(pp[i % pathCnt][(j + 1) % polyCnt]);
          if (!Orientation(quad)) ReversePath(quad);
          quads.push_back(quad);
        }

      ClipperLib::Clipper c;
      c.AddPaths(quads, ClipperLib::ptSubject, true);
      c.Execute(ClipperLib::ctUnion, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
#else
      Paths tmps;
      tmps.resize(pathCnt);
      for(size_t i=0;i<pathCnt;i++)
      {
          Path &tmp=tmps[i];
          tmp.resize(polyCnt);
          for(size_t j=0;j<polyCnt;j++)
          {
              tmp[j]=pp[i][j];
          }
      }
      ClipperLib::Clipper c;
      c.AddPaths(tmps, ClipperLib::ptSubject, true);
      c.Execute(ClipperLib::ctUnion, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
#endif
    }
    static std::vector<path_ptr> minkowskiDifference(const path_type &A, const path_type &B)
    {
        Path Ac = NestPath2Path(A);
        Path Bc = NestPath2Path(B);
        for (int i = 0; i < Bc.size(); i++)
        {
            long X = Bc[i].X;
            long Y = Bc[i].Y;
            IntPoint intPoint = Bc[i];
            intPoint.X= (-1 * X);
            intPoint.Y= (-1 * Y);
            Bc[i] = intPoint;
        }
        QTime time;time.start();
        qDebug()<<"begin minkowski";
        Paths solution;
        Minkowski(Ac, Bc,solution, true,true);
        qDebug()<<"end minkowski,speending : "<<time.elapsed()<<" s.";
        value_type largestArea = std::numeric_limits<value_type>::max();
        path_ptr clipperNfp;
        for (int i = 0; i < solution.size(); i++)
        {

            path_type n = Path2NestPath(solution[i]);
            value_type sarea = util_type::polygonArea(n);
            if (largestArea > sarea)
            {
                clipperNfp = std::make_shared<path_type>(n);
                largestArea = sarea;
            }
        }

        for (int i = 0; i < clipperNfp->size(); i++)
        {
            (*clipperNfp)[i].x += B[0].x;
            (*clipperNfp)[i].y += B[0].y;
        }
        std::vector<path_ptr> nfp;
        nfp.push_back(clipperNfp);
        return nfp;
    }

    static path_type Path2NestPath(Path path)
    {
        path_type nestPath;
        for (int i = 0; i < path.size(); i++)
        {
            IntPoint lp = path[i];
            coord_type coor = toNestCoor(lp.X, lp.Y);
            nestPath.add(coor.x,coor.y);
        }
        return nestPath;
    }
    static coord_type toNestCoor(cInt x,cInt y)
    {
        return coord_type(value_type(x/clipperScaler()),value_type(y/clipperScaler()));
    }
    static Path NestPath2Path(path_type nestPath)
    {
        Path path;
        for(auto v:nestPath)
        {
            IntPoint coor = toClipperCoor(v.x, v.y);
            path.push_back(coor);
        }
        return path;
    }
    static IntPoint toClipperCoor(value_type x,value_type y){
        return IntPoint(x*clipperScaler(),y*clipperScaler());
    }
    static int toTree(std::vector<path_ptr> list, int idstart)
    {
        std::vector<path_ptr> parents;
        int id = idstart;
        /**
                 * 找出所有的内回环
                 */
        for (int i = 0; i < list.size(); i++)
        {
            path_ptr p = list[i];
            bool isChild = false;
            for (int j = 0; j < list.size(); j++)
            {
                if (j == i)
                {
                    continue;
                }

                if (util_type::pointInPolygon((*p)[0], *list[j]) == 1)
                {
                    list[j]->children.push_back(p);
                    p->parent=list[j];
                    isChild = true;
                    break;
                }
            }
            if (!isChild)
            {
                parents.push_back(p);
            }
        }
        /**
        *  将内环从list列表中去除
        */
        for(auto begin=list.begin();begin!=list.end();)
        {
            if(std::find(parents.begin(),parents.end(),*begin)==parents.end())
            {
                begin=list.erase(begin);
            }
            else {
                begin++;
            }
        }

        for (int i = 0; i < parents.size(); i++)
        {
            parents[i]->id=id;
            id++;
        }

        for (int i = 0; i < parents.size(); i++)
        {
            if (parents[i]->children.size() > 0)
            {
                id = toTree(parents[i]->children, id);
            }
        }
        return id;
    }
    static path_type clipperToNestPath(Path polygon)
    {
        path_type normal;
        for (int i = 0; i < polygon.size(); i++)
        {
            coord_type nestCoor = toNestCoor(polygon[i].X, polygon[i].Y);
            normal.add(nestCoor);
        }
        return normal;
    }
    static void offsetTree(std::vector<path_ptr> &t, value_type offset)
    {
        for (int i = 0; i < t.size(); i++)
        {
            std::vector<path_type> offsetPaths = polygonOffset(*t[i], offset);
            if (offsetPaths.size() == 1)
            {
                t[i]->clear();
                path_type from = offsetPaths[0];

                for(coord_type s : from)
                {
                    t[i]->add(s);
                }
            }
            if (t[i]->children.size() > 0)
            {

                offsetTree(t[i]->children, -offset);
            }
        }
    }
    static std::vector<path_type> polygonOffset(const path_type &polygon, value_type offset)
    {
        std::vector<path_type> result ;
        if (offset == 0 || util_type::almostEqual(offset, 0))
        {
            return result;
        }
        Path p;
        for (coord_type s : polygon)
        {
            IntPoint cc = toClipperCoor(s.x, s.y);
            p.push_back(cc);
        }
        int miterLimit = 2;
        ClipperLib::ClipperOffset co(miterLimit, util_type::tol() * clipperScaler());
        co.AddPath(p, ClipperLib::jtSquare, ClipperLib::etClosedPolygon);
        Paths newpaths;
        co.Execute(newpaths, offset * clipperScaler());
        for (int i = 0; i < newpaths.size(); i++)
        {
            result.push_back(clipperToNestPath(newpaths[i]));
        }
        if (offset > 0)
        {
            path_type &from = result[0];
            if (util_type::polygonArea(from) > 0)
            {
                from.reverse();
            }
            from.add(from[0]);
            from.erase(from.begin());
        }
        return result;
    }
    static std::vector<path_ptr> BuildTree(std::vector<path_ptr> parts, value_type curve_tolerance)
    {
        std::vector<path_ptr> polygons;
        for (int i = 0; i < parts.size(); i++)
        {
            path_type cleanPoly = path_type::cleanNestPath(*parts[i]);
            cleanPoly.id = parts[i]->id;
            if (cleanPoly.size() > 2 && std::abs(util_type::polygonArea(cleanPoly)) > curve_tolerance * curve_tolerance)
            {
                cleanPoly.source=i;
                polygons.push_back(std::make_shared<path_type>(cleanPoly));
            }
        }
        toTree(polygons, 0);
        return polygons;
    }
};
}
#endif // COMMONUTIL_H
