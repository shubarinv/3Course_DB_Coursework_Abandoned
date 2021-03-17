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
#include <future>
#include <iostream>
#include <pqxx/connection>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class MainWindow : public QMainWindow {
    Q_OBJECT;

public:
    explicit MainWindow(QPair<int, int> screenSize, QApplication *app = nullptr) {
        initLog();
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
            setupConnections();
        }
    }

private:
    static void openSettings() {
        SettingsDialog settings_win;
        settings_win.exec();
    }

    QList<Server> servers;
    std::list<std::future<pqxx::connection *>> active_connections;

    static QString constructConnectionString(Server &server) {
        return "host= " + server.host + " user=" + server.user + " port= " + server.port + " dbname= " + server.db + " password= " + server.password + "connect_timeout=4";
    }

    static pqxx::connection *tryConnectingToServer(const QString &connectString) {
        pqxx::connection *connection{nullptr};
        try {
            connection = new pqxx::connection(connectString.toStdString());
        } catch (const std::exception &e) {
            spdlog::error(e.what());
            return nullptr;
        }
        return connection;
    }

    static void initLog() {
        // create color multi threaded logger
        auto console = spdlog::stdout_color_mt("console");
        auto err_logger = spdlog::stderr_color_mt("stderr");
    }
    void setupConnections() {
        spdlog::info("Begin setting up connections to databases");
        for (auto &srv : servers) {
            spdlog::info("Trying: " + SettingsDialog::constructServerListString(&srv).toStdString());
            active_connections.push_back(std::async(std::launch::async, tryConnectingToServer, constructConnectionString(srv)));
        }
    }
};


#endif//DB_QT_COURSEWORK_MAINWINDOW_HXX
