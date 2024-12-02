#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>


class Answer{
public:
	Answer(QList<QString> answers, QList<int> correctAnswers = QList<int>{}) : m_answer{ answers }, m_correctAnswers{correctAnswers} {}

	// Getters
	int getAmountAnswers() const { return m_answer.size(); }
	QList<QString> getCorrectAnswers() const;
	QList<int> getIndicesCorrectAnswers() const;
	QList<QString> getAnswers() const { return m_answer; }

	QJsonObject toJson() const;

	bool operator==(const Answer& other) const;
	bool operator!=(const Answer& other) const { return !(*this == other); }

private:
	QList<QString> m_answer;
	QList<int> m_correctAnswers;
};

