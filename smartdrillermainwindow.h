#ifndef SMARTDRILLERMAINWINDOW_H
#define SMARTDRILLERMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class SmartDrillerMainWindow;
}

class SmartDrillerMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit SmartDrillerMainWindow(QWidget *parent = 0);
  ~SmartDrillerMainWindow();

private:
  Ui::SmartDrillerMainWindow *ui;
};

#endif // SMARTDRILLERMAINWINDOW_H
