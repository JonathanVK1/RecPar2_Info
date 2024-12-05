#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "muParser.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString filePath = "C:/Users/Jony/Desktop/parcial 2 de info2/vi_3.dat";
        readData(filePath);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readData(const QString &filePath){

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No se pudo abrir el archivo.";
    }

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);
    int i=0;
    QVector <double>time;
    QVector <double>volt;
    QVector <double>current;
    double ang,vrms,irms,P;
    while (!in.atEnd()) {
        //qDebug()<<"si se abrio";

        double values;
            in >> values; // Leer un double
            if(i==0){
               time.append(values);
            }
            if(i==1){
                volt.append(values);
            }
            if(i==2){
                current.append(values);
            }
            i++;
            if(i==3){
                i=0;
            }

        }
        int dat=time.size();
        file.close();
        time.resize(dat);
        volt.resize(dat);
        current.resize(dat);

        ui->graphic->addGraph();
        ui->graphic->graph(0)->setData(time,volt);
        ui->graphic->graph(0)->setPen(QPen(Qt::green));
        ui->graphic->graph(0)->setBrush(QBrush(Qt::blue));

        ui->graphic->addGraph();
        ui->graphic->graph(1)->setData(time,current);
        ui->graphic->graph(1)->setPen(QPen(Qt::yellow));
        ui->graphic->rescaleAxes();


        ui->graphic->replot();

        irms=PRMS(current,time);
        vrms=PRMS(volt,time);

        ang=ANG(current,time);
        ang=M_PI*ang/180;

        P=vrms*irms*cos(ang);

        qDebug()<<"PRMS " << P;

        ui->Vrms->setText(QString::number(vrms));

        ui->Irms->setText(QString::number(irms));

        ui->Pot->setText(QString::number(P));
}

double MainWindow::PRMS(QVector <double>current,QVector <double>time){

    int cM=posM(current);
    int cm=posm(current);


    double z=0;
    for (int i=cM;i<=cm;i++) {
        z+=current[i]*current[i]*(time[i+1]-time[i]);
    }
    z=z/(time[cm]-time[cM]);
    z=sqrt(z);

    return z;

}

double MainWindow::ANG(QVector <double>current,QVector <double>time){

    double curM=0;
    int i=0;
    while(curM>=current[i]){
        i++;
    }
    curM=(-2*current[i-1]*time[i-1]+current[i-1]*time[i]+current[i]*time[i-1])/(current[i-1]-current[i]);
    return curM;
}


int MainWindow::posM(QVector<double> val){
    double d=0,c=val[0];
    int i=0;
    for(double vor:val){
        if(c<=vor)
            c=vor;
    }
    while(d!=c){
        d=val[i];
        i++;
    }
    return i-1;
}

int MainWindow::posm(QVector<double> val){
    double d=0,c=val[0];
    int i=0;
    for(double vor:val){
        if(c>=vor)
            c=vor;
    }
    while(d!=c){
        d=val[i];
        i++;
    }
    return i-1;
}
