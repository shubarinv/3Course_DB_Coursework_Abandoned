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
#include <QStatusBar>
#include <iostream>
#include <pqxx/connection>

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
        menuBar()->setNativeMenuBar(false);
        connect(settings_act, &QAction::triggered, app, []() {
            openSettings();
        });
        connect(quit_act, &QAction::triggered, app, QApplication::quit);
        statusBar()->showMessage(tr("Ready"));
        SettingsDialog::loadServers(&servers);
        if (servers.isEmpty()) {
            statusBar()->showMessage(tr("No servers defined! Please open settings(FIle->Settings)."));
        } else {
            statusBar()->showMessage(tr("Server list loaded; Will now try to connect"));
            for (auto &srv : servers) {
                tryConnectingToServer(constructConnectionString(&srv));
            }
        }
    }

private:
    static void openSettings() {
        SettingsDialog settings_win;
        settings_win.exec();
    }
    QList<Server> servers;
    static QString constructConnectionString(Server *server) {
        return "host= " + server->host + " user=" + server->user + " port= " + server->port + " dbname= " + server->db + " password= " + server->password;
    }
    static bool tryConnectingToServer(const QString &connectString) {
        try {
            auto conn = new pqxx::connection(connectString.toStdString());
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
        return true;
    }
};


#endif//DB_QT_COURSEWORK_MAINWINDOW_HXX
