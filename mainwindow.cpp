#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataholder.h"
#include "datatable.h"
#include "runge.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->uGraphicsView->addGraph();
    ui->iGraphicsView->addGraph();
    ui->rGraphicsView->addGraph();

    ui->uGraphicsView->xAxis->setLabel("t, с");
    ui->uGraphicsView->yAxis->setLabel("U, В");

    ui->iGraphicsView->xAxis->setLabel("t, с");
    ui->iGraphicsView->yAxis->setLabel("I, А");

    ui->rGraphicsView->xAxis->setLabel("t, с");
    ui->rGraphicsView->yAxis->setLabel("R, Ом");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculatePushButton_clicked()
{
    DataHolder data;
    data.Ck = ui->ckDoubleSpinBox->value();
    data.I0 = ui->i0DoubleSpinBox->value();
    data.l = ui->lDoubleSpinBox->value();
    data.Lk = ui->lkDoubleSpinBox->value();
    data.P0 = ui->p0DoubleSpinBox->value();
    data.R = ui->rDoubleSpinBox->value();
    data.Rk = ui->rkDoubleSpinBox->value();
    data.Tn = ui->tDoubleSpinBox->value();
    data.UC0 = ui->uc0DoubleSpinBox->value();

    DataTable tableVoltage("data/tableVoltage.txt");
    DataTable tableConductivity("data/tableConductivity.txt");
    DataTable tableITM("data/tableITM.txt");

    double max_t = 300e-6;
    double step = 1e-6;

    Runge solver(data, tableVoltage, tableConductivity, tableITM);
    DataTable result(solver.calculate(0, max_t, step));

    QVector<double> time;
    for (double t = 0; t < max_t; t += step) {
        time.push_back(t);
    }

    ui->iGraphicsView->graph(0)->addData(time, result.getRow(0));
    ui->uGraphicsView->graph(0)->addData(time, result.getRow(1));
    ui->rGraphicsView->graph(0)->addData(time, result.getRow(2));

    ui->iGraphicsView->rescaleAxes();
    ui->uGraphicsView->rescaleAxes();
    ui->rGraphicsView->rescaleAxes();

    ui->iGraphicsView->replot();
    ui->uGraphicsView->replot();
    ui->rGraphicsView->replot();
}
