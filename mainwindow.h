#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "muParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Root{
    bool valid;
    double value;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    double data();
    double f(double x);
    double df(double x);


public slots:
    void ploter();
    void save();

private:
    Ui::MainWindow *ui;
    mu::Parser parser;
    QVector<double> x,y,a ;

};
#endif // MAINWINDOW_H
