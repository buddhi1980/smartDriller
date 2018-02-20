#ifndef SMARTDRILLERMAINWINDOW_H
#define SMARTDRILLERMAINWINDOW_H

#include <QMainWindow>

class QuestionsDatabase;

namespace Ui {
  class SmartDrillerMainWindow;
}

class SmartDrillerMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit SmartDrillerMainWindow(QWidget *parent = 0);
  ~SmartDrillerMainWindow();

private slots:
  void on_pushButton_random_question_clicked();

private:
  void CreateDefaultDatabase();

  Ui::SmartDrillerMainWindow *ui;
  QuestionsDatabase *database;
  QString databaseFileName;
};

#endif // SMARTDRILLERMAINWINDOW_H
