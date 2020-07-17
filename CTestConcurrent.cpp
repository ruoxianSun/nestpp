#include "CTestConcurrent.h"


#include <QtConcurrent/QtConcurrent>
#include <QFuture>

struct STest{
    STest(){}
    std::vector<int> data;
    void test(){
        for(int i=0;i<30;i++)
        {
            data.push_back(i);
        }
    }
};
CTestConcurrent::CTestConcurrent()
{
    QVector<STest> seq(10000000);

    auto timTest=[&](QString msg,std::function<void()> cb)
    {
        qDebug()<<"test : "<<msg;
        QTime time;
        time.start();
        cb();
        qDebug()<<"speend time "<<time.elapsed()*0.001f<<" s.";
    };
    timTest("normal",[&](){
       for(auto n:seq)
       {
           n.test();
       }
    });
    timTest("map",[&](){

        QFuture<void> r=QtConcurrent::map(seq,[&](STest&t){
            t.test();
        });
        r.waitForFinished();
    });

}
