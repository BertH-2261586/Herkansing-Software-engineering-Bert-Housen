
#ifndef ALLVAKKENDIALOG_H
#define ALLVAKKENDIALOG_H


#include <QObject>
#include <QDialog>
#include <QSettings>
#include <QLayout>

#include "../../model/manager/vakkenmanager.h"
#include "../../Controller/vakkenmanagercontroller.h"
#include "singlevakdialog.h"

class AllVakkenDialog : public QDialog
{
    Q_OBJECT
public:
    AllVakkenDialog(VakkenManager* vakkenManager);

private:
    void BuildStudentLayout();
    void BuildDocentLayout();
    void ClearLayout(QLayout *layout);
    void ShowNewVakInput();

    void showSingleVakDialog(Vak* vak);

    VakkenManagerController m_vmanagerController;
    VakkenManager* m_vakkenManager;
    QVBoxLayout* m_mainLayout;

public slots:
    void VakkenChanged();
};

#endif // ALLVAKKENDIALOG_H
