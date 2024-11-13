//#include "view/leerhulpmiddelmainwindow.h"
#include "model/questionmanager.h"
#include <QApplication>
#include <QMainWindow>

#include "model/fileManager.h"
#include "model/Questions/MultipleChoiceQuestion.h"
#include "view/create questions/CreateQuestionView.h"
#include "model/questionset.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QuestionManager qManager;

    //LeerhulpmiddelMainWindow mainWindow(qManager);
    //mainWindow.show();

    QMainWindow mainWindow = QMainWindow();
    mainWindow.show();

    //TEMP
    CreateQuestionView* createView = new CreateQuestionView(&mainWindow);

    //Center
    int x = mainWindow.geometry().x() + (mainWindow.width() / 2) + (createView->width() / 2);
    int y = mainWindow.geometry().y() + (mainWindow.height() / 2) + (createView->height() / 2);
    createView->move(x, y);

    /*
    try{
    createView->show();

        FileManager FileManager;
        QList<Questionset*> questionsets = FileManager.loadQuestionSetsObject();
        qDebug() << questionsets;
        FileManager.makeQuestionSet("placeholder", "testingfolder");
        MultipleChoiceQuestion multiquestion = MultipleChoiceQuestion("wiskunde", "wat is 2+2", Answer(QList<QString>{"test","test2"}, QList<int> {1}));
        FileManager.saveQuestionToJSON("test", "", multiquestion);
        unique_ptr<Question> loadedquestion = FileManager.loadQuestionFromJSON("test", "", "wiskunde");
        qDebug() << loadedquestion->getName() << " " << loadedquestion->getQuestion() << " " << loadedquestion->getAnswer().getAnswers();
    }
    catch (const FolderQuestionSetMovedException& e) {
        qDebug() << "an error has occured: " << e.what();
    }
    catch (const filesystem::filesystem_error& e) {
        qDebug() << "an error has occured: " << e.what();
    }
    catch (const saveException& e) {
        qDebug() << "an error has occured: " << e.what();
    }
    catch (const loadException& e) {
        qDebug() << "an error has occured: " << e.what();
    }
    */
    return app.exec();
}

