#pragma once
#include "../../Controller/ShareController.h"
#include "../Overlay.h"
#include "../chooseQuestionView.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QListWidget>


class ShareView : public QWidget
{
	Q_OBJECT
public:
	ShareView(ShareController* controller, QWidget* parent = nullptr);
	~ShareView();
private:
	void setUpQuestionSetChooser();

	void getFriendUsernames() { m_shareController->getFriendUsernames(); }

	ShareController* m_shareController;
	Overlay* m_overlay;
	QVBoxLayout* m_layout;
	QPushButton* m_shareButton;
	ChooseQuestionView* m_chooseQuestionView;

	QListWidget* m_friendsList;

public slots:
	void showShareCode(QString code);
	void showShareFailed();
	void closeView() { this->close(); };
};

