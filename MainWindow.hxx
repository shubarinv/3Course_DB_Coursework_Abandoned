//
// Created by vhund on 01.03.2021.
//

#ifndef DB_QT_COURSEWORK_MAINWINDOW_HXX
#define DB_QT_COURSEWORK_MAINWINDOW_HXX

#include "Functions.hpp"
#include "data_base_manager.hpp"
#include "input_dialog.hpp"
#include "settingsDialog.hxx"
#include <QApplication>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>
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

        setupMainWidget();

        drawConnectionStatusWidget();
    }
    void terminateConnections() {
    }

private:
    void openSettings() {
        SettingsDialog settings_win(&serverManager);
        settings_win.exec();
        servers = serverManager.getServers();
        terminateConnections();

        drawConnectionStatusWidget();
    }

    QList<Server> servers;
    std::list<std::future<pqxx::connection *>> active_connections;
    ServerManager serverManager;
    DataBaseManager dbManager;
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

        suppliers_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        checkForm_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        report_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        contracts_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        merchandise_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        shipments_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        help_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


        connect(suppliers_btn, &QPushButton::clicked, this, [this]() {
            spdlog::info("Layout switch: mainMenu->suppliersMenu");
            drawSuppliersMenu();
        });
        connect(contracts_btn, &QPushButton::clicked, this, [this]() {
            spdlog::info("Layout switch: mainMenu->contractsMenu");
            drawContractsMenu();
        });
    }

    void drawSuppliersMenu() {
        clearWidgetsForLayoutSwitch();
        gridLayout = new QGridLayout();
        //initializing buttons
        auto back_btn = new QPushButton("<-");
        auto add_btn = new QPushButton("Добавить");
        auto redact_btn = new QPushButton("Редактировать");
        auto delete_btn = new QPushButton("Удалить");

        auto table_widget = new QTableWidget(0, 7, this);
        table_widget->setHorizontalHeaderLabels({"#", "Название", "Адрес", "Телефон", "Почтовый адрес", "Директор", "Реквизиты"});


        add_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        redact_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        delete_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        table_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        gridLayout->addWidget(back_btn, 0, 0);
        gridLayout->addWidget(table_widget, 1, 0, 3, 1);
        gridLayout->addWidget(add_btn, 1, 1);
        gridLayout->addWidget(redact_btn, 2, 1);
        gridLayout->addWidget(delete_btn, 3, 1);

        mainWidget->setLayout(gridLayout);

        // Font size related manipulations
        auto font = back_btn->font();
        font.setPixelSize(20);

        back_btn->setFont(font);
        add_btn->setFont(font);
        redact_btn->setFont(font);
        delete_btn->setFont(font);


        connect(back_btn, &QPushButton::clicked, this, [this]() {
            spdlog::info("Layout switch: suppliersMenu->mainMenu");
            drawMainMenu();
        });

        connect(add_btn, &QPushButton::clicked, this, [this]() {
            InputDialog inputDialog;
            inputDialog.SetTitle("Добавление поставщика");
            inputDialog.AddField("Название:");
            inputDialog.AddField("Адрес:");
            inputDialog.AddField("Телефон:");
            inputDialog.AddField("Почтовый адрес:");
            inputDialog.AddField("Директор:");
            inputDialog.AddField("Реквизиты:");

            inputDialog.exec();
            // by this point we should have gotten the result
            spdlog::info("suppliersMenu::addSupplierDialog returned following values {");
            if (!inputDialog.getResults().empty())
                for (auto &result : inputDialog.getResults()) {
                    spdlog::info("  {}", result.toUtf8().constData());
                }
            spdlog::info("}");
        });
    }

    void drawContractsMenu() {
        clearWidgetsForLayoutSwitch();
        gridLayout = new QGridLayout();
        //initializing buttons
        auto back_btn = new QPushButton("<-");
        auto add_btn = new QPushButton("Добавить");
        auto redact_btn = new QPushButton("Редактировать");
        auto delete_btn = new QPushButton("Удалить");

        auto table_widget = new QTableWidget(0, 5, this);
        table_widget->setHorizontalHeaderLabels({"#", "# Номер договора", "Сумма", "Оплата", "Отгрузка"});


        add_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        redact_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        delete_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        table_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        gridLayout->addWidget(back_btn, 0, 0);
        gridLayout->addWidget(table_widget, 1, 0, 3, 1);
        gridLayout->addWidget(add_btn, 1, 1);
        gridLayout->addWidget(redact_btn, 2, 1);
        gridLayout->addWidget(delete_btn, 3, 1);

        mainWidget->setLayout(gridLayout);

        // Font size related manipulations
        auto font = back_btn->font();
        font.setPixelSize(20);

        back_btn->setFont(font);
        add_btn->setFont(font);
        redact_btn->setFont(font);
        delete_btn->setFont(font);


        connect(back_btn, &QPushButton::clicked, [this]() {
            spdlog::info("Layout switch: contractsMenu->mainMenu");
            drawMainMenu();
        });
    }

    void drawConnectionStatusWidget() {
        clearWidgetsForLayoutSwitch();
        gridLayout = new QGridLayout();

        // By this point servers list should have been loaded.

        if (servers.empty()) {// ask user to add servers
            auto noServersLabel = new QLabel();
            noServersLabel->setText("No servers defined. Please set them at File->Settings");
            noServersLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

            auto font = noServersLabel->font();
            font.setPixelSize(20);
            noServersLabel->setFont(font);

            gridLayout->addWidget(noServersLabel, 0, 0);

        } else {
            auto tableWidget = new QTableWidget(0, 2, this);
            tableWidget->setHorizontalHeaderLabels({"Server", "Status"});
            tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
            tableWidget->verticalHeader()->hide();
            tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

            // filling table with servers and their status
            for (auto &srv : servers) {
                tableWidget->insertRow(tableWidget->rowCount());
                tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(ServerManager::constructServerListString(srv)));

                if (ServerManager::tryConnectingToServer(srv) != nullptr) {
                    tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem("OK"));
                } else {
                    tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem("Bad"));
                }
            }

            tableWidget->resizeColumnsToContents();
            tableWidget->resizeRowsToContents();

            auto selectServer_btn = new QPushButton("Use selected server");
            selectServer_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
            auto refresh_btn = new QPushButton("Refresh");
            refresh_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

            auto font = selectServer_btn->font();
            font.setPixelSize(16);
            selectServer_btn->setFont(font);
            refresh_btn->setFont(font);

            gridLayout->addWidget(tableWidget, 0, 0, 1, 2);
            gridLayout->addWidget(selectServer_btn, 1, 1);
            gridLayout->addWidget(refresh_btn, 1, 0);

            connect(tableWidget, &QTableWidget::clicked, this, [=, this](const QModelIndex &index) {
                spdlog::info("{}:{}", index.row(), tableWidget->item(index.row(), 0)->text().toUtf8().toStdString());
                if (tableWidget->item(index.row(), 1)->text() == "Bad") {
                    tableWidget->clearSelection();
                    spdlog::info("serverSelection: removed selection from {}: BAD SERVER", tableWidget->item(index.row(), 0)->text().toStdString());
                    dbManager.setConnection(nullptr);
                    selectServer_btn->setDisabled(true);
                } else {
                    dbManager.setConnection(ServerManager::tryConnectingToServer(servers[index.row()]));
                    selectServer_btn->setEnabled(true);
                }
            });

            connect(selectServer_btn, &QPushButton::clicked, this, [=, this]() {
                if (dbManager.connection() == nullptr) {
                    return;
                }
                spdlog::info("Layout switch: serverSelection->mainMenu");
                drawMainMenu();
            });

            connect(refresh_btn, &QPushButton::clicked, [=, this]() {
                for (int i = 0; i < servers.size(); ++i) {
                    if (ServerManager::tryConnectingToServer(servers[i]) != nullptr) {
                        tableWidget->setItem(i, 1, new QTableWidgetItem("OK"));
                    } else {
                        tableWidget->setItem(i, 1, new QTableWidgetItem("Bad"));
                    }
                }
            });
        }
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


    // TODO move everything below to some class

    static void initLog() {
        // create color multi threaded logger
        auto console = spdlog::stdout_color_mt("console");
        auto err_logger = spdlog::stderr_color_mt("stderr");
    }

    static void terminateConnection(std::future<pqxx::connection *> *connection) {
        return;
    }
};


#endif//DB_QT_COURSEWORK_MAINWINDOW_HXX
