#include "CreateFillInQuestionView.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "BracketHighlighter.h"

CreateFillInQuestionView::CreateFillInQuestionView(QWidget* parent) {	
	QPushButton* btnInsertAnswer = new QPushButton(this);
	btnInsertAnswer->setText("Insert answer at cursor position");

	QLabel* lblQuestion = new QLabel(this);
	lblQuestion->setText("Question: ");

	m_txtQuestion = new QTextEdit(this);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(btnInsertAnswer);
	mainLayout->addWidget(lblQuestion);
	mainLayout->addWidget(m_txtQuestion);

	setLayout(mainLayout);

	connect(btnInsertAnswer, &QPushButton::pressed,
		this, &CreateFillInQuestionView::insertAnswer);

	BracketHighlighter* highlighter = new BracketHighlighter(m_txtQuestion->document());	//Highlights answers
}


/*
* Inserts a blank space at current cursor position and creates a lineedit to insert an answer
*/
void CreateFillInQuestionView::insertAnswer() {
	QTextCursor cursor = m_txtQuestion->textCursor();

	cursor.insertText("[insert answer here]");

	m_txtQuestion->setTextCursor(cursor);
}