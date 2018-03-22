#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataholder.h"
#include "datatable.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->uGraphicsView->addGraph();
    ui->iGraphicsView->addGraph();
    ui->rGraphicsView->addGraph();
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

}
