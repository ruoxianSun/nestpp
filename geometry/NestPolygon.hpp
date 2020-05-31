#ifndef CNESTPATH_H
#define CNESTPATH_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <memory>
#include "UtilCommon.hpp"
#include "UtilGeometry.hpp"
namespace nest {

template<typename PointType>
struct PathType:public std::vector<PointType>{
    using type = PathType;
    using coord_type =PointType;
    using value_type =typename coord_type::value_type;
    using Ptr =typename std::shared_ptr<type>;
    std::vector<Ptr> children;
    Ptr parent;
    coord_type offset;
    int id,bid,source;
    value_type rotation,area;
    bool marked;

    type& add(value_type x,value_type y){
        this->push_back(coord_type(x,y));
        return *this;
    }
    type &add(coord_type coor){
        this->push_back(coor);
        return *this;
    }
    bool operator==(const type&path){
        if(this->size()!=path.size())
        {
            return false;
        }
        for(int i=0;i<this->size();i++)
        {
            if(*this[i]!=path[i])
            {
                return false;
            }
        }
        for(int i=0;i<children.size();i++)
        {
            if(children[i]!=path.children[i])
            {
                return false;
            }
        }
        return true;
    }
    void reverse()
    {
        std::reverse(begin(),end());
    }
    static type cleanNestPath(type srcPath)
    {
        using npGeometry = typename UtilGeometry<type>::type;
        using Common = typename UtilCommon<type,npGeometry>::type;
        /**
        * Convert NestPath 2 Clipper
        */
        using Path=ClipperLib::Path;
        using Paths=ClipperLib::Paths;
        Path path = Common::NestPath2Path(srcPath);
        Paths simple ;
        ClipperLib::SimplifyPolygon(path,simple, ClipperLib::pftEvenOdd);
        if (simple.size() == 0)
        {
            return type();
        }
        Path biggest = simple[0];
        double biggestArea = std::abs(ClipperLib::Area(biggest));
        for (int i = 0; i < simple.size(); i++)
        {
            double area = std::abs(ClipperLib::Area(simple[i]));
            if (area > biggestArea)
            {
                biggest = simple[i];
                biggestArea = area;
            }
        }
        //Path clean = biggest.Cleaned(Config.CURVE_TOLERANCE * Config.CLIIPER_SCALE);
        Path clean ;
        ClipperLib::CleanPolygon(biggest,clean, npGeometry::tol() * Common::clipperScaler());

        if (clean.size() == 0)
        {
            return type();
        }

        /**
         *  Convert Clipper 2 NestPath
         */
        type cleanPath = Common::Path2NestPath(clean);
        cleanPath.id = srcPath.id;
        cleanPath.rotation=(srcPath.rotation);
        return cleanPath;
    }

    std::string toString(){
            std::stringstream ss;
            std::string buf;
            ss<<"NestPath(";
            for(auto v:*this)
            {
                ss<<"["<<v.x<<","<<v.y<<"],";
            }
            ss<<")";
            ss>>buf;
            return buf;
    }
};
}

#endif // CNESTPATH_H
