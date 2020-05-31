#ifndef CTEST_HPP
#define CTEST_HPP
#include <Primitive.hpp>
#include <UtilGeometry.hpp>
#include <NestPolygon.hpp>
#include <UtilNfp.hpp>
#include <QDebug>
#include <SkylineBinPack.h>
#include <QSettings>
#include <QPolygonF>
namespace nest {
using npPoint=typename PointType<double>::type;
using npSegment=typename SegmentType<npPoint>::type;
using npPolygon=typename PathType<npPoint>::type;
using GeometryUtil=typename UtilGeometry<npPolygon>::type;
using npRectangle=typename GeometryUtil::rect_type;
using npNfp=typename UtilNfp<NfpPair<PathType<npPoint>::type>,GeometryUtil>::type;
using npPair=typename npNfp::pair_type;
using npKey=typename npNfp::key_type;
using npParalle=typename npNfp::paralle_type;
using npConfig=typename npNfp::config_type;
using npPolygonPtr=typename npPolygon::Ptr;
using npVector=typename GeometryUtil::vector_type;
using npCommon=typename UtilCommon<npPolygon,GeometryUtil>::type;


class CTest{
public:
    npConfig config;
    CTest(){
        config.concave=false;
        config.usehole=false;
    }
    struct TranslateVector{
        int id;
        double rotation;
        npVector shift;
        TranslateVector(){}
        TranslateVector(int idd,double rt,npVector sh):id(idd),rotation(rt),shift(sh){}
    };
    std::vector<npPolygonPtr> datas(){
        QPolygonF poly=QSettings("poly0.pl",QSettings::IniFormat).value("pl").value<QPolygonF>();
        for(auto &p:poly)
        {
            p.setX(p.x());
            p.setY(p.y());
        }
        npPolygon::Ptr polygon=std::make_shared<npPolygon>();
        for(auto v:poly)
        {
            polygon->add(v.x(),v.y());
        }
        polygon->id = 1;
        polygon->rotation=(4);
//        qDebug()<<"raw size:"<<polygon->size();
//        *polygon=npPolygon::cleanNestPath(*polygon);
//        qDebug()<<"des size:"<<polygon->size();
        std::vector<npPolygonPtr> paths;
        for(int i=0;i<2;i++)
        {
            npPolygonPtr ptr=std::make_shared<npPolygon>(*polygon);
            ptr->id=i;
            ptr->rotation=4;
            paths.push_back(ptr);
        }
        return paths;
    }
    void test(){

        npPolygon::Ptr path =std::make_shared<npPolygon>();
        path->add(0,1);
        path->add(10,1);
        path->add(20,1);
        path->add(30,1);
        path->add(40,1);
        path->add(50,1);
        path->add(60,1);
        path->add(70,1);

        qDebug()<<path->toString().c_str();

        GeometryUtil::value_type a=1000;
        GeometryUtil::value_type b=999.999999999;
        qDebug()<<"a == b"<<GeometryUtil::almostEqual(a,b);

        npPolygon::Ptr rect =std::make_shared<npPolygon>();
        rect->add(0,0);
        rect->add(100,0);
        rect->add(100,100);
        rect->add(0,100);

        npPoint p0(50,50),p1(50,0),p2(150,100.1),p3(50,25),p4(50.000000001,25);
        LocationEnum LE;
        qDebug()<<"p0 "<<LE[GeometryUtil::pointInPolygon(p0,*rect)].c_str();
        qDebug()<<"p1 "<<LE[GeometryUtil::pointInPolygon(p1,*rect)].c_str();
        qDebug()<<"p2 "<<LE[GeometryUtil::pointInPolygon(p2,*rect)].c_str();

        qDebug()<<"rect area:"<<GeometryUtil::polygonArea(*rect);

        rect->reverse();
        qDebug()<<"rect reverse area:"<<GeometryUtil::polygonArea(*rect);

        qDebug()<<"p0 on segment(p0,p1):"<<GeometryUtil::onSegment(p0,p1,p0);
        qDebug()<<"p2 on segment(p0,p1):"<<GeometryUtil::onSegment(p0,p1,p2);
        qDebug()<<"p3 on segment(p0,p1):"<<GeometryUtil::onSegment(p0,p1,p3);
        qDebug()<<"p4 on segment(p0,p1):"<<GeometryUtil::onSegment(p0,p1,p4);

        npPolygon::Ptr ps=std::make_shared<npPolygon>();
        ps->add(p0).add(p1).add(p2).add(p3).add(p4);


        npRectangle bound=GeometryUtil::getPolygonBounds(*ps);
        qDebug()<<bound.toString().c_str();

        ps->clear();
        ps->add(bound.topLeft()).add(bound.bottomLeft()).add(bound.bottomRight()).add(bound.topRight());
        qDebug()<<"path is rectangle:"<<GeometryUtil::isRectangle(*ps,GeometryUtil::tol());


    }
    std::tuple<std::vector<npPolygonPtr>,std::vector<npPolygonPtr>> nfptest(){

        std::vector<npPolygon::Ptr> paths=datas();
        std::vector<npPair> nfpPairs ;
        npKey key;


        for (int i = 0; i < paths.size(); i++)
        {
            npPolygon::Ptr part = paths[i];
            for (int j = 0; j < i; j++)
            {
                npPolygon::Ptr placed = paths[j];
                NfpKey keyed(placed->id, part->id, false, 0, 0);
                nfpPairs.push_back(npPair(placed, part, keyed));
            }
        }
        std::vector<npParalle> generatedNfp ;
        config.concave=false;config.usehole=false;
        for(npPair &nfpPair : nfpPairs)
        {
            npParalle dataTemp = npNfp::nfpGenerator(nfpPair, config);
            generatedNfp.push_back(dataTemp);
        }
        qDebug()<<"nfp size:"<<generatedNfp[0].value.size();
        return {paths,generatedNfp.front().value};
    }

    std::vector<npPolygonPtr> nestSameParts(){
        ///假设前提，所有零件都是一个样子的
        /// 两两配对，其中一个旋转180度
        /// 然后计算配对后的nfp，再然后根据nfp计算最小包围矩
        /// 最后对包围矩进行装箱处理
        ///

        auto paths=datas();
        QMap<int,npPolygonPtr> rawPolys;
        for(auto path:paths)
        {
            rawPolys[path->id]=path;
        }
        auto trees=npCommon::BuildTree(paths,GeometryUtil::tol());
        if(!GeometryUtil::almostEqual(config.spaceing,0))
            npCommon::offsetTree(trees,config.spaceing*0.5);
        QMap<QString,npPair> pairs;
        for(int i=0;i<trees.size()/2;i++)
        {
            npPolygonPtr A=trees[i*2];
            npPolygonPtr B=trees[i*2+1];
            npPair pair(A,B,NfpKey(A->id,B->id,false,0,180));
            QString key(pair.key().c_str());
            pairs[key]=pair;
        }
        std::vector<npParalle> generatedNfp ;
        config.concave=false;
        config.usehole=false;
        for(npPair &nfpPair : pairs)
        {
            npParalle dataTemp = npNfp::nfpGenerator(nfpPair, config);
            generatedNfp.push_back(dataTemp);
        }

        ///根据nfp进行聚合,只记录聚合的面积最小的B零件的摆放向量（包含零件的旋转角度，偏移向量-相对零件轮廓的第一个点）
        QMap<int,TranslateVector> translates;
        for(int i=0;i<generatedNfp.size();i++)
        {
            npParalle paralle=generatedNfp[i];
            npPair pair=pairs[QString::fromStdString(paralle.key())];
            npPolygonPtr pptr=GeometryUtil::rotatePolygon2Polygon(pair.B,pair.key.Brotation);
            const npPolygon& path=*pptr;
            translates[pair.A->id]=TranslateVector(pair.A->id,pair.key.Arotation,npVector(0,0));
            double area=std::numeric_limits<double>::max();
            TranslateVector best(pair.B->id,pair.key.Brotation,npVector(0,0));
            int bestN=0;
            for(int n=0;n<paralle.value.size();n++)
            {
                const npPolygon& nfp=*paralle.value[n];
                for(int m=0;m<nfp.size();m++)
                {
                   npVector shift(nfp[m].x-path[0].x,nfp[m].y-path[0].y);
                   npPolygon points(*pair.A);
                   for(auto v:path)
                   {
                       points.push_back(npPoint(v.x+shift.x,v.y+shift.y));
                   }
                   npRectangle rect=GeometryUtil::getPolygonBounds(points);
                   double aaa=rect.w*rect.h;
                   if(aaa<area)
                   {
                       area=aaa;
                       best.shift=shift;
                       bestN=n;
                   }
                }
            }
            translates[pair.B->id]=best;
            pair.A->parent=paralle.value[bestN];
        }

        //进行状态位置的变换

        for(auto &pair:pairs)
        {
            if(!GeometryUtil::almostEqual(translates[pair.A->id].rotation,0))
            {
                pair.A=GeometryUtil::rotatePolygon2Polygon(pair.A,translates[pair.A->id].rotation);
                rawPolys[pair.A->id]=GeometryUtil::rotatePolygon2Polygon(rawPolys[pair.A->id],translates[pair.A->id].rotation);
            }
            if(!GeometryUtil::almostEqual(translates[pair.B->id].rotation,0))
            {
                pair.B=GeometryUtil::rotatePolygon2Polygon(pair.B,translates[pair.B->id].rotation);
                rawPolys[pair.B->id]=GeometryUtil::rotatePolygon2Polygon(rawPolys[pair.B->id],translates[pair.B->id].rotation);
            }
            if(!(GeometryUtil::almostEqual(translates[pair.A->id].shift.x,0)&&
                 (GeometryUtil::almostEqual(translates[pair.A->id].shift.y,0))))
            {
                for(auto &v:*pair.A)
                {
                    v.x+=translates[pair.A->id].shift.x;
                    v.y+=translates[pair.A->id].shift.y;
                }
                for(auto &v:*rawPolys[pair.A->id])
                {
                    v.x+=translates[pair.A->id].shift.x;
                    v.y+=translates[pair.A->id].shift.y;
                }
            }
            if(!(GeometryUtil::almostEqual(translates[pair.B->id].shift.x,0)&&
                  (GeometryUtil::almostEqual(translates[pair.B->id].shift.y,0))))
             {
                 for(auto &v:*pair.B)
                 {
                     v.x+=translates[pair.B->id].shift.x;
                     v.y+=translates[pair.B->id].shift.y;
                 }
                 for(auto &v:*rawPolys[pair.B->id])
                 {
                     v.x+=translates[pair.B->id].shift.x;
                     v.y+=translates[pair.B->id].shift.y;
                 }
             }
        }

        std::vector<npPolygonPtr> rets;
#if 1   //矩形摆放
        QMap<QString,npRectangle> rects;
        for(auto &pair:pairs)
        {
            auto tmp(*pair.A);
            tmp.insert(tmp.end(),pair.B->begin(),pair.B->end());
            npRectangle rect=GeometryUtil::getPolygonBounds(tmp);
            rects[QString(pair.key().c_str())]=rect;
        }
        std::vector<rbp::RectSize> brects;
        for(auto key:rects.keys())
        {
            rbp::RectSize br;
            br.data=key.toStdString();
            br.width=rects[key].w;
            br.height=rects[key].h;
            brects.push_back(br);
        }
        rbp::SkylineBinPack bin;
        bin.Init(config.binWidth,config.binHeight,true);
        std::vector<rbp::Rect> resultRects;
        bin.Insert(brects,resultRects,rbp::SkylineBinPack::LevelBottomLeft);

        for(auto rect:resultRects)
        {
            std::string key=rect.size.data;
            const npRectangle&rawRect=rects[QString(key.c_str())];
            npPair &pair=pairs[QString(key.c_str())];
            npPolygonPtr A=rawPolys[pair.A->id];
            npPolygonPtr B=rawPolys[pair.B->id];
            if(rect.size.width!=rawRect.w)
            {//rotate 90 degrees, todo: 在装箱过程中记录确切的数值
                A=GeometryUtil::rotatePolygon2Polygon(A,90);
                B=GeometryUtil::rotatePolygon2Polygon(B,90);
            }
            npPolygon tmp(*A);
            tmp.insert(tmp.end(),B->begin(),B->end());
            npRectangle rrect=GeometryUtil::getPolygonBounds(tmp);
            npVector offset(rect.x-rrect.x,rect.y-rrect.y);

            for(auto &v:*A)
            {
                v.x+=offset.x;
                v.y+=offset.y;
            }
            for(auto &v:*B)
            {
                v.x+=offset.x;
                v.y+=offset.y;
            }
            rets.push_back(pair.A);
            rets.push_back(pair.B);
        }
#else
        for(auto pair:pairs)
        {
//            npPolygonPtr a=GeometryUtil::rotatePolygon2Polygon(pair.A,90);
//            npPolygonPtr b=GeometryUtil::rotatePolygon2Polygon(pair.B,90);
            rets.push_back(a);
            rets.push_back(b);
        }
#endif
        return rets;
    }


};
}
#endif // CTEST_HPP
