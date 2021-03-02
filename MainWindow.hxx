//
// Created by vhund on 01.03.2021.
//

#ifndef DB_QT_COURSEWORK_MAINWINDOW_HXX
#define DB_QT_COURSEWORK_MAINWINDOW_HXX

#include "settingsDialog.hxx"
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <iostream>

class MainWindow : public QMainWindow {
    Q_OBJECT;

public:
    explicit MainWindow(QPair<int, int> screenSize, QApplication *app = nullptr) {
        setFixedSize(screenSize.first / 2, screenSize.second / 2);
        auto *quit_act = new QAction(tr("&Quit"), this);
        auto *settings_act = new QAction(tr("&Settings"), this);

        QMenu *menu = menuBar()->addMenu("File");
        menu->addAction(settings_act);
        menu->addAction(quit_act);


        connect(settings_act, &QAction::triggered, app, []() {
            openSettings();
        });
        connect(quit_act, &QAction::triggered, app, QApplication::quit);
    }

private:
    static void openSettings() {
        SettingsDialog settings_win;
        settings_win.exec();
    }
};


#endif//DB_QT_COURSEWORK_MAINWINDOW_HXX
