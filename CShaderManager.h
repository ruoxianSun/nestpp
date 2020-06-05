#ifndef CSHADERMANAGER_H
#define CSHADERMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include "Shader.h"
class CShaderManager : public QObject
{
    Q_OBJECT
public:
    static CShaderManager* Instance();
    ~CShaderManager();
    Shader::Ptr getShader(const QString&name);
    static Shader::Ptr shader(const QStringList&files);
    static Shader::Ptr getShader(const QStringList &files);

signals:

public slots:
protected:
    explicit CShaderManager(QObject *parent = nullptr);
    Shader::Ptr loadShader(const QString&name);
protected:
    static CShaderManager *_instance;
    QMap<QString,Shader::Ptr> _shaders;
};

#endif // CSHADERMANAGER_H
