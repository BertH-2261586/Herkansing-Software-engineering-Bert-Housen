#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Qt 6.8 Boilerplate Window");
    mainWindow.resize(800, 600); // Set initial window size
    mainWindow.show();

    return app.exec();
}
