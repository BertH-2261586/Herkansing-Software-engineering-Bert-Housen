#include <QApplication>
#include <QMainWindow>
#include <QSettings>

#include <QtTest/QtTest>
#include "tests/TestNetworkManager.h"

#include "Controller/questionmanagercontroller.h"
#include "model/manager/questionmanager.h"
#include "view/leerhulpmiddelmainwindow.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Set the color palette of the main background
    QPalette currentPalette = app.palette();
    QColor baseColor = currentPalette.color(QPalette::Window);
    QColor lighterColor = baseColor.lighter(110);
    app.setStyleSheet(
        QString("QMainWindow { background-color: %1; }").arg(lighterColor.name())
    );

    TestNetworkManager networkTest;
    QTest::qExec(&networkTest, argc, argv);

    // Create the main window
    QuestionManager qManager;
    QuestionManagerController* questionManagerController = new QuestionManagerController(qManager);

    LeerhulpmiddelMainWindow mainWindow(questionManagerController);
    
    mainWindow.show();
    
    return app.exec();
}

