#pragma once

#include <QWidget>
#include <QTextEdit>

class CreateFillInQuestionView : public QWidget {
	Q_OBJECT
public:
	CreateFillInQuestionView(QWidget* parent = nullptr);
private:
	QTextEdit* m_txtQuestion;
private slots:
	void insertAnswer();
};


