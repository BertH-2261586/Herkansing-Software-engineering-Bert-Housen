#pragma once
#include "../../Controller/ShareController.h"
#include "../Overlay.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "../chooseQuestionView.h"


class ShareView : public QWidget
{
	Q_OBJECT
public:
	ShareView(ShareController* controller, QWidget* parent = nullptr);
	~ShareView();
private:
	void setUpQuestionSetChooser();

	ShareController* m_shareController;
	Overlay* m_overlay;
	QVBoxLayout* m_layout;
	QPushButton* m_shareButton;
	ChooseQuestionView* m_chooseQuestionView;
public slots:
	void showShareCode(QString code);
	void showShareFailed();
};

