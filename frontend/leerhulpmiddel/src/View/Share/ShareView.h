#pragma once
#include "../../Controller/ShareController.h"
#include "../Overlay.h"


#include <QWidget>


class ShareView : public QWidget
{
	Q_OBJECT
public:
	ShareView(ShareController* controller, QWidget* parent = nullptr);
	~ShareView();
private:
	ShareController* m_shareController;
	Overlay* m_overlay;
public slots:
	void showShareCode(QString code);
};

