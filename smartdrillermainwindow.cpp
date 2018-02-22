#include "smartdrillermainwindow.h"
#include "questionsdatabase.h"
#include "ui_smartdrillermainwindow.h"

#include <QInputDialog>
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

	if (database->GetNumberOfQuestions() > 2)
	{
		NextQuestion();
	}
}

SmartDrillerMainWindow::~SmartDrillerMainWindow()
{
	delete ui;
	delete database;
}

void SmartDrillerMainWindow::on_pushButton_random_question_clicked()
{
	NextQuestion();
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
			int repeatPeriod = record.repeatPeriod;
			if (!record.lastGood)
			{
				repeatPeriod /= 2;
				if (repeatPeriod < 1) repeatPeriod = 1;
			}

			if (qrand() % repeatPeriod == 0 && index != actualQuestionIndex)
			{
				next = false;
			}

		} while (next);
		lastQuestionIndex = actualQuestionIndex;
		actualQuestionIndex = index;
	}
}

void SmartDrillerMainWindow::NextQuestion()
{
	SelectRandomQuestion();
	QuestionsDatabase::sQuestionData record;
	record = database->GetQuestion(actualQuestionIndex);
	ui->label_question->setText(record.question);

	QColor color(Qt::black);
	if (record.repeatPeriod < 10)
		color = QColor(255, 0, 0);
	else if (record.repeatPeriod < 40)
		color = QColor(255, 100, 0);
	else if (record.repeatPeriod < 80)
		color = QColor(255, 255, 0);
	else if (record.repeatPeriod < 160)
		color = QColor(255, 255, 255);
	else if (record.repeatPeriod < 320)
		color = QColor(0, 255, 0);

	QPalette palette = ui->label_question->palette();
	palette.setColor(ui->label_question->foregroundRole(), color);
	ui->label_question->setPalette(palette);

	ui->lineEdit_answer->clear();
}

void SmartDrillerMainWindow::UpdateStatistics()
{
	int count = database->GetNumberOfQuestions();
	int score = 0;
	int good = 0;
	int bad = 0;
	int learnedCount = 0;

	QuestionsDatabase::sQuestionData record;
	for (int i = 0; i < count; i++)
	{
		record = database->GetQuestion(i);
		score += record.repeatPeriod - 1;
		good += record.goodAnswers;
		bad += record.badAnswers;
		if (record.lastGood) learnedCount++;
	}
	double percent = double(good) / (good + bad) * 100.0;
	double learned = double(learnedCount) / count * 100.0;

	QString summaryString = tr("%1% total good answers, %2% already learned sentences, score: %3")
														.arg(percent, 0, 'g', 4)
														.arg(learned, 0, 'g', 4)
														.arg(score / 10);
	statusBar()->showMessage(summaryString);
}

void SmartDrillerMainWindow::on_lineEdit_answer_returnPressed()
{
	QString answerEntered = ui->lineEdit_answer->text();
	QuestionsDatabase::sQuestionData record = database->GetQuestion(actualQuestionIndex);
	QString correctAnswer = record.answer;

	// compare and count wrong letters
	int differentLetters = 0;
	for (int i = 0; i < answerEntered.length(); i++)
	{
		if (i < correctAnswer.length())
		{
			if (answerEntered.at(i).toLower() != correctAnswer.at(i).toLower())
			{
				differentLetters++;
			}
		}
	}
	differentLetters += abs(correctAnswer.length() - answerEntered.length());

	QString resultText;
	if (differentLetters == 0)
	{
		resultText = tr("Correct answer!");
		record.goodAnswers++;
		record.repeatPeriod = record.repeatPeriod * 1.5 + 1.0;
		record.lastGood = true;
	}
	else if (differentLetters == 1)
	{
		resultText = tr("Wrong, but you were so close");
		record.badAnswers++;
		record.repeatPeriod /= 1;
		record.lastGood = false;
	}
	else if (differentLetters == 2)
	{
		resultText = tr("Wrong. Your answer was quite similar");
		record.badAnswers++;
		record.repeatPeriod /= 1.5;
		record.lastGood = false;
	}
	else if (differentLetters == 3)
	{
		resultText = tr("Wrong. Your answer was too different");
		record.badAnswers++;
		record.repeatPeriod /= 2;
		record.lastGood = false;
	}
	else if (differentLetters > 3)
	{
		resultText = tr("Wrong. Your answer was totally bad");
		record.badAnswers++;
		record.repeatPeriod /= 3;
		record.lastGood = false;
	}
	if (record.repeatPeriod < 1) record.repeatPeriod = 1;

	ui->label_result->setText(resultText);
	if (differentLetters > 0)
	{
		ui->label_correct_answer->setText(tr("Should be: %1").arg(correctAnswer));
	}
	else
	{
		ui->label_correct_answer->setText("");
	}

	database->UpdateQuestion(record, actualQuestionIndex);
	UpdateStatistics();
	NextQuestion();
}

void SmartDrillerMainWindow::on_pushButton_add_question_clicked()
{
	bool ok;
	QString question = QInputDialog::getText(
		this, tr("Adding new question"), tr("Question:"), QLineEdit::Normal, "", &ok);
	if (ok && !question.isEmpty())
	{
		QString answer = QInputDialog::getText(
			this, tr("Adding new question"), tr("Answer:"), QLineEdit::Normal, "", &ok);

		if (ok && !answer.isEmpty())
		{
			QuestionsDatabase::sQuestionData record;
			record.answer = answer;
			record.question = question;
			record.badAnswers = 0;
			record.goodAnswers = 0;
			record.lastGood = false;
			record.repeatPeriod = 10;
			database->AddQuestion(record);
			statusBar()->showMessage(tr("Added question #%1").arg(database->GetNumberOfQuestions()));
		}
	}
}
