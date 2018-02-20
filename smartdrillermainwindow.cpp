#include "smartdrillermainwindow.h"
#include "ui_smartdrillermainwindow.h"

SmartDrillerMainWindow::SmartDrillerMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::SmartDrillerMainWindow)
{
  ui->setupUi(this);
}

SmartDrillerMainWindow::~SmartDrillerMainWindow()
{
  delete ui;
}
