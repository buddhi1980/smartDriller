#ifndef QUSTIONSDATABASE_H
#define QUSTIONSDATABASE_H

#include <QList>
#include <QString>

class QuestionsDatabase : QObject
{
	Q_OBJECT

public:
	struct sQuestionData
	{
		QString question;
		QString answer;
		int repeatPeriod;
		int goodAnswers;
		int badAnswers;
		bool lastGood;
	};

public:
	QuestionsDatabase(QObject *parent = nullptr);

	void LoadDatabase(const QString &filename);
	int GetNumberOfQuestions() const { return data.length(); }
	const sQuestionData &GetQuestion(int index) const;

private:
	QList<sQuestionData> data;
};

#endif // QUSTIONSDATABASE_H
