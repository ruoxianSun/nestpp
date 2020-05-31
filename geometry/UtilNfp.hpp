#ifndef CNFP_H
#define CNFP_H
#include <Primitive.hpp>
#include <UtilGeometry.hpp>
#include <NestPolygon.hpp>
namespace nest {

struct NfpKey{
    int A,B;
    bool inside;
    double Arotation,Brotation;
    NfpKey(){}
    NfpKey(int a,int b,bool is,double ar,double br):A(a),B(b),inside(is),Arotation(ar),Brotation(br){}
    NfpKey&operator=(const NfpKey&key){
        A=key.A;B=key.B;inside=key.inside;
        Arotation=key.Arotation;Brotation=key.Brotation;
        return *this;
    }
    std::string operator()(){
        std::stringstream ss;
        ss<<A<<","<<B;
        std::string buf;
        ss>>buf;
        return buf;
    }
};
template<typename PathType>
struct NfpPair{
    using path_type=typename PathType::type;
    using path_ptr=typename path_type::Ptr;
    using key_type=NfpKey;
    using type=NfpPair;
    path_ptr A,B;
    key_type key;
    NfpPair():A(0),B(0){}
    NfpPair(path_ptr a,path_ptr b,key_type k):A(a),B(b),key(k){}
};
template<typename PathType>
struct NfpConfig{
    using type=NfpConfig;
    using path_type=typename PathType::type;
    using value_type=typename path_type::value_type;

    bool concave,usehole;
    value_type binWidth,binHeight;
    value_type spaceing;
};
template<typename PathType>
struct NfpParalle{
    using type=NfpParalle;
    using path_type=typename PathType::type;
    using path_ptr=typename path_type::Ptr;
    NfpKey key;
    std::vector<path_ptr> value;
};
template<typename Pair,typename Util>
struct UtilNfp{
    using pair_type=typename Pair::type;
    using path_type=typename pair_type::path_type;
    using key_type=typename pair_type::key_type;
    using util_type=typename Util::type;
    using rect_type=typename util_type::rect_type;
    using paralle_type=typename NfpParalle<path_type>::type;
    using path_ptr=typename path_type::Ptr;
    using type=UtilNfp;
    using config_type=typename NfpConfig<path_type>::type;
    static paralle_type nfpGenerator(pair_type pair, config_type config)
    {
        using Geometry =typename UtilGeometry<path_type>::type;
        using Common =typename UtilCommon<path_type,Geometry>::type;
        bool searchEdges = config.concave;
        bool useHoles = config.usehole;

        path_ptr A = util_type::rotatePolygon2Polygon(pair.A, pair.key.Arotation);
        path_ptr B = util_type::rotatePolygon2Polygon(pair.B, pair.key.Brotation);

        std::vector<path_ptr> nfp;
        if (pair.key.inside)
        {
            if (util_type::isRectangle(*A, 0.001))
            {
                nfp =util_type::noFitPolygonRectangle(*A, *B);
            }
            else
            {
                nfp = util_type::noFitPolygon(*A, *B, true, searchEdges);
            }
            if (nfp.size() > 0)
            {
                for (int i = 0; i < nfp.size(); i++)
                {
                    if (util_type::polygonArea(*nfp[i]) > 0)
                    {
                        nfp[i]->reverse();
                    }
                }
            }
            else
            {
                //Warning on null inner NFP
            }
        }
        else
        {
            int count = 0;
            if (searchEdges)
            {

                // NFP Generator TODO  double scale contorl
                nfp = util_type::noFitPolygon(*A, *B, false, searchEdges);
            }
            else
            {
                nfp = Common::minkowskiDifference(*A, *B);
            }
            // sanity check
            if (nfp.size() == 0)
            {
                return paralle_type();
            }
            for (auto item=nfp.begin();item!=nfp.end();item++)// int i = 0; i < nfp.size(); i++)
            {
                if (!searchEdges || item == nfp.begin())
                {
                    if (std::abs(util_type::polygonArea(**item)) < std::abs(util_type::polygonArea(*A)))
                    {
                        nfp.erase(item);
                        return paralle_type();
                    }
                }
            }
            if (nfp.size() == 0)
            {
                return paralle_type();
            }

            for (int i = 0; i < nfp.size(); i++)
            {
                if (util_type::polygonArea(*nfp[i]) > 0)
                {
                    nfp[i]->reverse();
                }

                if (i > 0)
                {
                    if (1== util_type::pointInPolygon((*nfp[i])[0], *nfp[0]))
                    {
                        if (util_type::polygonArea(*nfp[i]) < 0)
                        {
                            nfp[i]->reverse();
                        }
                    }
                }
            }

            if (useHoles && A->children.size() > 0)
            {
                rect_type Bbounds = util_type::getPolygonBounds(*B);
                for (int i = 0; i < A->children.size(); i++)
                {
                    rect_type Abounds = util_type::getPolygonBounds(*A->children[i]);

                    if (Abounds.w > Bbounds.w && Abounds.h > Bbounds.h)
                    {

                        std::vector<path_ptr> cnfp = util_type::noFitPolygon(*A->children[i], *B, true, searchEdges);
                        // ensure all interior NFPs have the same winding direction

                        if (cnfp.size() > 0)
                        {

                            for (int j = 0; j < cnfp.size(); j++)
                            {
                                if (util_type::polygonArea(*cnfp[j]) < 0)
                                {
                                    cnfp[j]->reverse();
                                }
                                nfp.push_back(cnfp[j]);
                            }
                        }

                    }
                }
            }
        }
        return {pair.key, nfp};
    }
};
}

#endif // CNFP_H
