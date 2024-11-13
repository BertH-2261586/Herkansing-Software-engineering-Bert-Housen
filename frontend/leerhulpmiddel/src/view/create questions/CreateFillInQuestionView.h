#pragma once

#include <QWidget>
#include <QTextEdit>

#include "../../model/Questions/Answer.h"

class CreateFillInQuestionView : public QWidget {
	Q_OBJECT
public:
	CreateFillInQuestionView(QWidget* parent = nullptr);

	QString getQuestion() const;
	Answer getAnswer() const;
private:
	QTextEdit* m_txtQuestion;
private slots:
	void insertAnswer();
};


