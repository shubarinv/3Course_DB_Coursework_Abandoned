//
// Created by vhund on 01.03.2021.
//

#ifndef DB_QT_COURSEWORK_MAINWINDOW_HXX
#define DB_QT_COURSEWORK_MAINWINDOW_HXX

#include "Functions.hpp"
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
        resize(screenSize.first / 2, screenSize.second / 2);

        setupTopBar(*app);

        servers = serverManager.getServers();

        statusBar()->showMessage(tr("Ready"));
        if (servers.isEmpty()) {
            statusBar()->showMessage(tr("No servers defined! Please open settings(FIle->Settings)."));
        } else {
            statusBar()->showMessage(tr("Server list loaded; Will now try to connect"));
            setupConnections();
        }

        setupMainWidget();

        drawMainMenu();
    }
    void terminateConnections() {
        spdlog::info("Terminating active connections");
        for (auto &connection : active_connections) {
            auto tmp = std::async(std::launch::async, terminateConnection, &connection);
        }
        active_connections.clear();
    }

private:
    void openSettings() {
        SettingsDialog settings_win(&serverManager);
        settings_win.exec();
        servers = serverManager.getServers();
        terminateConnections();
        setupConnections();
    }

    QList<Server> servers;
    std::list<std::future<pqxx::connection *>> active_connections;
    ServerManager serverManager;
    QWidget *mainWidget{};
    QGridLayout *gridLayout{};


    void drawMainMenu() {
        clearWidgetsForLayoutSwitch();
        gridLayout = new QGridLayout();

        //initializing buttons
        auto suppliers_btn = new QPushButton("Поставщики");
        auto checkForm_btn = new QPushButton("Формирование чека");
        auto report_btn = new QPushButton("Отчёт");
        auto contracts_btn = new QPushButton("Договора");
        auto merchandise_btn = new QPushButton("Товары");
        auto shipments_btn = new QPushButton("Поставки");
        auto help_btn = new QPushButton("Справка");

        // Font size related manipulations
        auto font = suppliers_btn->font();
        font.setPixelSize(20);

        // increasing buttons font size
        suppliers_btn->setFont(font);
        checkForm_btn->setFont(font);
        report_btn->setFont(font);
        contracts_btn->setFont(font);
        merchandise_btn->setFont(font);
        shipments_btn->setFont(font);
        help_btn->setFont(font);

        gridLayout->addWidget(suppliers_btn, 0, 0);
        gridLayout->addWidget(checkForm_btn, 0, 1);
        gridLayout->addWidget(report_btn, 0, 2);
        gridLayout->addWidget(contracts_btn, 1, 0);
        gridLayout->addWidget(merchandise_btn, 1, 1);
        gridLayout->addWidget(shipments_btn, 1, 2);
        gridLayout->addWidget(help_btn, 2, 1);

        mainWidget->setLayout(gridLayout);
    }

    void clearWidgetsForLayoutSwitch() {
        while (mainWidget->findChildren<QWidget *>().count() > 0) {
            delete mainWidget->findChildren<QWidget *>().at(0);
        }
        if (mainWidget->layout()) {
            QLayoutItem *p_item;
            while ((p_item = mainWidget->layout()->takeAt(0)) != nullptr)
                delete p_item;
            delete mainWidget->layout();
        }
        gridLayout = {nullptr};
    }

    void setupMainWidget() {
        mainWidget = new QWidget();
        setCentralWidget(mainWidget);
        mainWidget->show();
    }

    void setupTopBar(QApplication &app) {
        auto *quit_act = new QAction(tr("&Quit"), this);
        auto *settings_act = new QAction(tr("&Settings"), this);

        QMenu *menu = menuBar()->addMenu("File");

        // Adding actions to topBar
        menu->addAction(settings_act);
        menu->addAction(quit_act);


        menuBar()->setNativeMenuBar(false);// fixing messed up menu on MacOS


        // setting up button connections
        connect(settings_act, &QAction::triggered, &app, [this]() {
            openSettings();
        });
        connect(quit_act, &QAction::triggered, &app, QApplication::quit);
    }
    static QString constructConnectionString(Server &server) {
        return "host= " + server.host + " user=" + server.user + " port= " + server.port + " dbname= " + server.db + " password= " + server.password + " connect_timeout= 4";
    }

    static pqxx::connection *tryConnectingToServer(const QString &connectString) {
        pqxx::connection *connection{nullptr};
        try {
            connection = new pqxx::connection(connectString.toStdString());
        } catch (const std::exception &e) {
            spdlog::error(e.what());
            return nullptr;
        }
        spdlog::info(std::string("Connected to: ") + connection->username() + "@" + connection->hostname() + ":" + connection->port() + "/" + connection->dbname());
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
            spdlog::info("Trying: " + ServerManager::constructServerListString(srv).toStdString());
            active_connections.push_back(std::async(std::launch::async, tryConnectingToServer, constructConnectionString(srv)));
        }
    }

    static void terminateConnection(std::future<pqxx::connection *> *connection) {
        if (!vh::checkIfAsyncTaskFinished(*connection)) {
            spdlog::info("Awaiting connection");
            connection->wait();
        }
        spdlog::info("closing connection");
        if (connection->get() != nullptr) {
            connection->get()->close();
        }
    }
};


#endif//DB_QT_COURSEWORK_MAINWINDOW_HXX
