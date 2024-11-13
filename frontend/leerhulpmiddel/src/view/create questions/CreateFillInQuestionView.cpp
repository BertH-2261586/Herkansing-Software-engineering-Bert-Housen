#include "CreateFillInQuestionView.h"
#include "../../Exceptions/EmptyFieldException.h"


#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRegularExpression>

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

QString CreateFillInQuestionView::getQuestion() const {
	if (m_txtQuestion->toPlainText().isEmpty())
	{
		throw EmptyFieldException("Please fill in the question");
	}
	return m_txtQuestion->toPlainText();
}

Answer CreateFillInQuestionView::getAnswer() const {
	QStringList answers;

	QString question = m_txtQuestion->toPlainText();

	//Use regex to get all answers
	QRegularExpression regex = QRegularExpression(R"(\[(.*?)\])");
	QRegularExpressionMatchIterator matches = regex.globalMatch(question);

	//Get all answers
	while (matches.hasNext()) {
		QRegularExpressionMatch match = matches.next();
		answers.append(match.captured(1));
	}

	return Answer(answers);
}


/*
* Inserts a blank space at current cursor position and creates a lineedit to insert an answer
*/
void CreateFillInQuestionView::insertAnswer() {
	QTextCursor cursor = m_txtQuestion->textCursor();

	cursor.insertText("[insert answer here]");

	m_txtQuestion->setTextCursor(cursor);
}