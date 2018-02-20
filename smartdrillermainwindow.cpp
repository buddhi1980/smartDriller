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
	statusBar()->showMessage(tr("Loaded %1 questions").arg(database->GetNumberOfQuestions()));

	actualQuestionIndex = -1;
	lastQuestionIndex = -1;
}

SmartDrillerMainWindow::~SmartDrillerMainWindow()
{
	delete ui;
	delete database;
}

void SmartDrillerMainWindow::on_pushButton_random_question_clicked()
{
	SelectRandomQuestion();
	QString question = database->GetQuestion(actualQuestionIndex).question;
	ui->label_question->setText(question);
}

void SmartDrillerMainWindow::CreateDefaultDatabase()
{
	if (databaseFileName.length() == 0)
	{
		QString defaultDir = QDir::homePath() + QDir::separator() + "smartDriller";
		if (!QDir(defaultDir).exists())
		{
			QDir().mkdir(defaultDir);
		}
		databaseFileName = defaultDir + QDir::separator() + "database.txt";
	}
}

void SmartDrillerMainWindow::SelectRandomQuestion()
{
	int count = database->GetNumberOfQuestions();

	// randomize randomness :-)
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());

	QuestionsDatabase::sQuestionData record;
	int index = 0;

	bool next = true;
	if (count > 2)
	{
		do
		{
			index = qrand() % count;
			record = database->GetQuestion(index);

			if (qrand() % record.repeatPeriod == 0 && index != actualQuestionIndex)
			{
				next = false;
			}

		} while (next);
		lastQuestionIndex = actualQuestionIndex;
		actualQuestionIndex = index;
	}
}

void SmartDrillerMainWindow::on_lineEdit_answer_returnPressed()
{
}
