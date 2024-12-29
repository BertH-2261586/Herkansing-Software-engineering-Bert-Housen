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


    TestNetworkManager networkTest;
    QTest::qExec(&networkTest, argc, argv);

    // Create the main window
    QuestionManager qManager;
    QuestionManagerController* questionManagerController = new QuestionManagerController(qManager);

    LeerhulpmiddelMainWindow mainWindow(questionManagerController);
    
    mainWindow.show();
    
    return app.exec();
}

