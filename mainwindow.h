#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ctest.hpp"
using namespace nest;

class Form;
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
    Form*form;
    std::vector<npPolygon::Ptr> polygons,nfps;
};

#endif // MAINWINDOW_H
