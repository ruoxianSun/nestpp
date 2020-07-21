#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ctest.hpp"
using namespace nest;

class Form;
class CGlWidget;
class CGeometry;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent*e);
    void paintEvent(QPaintEvent*e);
private:
    Ui::MainWindow *ui;
    std::vector<npPolygon::Ptr> polygons,nfps;
    std::shared_ptr<CGlWidget> gls;
public:
    std::vector<std::shared_ptr<CGeometry>> geoms;
};

#endif // MAINWINDOW_H
