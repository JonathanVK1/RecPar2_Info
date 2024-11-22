#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "muParser.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time.resize(100);
    volts.resize(100);
    current.resize(100);

    QString filePath = "C:/Users/Jony/Desktop/parcial 2 de info2/vi_1.dat";
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

    QVector<double>timer;
    QVector<double>voltss;
    QVector<double>currents;
    double vmax,vmin,imin,imax;
    int d=0,g=0,f=0,h=0,dat=0;
        int i=0;
        while (!in.atEnd()) {
            //qDebug()<<"si se abrio";

            double values;
            in >> values; // Leer un double
            if(i==0){
               timer.append(values);
               dat++;
            }
            if(i==1){
                voltss.append(values);
                if(f==0){
                    vmin=values;
                    f=1;
                }
                else{
                    if(vmin>=values){
                        vmin=values;
                    }
                }
                if(h==0){
                    vmax=values;
                    h=1;
                }
                else{
                    if(vmax<=values){
                        vmax=values;
                    }
                }

            }
            if(i==2){
                currents.append(values);
                if(d==0){
                    imin=values;
                    d=1;
                }
                else{
                    if(imin>=values){
                        imin=values;
                    }
                }
                if(g==0){
                    imax=values;
                    g=1;
                }
                else{
                    if(imax<=values){
                        imax=values;
                    }
                }

            }
            i=i+1;
            if(i==3){
                i=0;
            }

        }

        file.close();
        timer.resize(dat);
        voltss.resize(dat);
        currents.resize(dat);
        i=0;
            // Imprimir los valores
        qDebug()<<"tiempo";
        for (double val : timer) {
            qDebug() <<"t ="<< val;
            timer[i]=val;
            i=i+1;
        }
        i=0;
        qDebug()<<"potencia";
        for (double val : volts) {
            qDebug() <<"V ="<< val;
            volts[i]=val;
        }
        i=0;
        for (double val:current) {
            qDebug()<<"I="<< val;
            currents[i]=val;
        }
        qDebug()<< "Imax="<<imax<<"Vmax="<<vmax;

        qDebug()<< "Imin="<<imin<<"Vmin="<<vmin;

        ui->graphic->addGraph();
        ui->graphic->graph(0)->setData(timer,voltss);

        ui->graphic->addGraph();
        ui->graphic->graph(1)->setData(timer,currents);

        ui->graphic->rescaleAxes();
        ui->graphic->replot();

    PRMS(current,time,volts,dat);
}

void MainWindow::PRMS(QVector<double> &current, QVector<double> &time, QVector<double> &volts,int dat){

    int f=0,h=0,d=0,c=0,i=0,pp=0;
    double vmin,vmax,imin,imax,tvmin,tvmax,timin,timax;
    double vrms[dat],irms[dat],trms[dat];


    for(double val:volts){
        vrms[pp]=val;

        if(f==0){
            vmin=val;
            f=1;
        }
        else{
            if(vmin>=val){
                vmin=val;
                f=f+1;
            }
        }
        if(h==0){
            vmax=val;
            h=1;
        }
        else{
            if(vmax<=val){
                vmax=val;
                h=h+1;
            }
        }
        pp++;
    }
    pp=0;
    for(double val:time){
        trms[pp]=val;
        if(d==h){
            tvmin=val;
        }
        d++;
        if(c==f){
            tvmax=val;
        }
        c++;
    }
    f=0;h=0;d=0;c=0,pp=0;
    for(double val:current){
        irms[pp]=val;
        if(f==0){
            imin=val;
            f=1;
        }
        else{
            if(imin>=val){
                imin=val;
                f=f+1;
            }
        }
        if(h==0){
            imax=val;
            h=1;
        }
        else{
            if(imax<=val){
                imax=val;
                h=h+1;
            }
        }
    }
    for(double val:time){
        if(d==h){
            timin=val;
        }
        d++;
        if(c==f){
            timax=val;
        }
        c++;
    }
    double x_supr=1e-9;
    double xmin=vrms[1];
    double ymin=irms[1];
    double tcero=trms[0];
    int w=0;
    double icero=irms[0];
    double angulo;
    while(xmin>=0){
        xmin=vrms[w];
        w++;
    }
    int jj=0;
    while(ymin<=0){
        ymin=irms[jj];
        jj++;
    }

    while(icero<=0){
        icero=icero+x_supr;
        tcero=tcero+x_supr;
    }

    angulo= 180/(0.01*tcero);
    double angulor;
    angulor=angulo*M_PI/180;




    double PRms,VRms,IRms,imms,iMms,vmms,vMms,tvmid,timid;

    vmms=pow(vmax,2)/2;
    imms=pow(imax,2)/2;
    iMms=pow(imin,2)/2;
    vMms=pow(vmin,2)/2;
    tvmid=1/(tvmax-tvmin);
    timid=1/(timax-timin);
    VRms=sqrt((vmms-vMms)/tvmid);
    IRms=sqrt((imms-iMms)/timid);

    PRms=VRms*IRms*cos(angulor);


    qDebug()<< "VRms = "<<VRms<< "IRms = "<<IRms<<"PRms = "<<PRms;


}
