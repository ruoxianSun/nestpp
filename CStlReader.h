#ifndef CSTLREADER_H
#define CSTLREADER_H

#include <vector>
#include <QString>
#include <QFile>
#include "CCore.h"
using namespace std;

class CStlReader
{
public:
    using delegateCallback=std::function<void(int,int)>;
    CStlReader();
    CGeom* readModel(const QString &file,delegateCallback dcb);

private:
    CGeom* readerSTL(QString filename,delegateCallback dcb);
    CGeom* readerAscii(QString filename);
    CGeom* readerBinary(QString filename,delegateCallback dcb);
    float readFloat(QFile &file);
    QString readString(QFile &file,int n);
    unsigned int readUInt32(QFile &file);
    unsigned short readUInt16(QFile &file);

};

#endif // CSTLREADER_H
