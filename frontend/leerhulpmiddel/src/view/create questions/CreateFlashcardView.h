#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QString>

#include "../../model/Questions/Answer.h"

class CreateFlashcardView : public QWidget {
public:
	CreateFlashcardView(QWidget* parent = nullptr);

	QString getQuestion() const;
	Answer getAnswer() const;
private:
	QTextEdit* m_txtQuestion;
	QTextEdit* m_txtAnswer;
};

