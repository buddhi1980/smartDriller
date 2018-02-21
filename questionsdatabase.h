#ifndef QUESTIONSDATABASE_H
#define QUESTIONSDATABASE_H

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
	void AddQuestion(const sQuestionData &record);
	void UpdateQuestion(const sQuestionData &record, int index);

private:
	QString databaseFileName;
	void SaveDatabase();
	QList<sQuestionData> data;
};

#endif // QUESTIONSDATABASE_H
