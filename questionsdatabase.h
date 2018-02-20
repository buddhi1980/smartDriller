#ifndef QUSTIONSDATABASE_H
#define QUSTIONSDATABASE_H

#include <QList>
#include <QString>

class QuestionsDatabase
{

private:
  struct sRecord
  {
    QString question;
    QString answer;
    int repeatPeriod;
    int goodAnswers;
    int badAnswers;
    bool lastGood;
  };

public:
  QuestionsDatabase();

  void LoadDatabase(const QString &filename);

private:
  QList<sRecord> data;
};

#endif // QUSTIONSDATABASE_H
