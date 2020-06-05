#include "CStlReader.h"

#include <QFileInfo>
#include <QDebug>
#include <iostream>
#include <fstream>
CStlReader::CStlReader()
{

}

CGeom * CStlReader::readModel(const QString &filename,delegateCallback dcb)
{
    CGeom * model=Q_NULLPTR;

    QString suffix=QFileInfo(filename).completeSuffix().toLower();
    if (suffix.compare("stl")==0)
        model =readerSTL(filename,dcb);
    else
        qDebug()<<"File type is not stl";
//    model->updateNormal();
    return model;
}

CGeom *CStlReader::readerSTL(QString filename, delegateCallback dcb)
{
    CGeom * model = readerAscii(filename);
    if (model == Q_NULLPTR || model->_vertexs.size()==0)
    {
        model = readerBinary(filename,dcb);
    }
    return model;
}



CGeom *CStlReader::readerAscii(QString filename)
{
    return Q_NULLPTR;
}

CGeom *CStlReader::readerBinary(QString filename, delegateCallback dcb)
{
    CGeom *model = new CGeom();
    QFile file(filename);
    file.open(QFile::ReadOnly);
    QString header= readString(file,80);
    uint32_t faceCount = readUInt32(file);
    int percent=0;
    QMap<glm::vec3,int> indexMap;
    for (size_t i = 0; i < faceCount; i++)
    {
        glm::vec3 normal;
        normal.x = readFloat(file);
        normal.y = readFloat(file);
        normal.z =readFloat(file);
        for (size_t j = 0; j < 3; j++)
        {
            glm::vec3 point;
            point.x = readFloat(file);
            point.y = readFloat(file);
            point.z = readFloat(file);
            model->insertVertex(point);
        }
        readUInt16(file);
        int pos=i*100.0/faceCount;
        if(pos>percent)
        {
           dcb(100,pos);
           percent=pos;
        }
    }
    dcb(100,100);
    file.close();
    return model;
}



float CStlReader::readFloat(QFile &file)
{
    float x;
    file.read((char*)&x, 4);
    return x;
}

QString CStlReader::readString(QFile &file, int n)
{
    std::string str(n, '0');
    file.read((char*)str.c_str(), n);
    return QString::fromStdString(str);
}

unsigned int CStlReader::readUInt32(QFile &file)
{
    unsigned int x;
    file.read((char*)&x, 4);
    return x;
}

unsigned short CStlReader::readUInt16(QFile &file)
{
    unsigned short x;
    file.read((char*)&x, 2);
    return x;
}
