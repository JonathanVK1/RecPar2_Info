#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include "muParser.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    x.resize(10);
    y.resize(10);
    a.resize(10);
    connect(ui->Graficar,
            SIGNAL(clicked()),
            this,
            SLOT(ploter()));

    connect(ui->Savedata,
            SIGNAL(clicked()),
            this,
            SLOT(save()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ploter(){
    // definir valores de x

    double xmin= ui->Xmin->text().toDouble();
    double xmax= ui->Xmax->text().toDouble();
    double dx=1e-4;
    double xpr= ui->Xbcero->text().toDouble();
    double xsc= ui->Xbuno->text().toDouble();
    double diff=1e-4,xmiddle,ymiddle;
    double xprmin=xmin,xscmax=xmin,H1=0,H2=0;
    int g=0,gg=0,tab=0;
    //double delta=0;

    int N=(xmax-xmin)/dx;
    x.resize(N);
    y.resize(N);

    for (int i=0;i<N;i++) {
        x[i]=xmin+double(i)*dx;
    }
    parser.DefineVar(L"x",x.data());


    //darle valor a F(x)

    parser.SetExpr(ui->Fx->text().toStdWString());
    parser.Eval(y.data(),y.size());

    double ymin=f(xpr);
    double ymax=f(xsc);

    if(ymin*ymax<0){
        while(xsc-xpr>diff){

        xmiddle=(xpr+xsc)/2;
        ymiddle=f(xmiddle);
        if(ymin*ymiddle<0){
            xsc=xmiddle;

        }
        else{
            xpr=xmiddle;
            ymin=f(xpr);
        }
        }
        tab=1;
    }
    else{
        qDebug()<<"no hay raices en este intervalo o hay mas de una";
    }

    if(tab==1){
        qDebug()<<"xmiddle "<<xmiddle;
        ui->xmiddle->setText(QString::number(xmiddle));
    }



    while (xprmin<=xpr) {
        xprmin=xprmin+dx;
        g++;
    }
    while (xscmax<=xsc) {
        xscmax=xscmax+dx;
        gg++;

    }
    int h=gg-g;
    a.resize(h);
    for (int i=g;i<gg;i++) {
       // delta=x[i-1]-x[i];
        //a+=y[i]*dx;
        H1=y[i+1]-y[i];
        H2=y[i];
        a[i]=((dx*H1)/2)+(dx*H2);


    }

    double X=ui->Xncero->text().toDouble();

    for (int i=0;i<100;i++) {
        double fx=f(X);
        double dfx=df(X);
        double x_next=X-(fx/dfx);
        X=x_next;
    }

    qDebug()<<"raiz newton"<<X;
    ui->xraiz->setText(QString::number(X));

    // cantidad de graficos
    //if(ui->graphic->graphCount()==0){
    ui->graphic->addGraph();
    ui->graphic->graph(0)->setData(x,y);

    ui->graphic->addGraph();
    ui->graphic->graph(1)->setData(x,a);

    ui->graphic->rescaleAxes();
    ui->graphic->replot();
    //}
    //calcular y realizar el grafico del area debajo de la curva con un boton

}

double MainWindow:: df(double x){

    double dx=1e-9;
    double dy=f(x+dx)-f(x);
    return dy/dx;

}

double MainWindow:: f(double x){

    mu::Parser parser;
    parser.DefineVar(L"x",&x);
    parser.SetExpr(ui->Fx->text().toStdWString());
    double y=parser.Eval();
    return y;
}


void MainWindow::save(){


    double dx=1e-4;
    int xtotal;
    double xmin=ui->Xmin->text().toDouble();
    double xmax=ui->Xmax->text().toDouble();


    if(xmin>=0){
        xtotal=(xmax+xmin)/dx;
    }
    else{
        xtotal=(xmax-xmin)/dx;
    }
    double Fx[xtotal];

    for(int i=0;i<xtotal;i++){
        Fx[i]=f(xmin);
        xmin+=dx;
    }

    QString filename="datos.txt";

    QFile file(filename);

    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream out(&file);
/*
        for (int i=0;i<xtotal;i++) {

            out<<Fx[i];
            out<< " ";

        }*/



        out<<"hola";
        qDebug()<<"entro aqui";
    }

    file.close();




}
