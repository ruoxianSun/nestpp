#ifndef CSEGMENT_H
#define CSEGMENT_H
#include <string>
#include <sstream>

namespace nest{
template<typename T>
struct PointType{
    using value_type=T;
    using type=PointType;
    value_type x,y;
    bool marked;
    PointType(const PointType&p):x(p.x),y(p.y),marked(p.marked){}
    PointType(value_type a,value_type b):x(a),y(b),marked(false){}
    PointType():x(0),y(0),marked(false){}
    std::string toString(){
        std::stringstream ss;
        ss<<"Point(x:"<<x<<",y:"<<y<<")";
        std::string buf;
        ss>>buf;
        return buf;
    }
    PointType&operator=(const PointType&p){
        x=p.x;y=p.y;marked=p.marked;
        return *this;
    }
    bool operator!=(const PointType&p)
    {
        return (std::abs(x-p.x)>=1e-9||std::abs(y-p.y)>=1e-9);
    }
    bool operator==(const PointType&p)
    {
        return (std::abs(x-p.x)<1e-9&&std::abs(y-p.y)<1e-9);
    }
};
template <typename PointType>
struct SegmentType{
    using type=SegmentType;
    using coord_type=PointType;
    coord_type start,end;
    std::string toString(){
        std::stringstream ss;
        ss<<"Segment("<<start.toString()<<","<<end.toString()<<")";
        std::string buf;
        ss>>buf;
        return buf;
    }
};
template<typename PointType>
struct RectangleType{
    using type=RectangleType;
    using coord_type=PointType;
    using value_type=typename coord_type::value_type;
    value_type x,y,w,h;
    RectangleType(value_type vx,value_type vy,value_type vw,value_type vh):
        x(vx),y(vy),w(vw),h(vh)
    {

    }
    RectangleType():x(0),y(0),w(0),h(0){}
    coord_type topLeft(){return coord_type(x,y);}
    coord_type bottomLeft(){return coord_type(x,y+h);}
    coord_type topRight(){return coord_type(x+w,y);}
    coord_type bottomRight(){return coord_type(x+w,y+h);}
    coord_type center(){return coord_type(x+0.5*w,y+0.5*h);}
    std::string toString(){
        std::stringstream ss;
        ss<<"Rectangle("<<topLeft().toString()
         <<","<<bottomLeft().toString()
        <<","<<bottomRight().toString()
        <<","<<topRight().toString()<<")";
        std::string buf;
        ss>>buf;
        return buf;
    }
};
template<typename PointType>
struct VectorType{
    using value_type=typename PointType::value_type;
    using coord_type=PointType;
    using type=VectorType;
    value_type x,y;
    coord_type start,end;
    VectorType(){}
    VectorType(value_type a,value_type b):x(a),y(b){}
    VectorType normalized()
    {
        if (std::abs((x * x + y * y)-1)<1e-9)
        {
            return *this;
        }
        value_type len = std::sqrt(x * x + y * y);
        value_type inverse = 1 / len;

        return VectorType(x * inverse, y * inverse);
    }
};
}

#endif // CSEGMENT_H
