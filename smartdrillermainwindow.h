#ifndef SMARTDRILLERMAINWINDOW_H
#define SMARTDRILLERMAINWINDOW_H

#include <QMainWindow>

class QuestionsDatabase;

namespace Ui
{
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

	void on_lineEdit_answer_returnPressed();

private:
	void CreateDefaultDatabase();
	void SelectRandomQuestion();

	Ui::SmartDrillerMainWindow *ui;
	QuestionsDatabase *database;
	QString databaseFileName;
	int actualQuestionIndex;
	int lastQuestionIndex;
};

#endif // SMARTDRILLERMAINWINDOW_H
