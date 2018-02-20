#include "smartdrillermainwindow.h"
#include "questionsdatabase.h"
#include "ui_smartdrillermainwindow.h"

#include <QtCore>

SmartDrillerMainWindow::SmartDrillerMainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::SmartDrillerMainWindow)
{
  ui->setupUi(this);
  database = new QuestionsDatabase;

  QSettings settings("SmartDriller", "SmartDriller");

  databaseFileName = settings.value("databaseLocation").value<QString>();
  CreateDefaultDatabase();
  settings.setValue("databaseLocation", databaseFileName);

  database->LoadDatabase(databaseFileName);
}

SmartDrillerMainWindow::~SmartDrillerMainWindow()
{
  delete ui;
  delete database;
}

void SmartDrillerMainWindow::on_pushButton_random_question_clicked() {}

void SmartDrillerMainWindow::CreateDefaultDatabase()
{
  if (databaseFileName.length() == 0) {
    QString defaultDir = QDir::homePath() + QDir::separator() + "smartDriller";
    if (!QDir(defaultDir).exists()) {
      QDir().mkdir(defaultDir);
    }
    databaseFileName = defaultDir + QDir::separator() + "database.txt";
  }
}
