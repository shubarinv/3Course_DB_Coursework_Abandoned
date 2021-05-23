#include "MainWindow.hxx"
#include <QApplication>
#include <QFile>

#include <QScreen>
#include <QStyleFactory>
#include <Qpair>


QString readStyleSheetFromFile(const QString &filename);
QPair<int, int> getScreenSize();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("fusion"));

    // Dark mode related
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(31, 31, 31));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(79, 79, 79));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    QApplication::setPalette(darkPalette);

    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    MainWindow mainWindow(getScreenSize(), &app);
    mainWindow.show();
    QApplication::exec();
    mainWindow.terminateConnections();
    return 0;
}


QString readStyleSheetFromFile(const QString &filename) {
    QFile file(filename);
    file.open(QFile::ReadOnly);
    return QLatin1String(file.readAll());
}


QPair<int, int> getScreenSize() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    return {width, height};
}
