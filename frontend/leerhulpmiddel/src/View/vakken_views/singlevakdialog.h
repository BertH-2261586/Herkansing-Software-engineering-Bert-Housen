
#ifndef SINGLEVAKDIALOG_H
#define SINGLEVAKDIALOG_H


#include <QObject>
#include <QDialog>
#include <QSettings>
#include <QLayout>

#include "../../model/manager/vakkenmanager.h"
#include "../../Controller/vakkenmanagercontroller.h"

class SingleVakDialog : public QDialog
{
    Q_OBJECT
public:
    SingleVakDialog(VakkenManager* vakkenManager, Vak *vak);

private:
    void BuildLayout();
    void ClearLayout(QLayout *layout);

    void showScoreDialog(OnlineExamen exam);

    Vak* m_vak;
    VakkenManager* m_vakkenManager;
    VakkenManagerController m_vmanagerController;
    QVBoxLayout* m_mainLayout;

public slots:
    void rebuild();
};

#endif // SINGLEVAKDIALOG_H
