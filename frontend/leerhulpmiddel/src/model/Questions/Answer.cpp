#include "Answer.h"
#include <QJsonArray>

/**
 * Returns the list of answers that are correct
 * @return QList<QString> list of correct answers
 */
QList<QString> Answer::getCorrectAnswers() const
{
	if (m_correctAnswers.isEmpty()) {
		return m_answer;
	}

	QList<QString> correctAnswers;
	for (int index : m_correctAnswers) {
		if (index >= 0 && index < m_answer.size()) {  // Check if index is in range
			correctAnswers.append(m_answer.at(index));
		}
	}
	return correctAnswers;
}

/**
 * Returns the index of the correct answer
 * @return QList<int> list of indices of the correct answers
 */
QList<int> Answer::getIndicesCorrectAnswers() const
{
	QList<int> correctAnswers;
	for (int index : m_correctAnswers) {
		if (index >= 0 && index < m_answer.size()) {  // Check if index is in range
			correctAnswers.append(index);
		}
	}
	return correctAnswers;
}

/**
 * Overloads the == operator to compare two Answer objects
 * @param other Answer object to compare with
 * @return bool true if the two objects are equal, false otherwise
 */
bool Answer::operator==(const Answer& other) const
{
	if (m_correctAnswers.isEmpty()) { // If no correct answers are given (not a multiple choice Questions), compare the answers
		return m_answer == other.m_answer;
	}

	if (m_correctAnswers.size() != other. m_answer.size()) {
		return false;
	}

	QList<QString> correctAnswers = getCorrectAnswers();
	for (QString m_answer : other.m_answer)
	{
		if (!correctAnswers.contains(m_answer)) {
			return false;
		}
	}

	return true;
}

/**
 * Converts the Answer object to a QJsonObject
 * @return QJsonObject the Answer object as a QJsonObject
 */

QJsonObject Answer::toJson() const {
	QJsonObject jsonObject;
	QJsonArray answersArray;
	QJsonArray correctAnswersArray;

	for (const QString& answer : m_answer) {
		answersArray.append(answer);
	}

	for (int correctAnswer : m_correctAnswers) {
		correctAnswersArray.append(correctAnswer);
	}

	jsonObject["answers"] = answersArray;
	jsonObject["correctAnswers"] = correctAnswersArray;

	return jsonObject;
}