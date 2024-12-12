#include "ShareView.h"
#include "../chooseQuestionView.h"
#include "../ToastMessage.h"


#include <qpushbutton.h>

ShareView::ShareView(ShareController* controller, QWidget* parent) : QWidget(parent), m_shareController{controller}, m_overlay{new Overlay(parent)}
{

	setWindowFlags(Qt::Popup);
	setAttribute(Qt::WA_DeleteOnClose);

	int popupWidth = parent->width() * 0.5;
	int popupHeight = parent->height() * 0.8;
	setFixedSize(popupWidth, popupHeight);

	QPoint parentCenter = parent->rect().center();
	QPoint globalCenter = parent->mapToGlobal(parentCenter);
	move(globalCenter.x() - width() / 2, globalCenter.y() - height() / 2);

	QVBoxLayout* layout = new QVBoxLayout(this);
	QPushButton* shareButton = new QPushButton("Share selected questionsets", this);


	ChooseQuestionView* chooseQuestionView = new ChooseQuestionView(true,true);

	m_overlay->resizeToParent();
	m_overlay->show();

	layout->addWidget(chooseQuestionView);
	layout->addWidget(shareButton);


	connect(shareButton, &QPushButton::pressed, this, [=] {

		QList<QString> questionSetPaths = chooseQuestionView->getQuestionSetPaths();
		if (questionSetPaths.size() == 0) {
			ToastMessage* toast = new ToastMessage("You must select at least one question set", this);
			toast->setFixedWidth(400);
			toast->move((width() - toast->width()) / 2, height() - 70);
			toast->show();
		}
		m_shareController->shareQuestionSets(questionSetPaths);
		});
}


void ShareView::showShareCode(QString code)
{
	ToastMessage* toast = new ToastMessage("Share code: " + code, this);
	toast->setFixedWidth(400);
	toast->move((width() - toast->width()) / 2, height() - 70);
	toast->show();
}

void ShareView::showShareFailed()
{
	ToastMessage* toast = new ToastMessage("Failed to share question sets, Something Went Wrong", this);
	toast->setFixedWidth(400);
	toast->move((width() - toast->width()) / 2, height() - 70);
	toast->show();
}

ShareView::~ShareView()
{
	m_overlay->deleteLater();
}



