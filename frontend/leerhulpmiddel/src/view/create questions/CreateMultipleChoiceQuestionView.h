#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QBoxLayout>

class CreateMultipleChoiceQuestionView : public QWidget {
public:
	CreateMultipleChoiceQuestionView(QWidget* parent = nullptr);
private:
	void addAnswer();

	QTextEdit* m_txtQuestion;
	QBoxLayout* m_layout;
};

