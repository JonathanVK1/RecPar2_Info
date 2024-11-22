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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public:
    void readData(const QString &filePath);
    void PRMS(QVector<double>&current,QVector<double>&time,QVector<double>&volts,int dat);

private:
    Ui::MainWindow *ui;
    mu::Parser parser;
    QVector<double> time,volts,current  ;

};
#endif // MAINWINDOW_H
