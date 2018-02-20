#include "questionsdatabase.h"

#include <QFile>
#include <QMessageBox>
#include <QtCore>

QuestionsDatabase::QuestionsDatabase() {}

void QuestionsDatabase::LoadDatabase(const QString &filename)
{
  QFile file(filename);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream stream(&file);

    QString header = stream.readLine();
    if(header == "#SmartDriller")
    {
      data.clear();
      QString line;
      while (!stream.atEnd())
      {
        sRecord record;
        line = stream.readLine();
        record.question = line;
        line = stream.readLine();
        record.answer = line;
        line = stream.readLine();
        QStringList results = line.split(" ");
        if(results.length() == 4)
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
            NULL, "File error",
            QString("File %1 is not a SmartDriller database").arg(filename));
    }

  }
  else
  {
    QMessageBox::critical(
          NULL, "File error",
          QString("Cannot open file %1 with database").arg(filename));
  }
}
