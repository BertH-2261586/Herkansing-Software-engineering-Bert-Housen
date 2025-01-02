#include "QuestionDisplay.h"
#include <QVBoxLayout>
#include <QLabel>


QuestionDisplay::QuestionDisplay(Question* question)
{

	// Set the layout
	
	QVBoxLayout* layout = new QVBoxLayout();
	setLayout(layout);

	// Set the question text

	QLabel* questionName = new QLabel("Name: " + question->getName());
	layout->addWidget(questionName);

	QLabel* questionText = new QLabel("Question: " + question->getQuestion());
	layout->addWidget(questionText);

	// Set the answer text
	QLabel* answerText = new QLabel("Answer: " + question->getAnswer().getCorrectAnswers().join("\n"));
	layout->addWidget(answerText);

}
