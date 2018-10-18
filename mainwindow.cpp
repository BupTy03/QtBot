#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("QtBot");

    connect(ui->LoginBtn, SIGNAL(released()), ui->LoginAction, SLOT(trigger()));
    connect(ui->NewTaskBtn, SIGNAL(released()), ui->NewTaskAction, SLOT(trigger()));
    connect(ui->ExitBtn, SIGNAL(released()), ui->ExitAction, SLOT(trigger()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ExitAction_triggered()
{
    QApplication::quit();
}

void MainWindow::on_NewTaskAction_triggered()
{
    AddTaskWindow addTskWin;
    addTskWin.setModal(true);

    if(!addTskWin.exec())
    {
        return;
    }

    ui->statusBar->showMessage(QString("Interval: ") + QString::number(addTskWin.getInterval()) +
                               QString("\tPeriod: ") + QString::number(addTskWin.getPeriod()));
}
