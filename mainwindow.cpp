#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myopenglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_2_triggered()
{
    MainWindow::close();
}


void MainWindow::on_action_triggered()
{
    ui->widget->GameLevel = 0;
    ui->widget->gameNew = false;
    ui->widget->control.Delete();
}

