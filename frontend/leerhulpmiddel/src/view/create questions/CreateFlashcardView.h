#pragma once

#include <QWidget>
#include <QTextEdit>

class CreateFlashcardView : public QWidget {
public:
	CreateFlashcardView(QWidget* parent = nullptr);
private:
	QTextEdit* m_txtQuestion;
	QTextEdit* m_txtAnswer;
};

