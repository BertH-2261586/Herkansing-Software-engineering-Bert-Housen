#include "CreateMultipleChoiceQuestionView.h"
#include "../ToastMessage.h"
#include "../../Exceptions/EmptyFieldException.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QScrollArea>
#include <QMessageBox>
#include <QStringList>
#include <QCheckBox>

CreateMultipleChoiceQuestionView::CreateMultipleChoiceQuestionView(QWidget* parent) : QWidget(parent), m_txtQuestion{ new QTextEdit(this)} {


	QLabel* lblQuestion = new QLabel(this);
	lblQuestion->setText("Question: ");

	QPushButton* btnAddAnswer = new QPushButton("Add an Answer",this);

	connect(btnAddAnswer, &QPushButton::clicked, this, &CreateMultipleChoiceQuestionView::addAnswer);


	m_layout = new QVBoxLayout(this);
	m_layout->addStretch();



	QWidget* contentWidget = new QWidget(this);
	contentWidget->setLayout(m_layout);

	

	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setWidget(contentWidget);  // Set the scrollable content widget
	scrollArea->setWidgetResizable(true);
	scrollArea->setFrameShape(QFrame::NoFrame);  // No frame around the scroll area
	scrollArea->setFrameShadow(QFrame::Plain);

	QVBoxLayout* outerLayout = new QVBoxLayout(this);
	outerLayout->addWidget(lblQuestion);
	outerLayout->addWidget(m_txtQuestion);
	outerLayout->addWidget(btnAddAnswer);
	outerLayout->addWidget(scrollArea);
	setLayout(outerLayout);

	//add 2 answer slots in the beginning
	addAnswer();
	addAnswer();
}


/**
* @brief Get the question from the text edit
*/
QString CreateMultipleChoiceQuestionView::getQuestion() const {
	if (m_txtQuestion->toPlainText().isEmpty())
	{
		throw EmptyFieldException("Please fill in the question");
	}
	return m_txtQuestion->toPlainText();
}

/**
* Creates an answer object from the answers and correct answers in the view
* @return Answer object: a list of answers and a list of the indexes of the correct answers
* @throws EmptyFieldException if an answer is empty
*/
Answer CreateMultipleChoiceQuestionView::getAnswer(){

	QList<QString> answers;
	QList<int> correctAnswers;

	int count = 0;

	for (QObject* answer : m_layout->children())
	{
		QHBoxLayout* answerLayout = qobject_cast<QHBoxLayout*>(answer);
		QString input = qobject_cast<QTextEdit*>(answerLayout->itemAt(0)->widget())->toPlainText();

		if (input.isEmpty())
		{
			throw EmptyFieldException("Please fill in all the answer options");
		}

		answers.append(input);

		if (qobject_cast<QCheckBox*>(answerLayout->itemAt(2)->widget())->isChecked())
		{
			correctAnswers.append(count); //add the index of the correct answer
		}
		count++;


	}
	if (correctAnswers.isEmpty())
	{
		throw EmptyFieldException("At least 1 answer must be selected as correct");
	}
	return Answer(answers,correctAnswers);
}




/**
* Adds an answer dield to the view Creates a toast message if the maximum amount of answers is reached
*/
void CreateMultipleChoiceQuestionView::addAnswer() {

	if (m_layout->count() - 1 > 15)
	{
		ToastMessage* toast = new ToastMessage("You can only add 15 answers", this);
		toast->move((width() - toast->width()) / 2, height() - 70);  // Position near the bottom center
		toast->show();
	}
	else
	{

		QHBoxLayout* answerLayout = new QHBoxLayout;

		QTextEdit* txtAnswer = new QTextEdit(this);
		txtAnswer->setPlaceholderText("Enter an answer");
		txtAnswer->setFixedHeight(50);


		QPushButton* removeButton = new QPushButton(this);

		removeButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));

		QCheckBox* correctCheckBox = new QCheckBox(this);


		answerLayout->addWidget(txtAnswer);
		answerLayout->addWidget(removeButton);
		answerLayout->addWidget(correctCheckBox);



		m_layout->addLayout(answerLayout);


		connect(removeButton, &QPushButton::clicked, [this, answerLayout]() {
			m_layout->removeItem(answerLayout);
			QLayoutItem* child;
			while ((child = answerLayout->takeAt(0)) != nullptr) {
				delete child->widget(); // delete the widget
				delete child;   // delete the layout item
			}
			delete answerLayout;
			updateRemoveButtonState();
			});

		updateRemoveButtonState();
		update();

	}
}

/**
* Updates the state of the remove buttons if 2 or less answers disables the deletion of answers
*/
void CreateMultipleChoiceQuestionView::updateRemoveButtonState()
{

	bool enableRemove = m_layout->count() > 3;  // 2 answer fields + 1 stretch

	for (QObject* child : m_layout->children()) { 
		QHBoxLayout* answerLayout = qobject_cast<QHBoxLayout*>(child); 
		if (answerLayout) {
			QPushButton* removeButton = qobject_cast<QPushButton*>(answerLayout->itemAt(1)->widget()); 
			if (removeButton) { 
				removeButton->setEnabled(enableRemove);
			}
		}
	}
}