#include "questionsdatabase.h"

#include <QFile>
#include <QMessageBox>
#include <QtCore>

QuestionsDatabase::QuestionsDatabase(QObject *parent) : QObject(parent)
{
}

void QuestionsDatabase::LoadDatabase(const QString &filename)
{
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		QString header = stream.readLine();
		if (header == "#SmartDriller")
		{
			databaseFileName = filename;
			data.clear();
			QString line;
			while (!stream.atEnd())
			{
				sQuestionData record;
				line = stream.readLine();
				if (line.length() == 0) break; // check if last line
				record.question = line;

				line = stream.readLine();
				record.answer = line;

				line = stream.readLine();
				QStringList results = line.split(" ");
				if (results.length() == 4)
				{
					record.badAnswers = results.at(0).toInt();
					record.goodAnswers = results.at(1).toInt();
					record.repeatPeriod = results.at(2).toInt();
					record.lastGood = results.at(3).toInt();
				}
				data.append(record);
			}
		}
		else
		{
			QMessageBox::critical(
				nullptr, tr("File error"), tr("File %1 is not a SmartDriller database").arg(filename));
		}

		file.close();
	}
	else
	{
		QMessageBox::critical(
			nullptr, tr("File error"), tr("Cannot open file %1 with database").arg(filename));
	}
}

const QuestionsDatabase::sQuestionData &QuestionsDatabase::GetQuestion(int index) const
{
	if (index < data.length())
	{
		return data[index];
	}
}

void QuestionsDatabase::AddQuestion(const QuestionsDatabase::sQuestionData &record)
{
	data.append(record);
	SaveDatabase();
}

void QuestionsDatabase::SaveDatabase()
{

	QFile file(databaseFileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << "#SmartDriller" << endl;

		for (int i = 0; i < data.length(); i++)
		{
			sQuestionData record = data.at(i);
			stream << record.question << endl;
			stream << record.answer << endl;

			QString stats = QString("%1 %2 %3 %4")
												.arg(record.badAnswers)
												.arg(record.goodAnswers)
												.arg(record.repeatPeriod)
												.arg(record.lastGood);
			stream << stats << endl;
		}
	}
	file.close();
}
