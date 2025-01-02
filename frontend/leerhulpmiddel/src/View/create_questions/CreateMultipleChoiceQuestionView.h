#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QBoxLayout>
#include <QString>

#include "../../model/Questions/Answer.h"

class CreateMultipleChoiceQuestionView : public QWidget {
public:
	CreateMultipleChoiceQuestionView(QWidget* parent = nullptr);

	QString getQuestion() const;
	Answer getAnswer();
private:
	void updateRemoveButtonState();
	void addAnswer();
	QTextEdit* m_txtQuestion;
	QBoxLayout* m_layout;
};

