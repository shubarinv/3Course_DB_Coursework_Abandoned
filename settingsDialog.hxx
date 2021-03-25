//
// Created by vhund on 01.03.2021.
//

#ifndef DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
#define DB_QT_COURSEWORK_SETTINGSDIALOG_HXX

#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardItemModel>
#include <iostream>
#include <spdlog/spdlog.h>
#include "server_manager.hpp"

class SettingsDialog : public QDialog {
    Q_OBJECT


public:
    SettingsDialog() {
        setModal(true);
        setWindowTitle("Settings");
        constructServerSettingsPage();
        servers_initial_values = servers;
    }



private:
    std::unique_ptr<QGridLayout> layout;
    std::unique_ptr<QStandardItemModel> itemModel{};
    std::unique_ptr<QListView> servers_list{};
    std::unique_ptr<QLineEdit> srvHost_inp{};
    std::unique_ptr<QLineEdit> srvPort_inp{};
    std::unique_ptr<QLineEdit> srvLogin_inp{};
    std::unique_ptr<QLineEdit> srvPass_inp{};
    std::unique_ptr<QLineEdit> srvDB_inp{};

    std::unique_ptr<QPushButton> addServer_btn{};
    std::unique_ptr<QPushButton> saveServer_btn{};
    std::unique_ptr<QPushButton> editServer_btn{};
    std::unique_ptr<QPushButton> removeServer_btn{};
    std::unique_ptr<QPushButton> clearData_btn{};

    std::unique_ptr<QLabel> srvHost_label{};
    std::unique_ptr<QLabel> srvPort_label{};
    std::unique_ptr<QLabel> srvLogin_label{};
    std::unique_ptr<QLabel> srvPass_label{};
    std::unique_ptr<QLabel> srvDB_label{};

    std::unique_ptr<QSettings> settings{};
    QList<Server> servers;
    QList<Server> servers_initial_values;
    /**
     * @brief constructs server settings page
     */
    void constructServerSettingsPage() {
        initializeVariables();
        setupComponents();
        setupLayout();
        setupEventConnections();

       //TODO getServers();
        fillServerList();
    }

    void initializeVariables() {
        layout = std::make_unique<QGridLayout>(this);

        srvHost_label = std::make_unique<QLabel>(this);
        srvPort_label = std::make_unique<QLabel>(this);
        srvLogin_label = std::make_unique<QLabel>(this);
        srvPass_label = std::make_unique<QLabel>(this);
        srvDB_label = std::make_unique<QLabel>(this);

        srvHost_inp = std::make_unique<QLineEdit>(this);
        srvPort_inp = std::make_unique<QLineEdit>(this);
        srvLogin_inp = std::make_unique<QLineEdit>(this);
        srvPass_inp = std::make_unique<QLineEdit>(this);
        srvDB_inp = std::make_unique<QLineEdit>(this);
        srvPass_inp->setEchoMode(QLineEdit::Password);

        itemModel = std::make_unique<QStandardItemModel>(this);
        servers_list = std::make_unique<QListView>(this);

        addServer_btn = std::make_unique<QPushButton>(this);
        saveServer_btn = std::make_unique<QPushButton>(this);
        clearData_btn = std::make_unique<QPushButton>(this);
        editServer_btn = std::make_unique<QPushButton>(this);
        removeServer_btn = std::make_unique<QPushButton>(this);
    }

    void setupComponents() const {
        srvHost_label->setText("Host:");
        srvPort_label->setText("Port:");
        srvLogin_label->setText("Login:");
        srvPass_label->setText("Password:");
        srvDB_label->setText("Database:");

        removeServer_btn->setText("Remove");
        removeServer_btn->setObjectName("remove-btn");
        removeServer_btn->setDisabled(true);

        editServer_btn->setText("Edit");
        editServer_btn->setObjectName("add-btn");
        editServer_btn->setDisabled(true);

        clearData_btn->setText("Clear");
        clearData_btn->setObjectName("clear-btn");
        clearData_btn->setDisabled(true);

        saveServer_btn->setText("Save");
        saveServer_btn->setObjectName("add-btn");
        saveServer_btn->setDisabled(true);
        saveServer_btn->setVisible(false);

        addServer_btn->setText("Add");
        addServer_btn->setObjectName("add-btn");
        addServer_btn->setDisabled(true);

        servers_list->setModel(itemModel.get());
        servers_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    void setupLayout() const {

        // inputs and labels
        layout->addWidget(srvHost_label.get(), 2, 0, 1, 1);
        layout->addWidget(srvHost_inp.get(), 2, 1, 1, 1);

        layout->addWidget(srvPort_label.get(), 3, 0, 1, 1);
        layout->addWidget(srvPort_inp.get(), 3, 1, 1, 1);

        layout->addWidget(srvLogin_label.get(), 4, 0, 1, 1);
        layout->addWidget(srvLogin_inp.get(), 4, 1, 1, 1);

        layout->addWidget(srvPass_label.get(), 5, 0, 1, 1);
        layout->addWidget(srvPass_inp.get(), 5, 1, 1, 1);

        layout->addWidget(srvDB_label.get(), 6, 0, 1, 1);
        layout->addWidget(srvDB_inp.get(), 6, 1, 1, 1);

        layout->addWidget(servers_list.get(), 0, 0, 1, 2);
        layout->addWidget(editServer_btn.get(), 1, 0, 1, 1);
        layout->addWidget(removeServer_btn.get(), 1, 1, 1, 1);
        layout->addWidget(clearData_btn.get(), 7, 0, 1, 1);
        layout->addWidget(addServer_btn.get(), 7, 1, 1, 1);
        layout->addWidget(saveServer_btn.get(), 7, 1, 1, 1);
    }


    void setupEventConnections() {
        // connections
        connect(addServer_btn.get(), &QPushButton::clicked, this, [this]() {
            addServer();
        });
        connect(saveServer_btn.get(), &QPushButton::clicked, this, [this]() {
            saveServerEdit();
        });
        connect(removeServer_btn.get(), &QPushButton::clicked, this, [this]() {
            removeServerFromList();
        });
        connect(editServer_btn.get(), &QPushButton::clicked, this, [this]() {
            fillInputFields();
        });
        connect(clearData_btn.get(), &QPushButton::clicked, this, [this]() {
            clearInputFields();
        });
        connect(servers_list.get(), &QListView::clicked, this, [this](const QModelIndex &index) {
            enableEditAndRemoveButtons();
        });
        connect(srvHost_inp.get(), &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvPort_inp.get(), &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvLogin_inp.get(), &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvPass_inp.get(), &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvDB_inp.get(), &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
    }

    /**
     * @brief removes selected(in Vertical List)  server from list
     */
    void removeServerFromList() {
        int i = 0;
        for (auto &srv : servers) {
            if (ServerManager::constructServerListString(srv) == itemModel->index(servers_list->currentIndex().row(), 0).data().toString()) {
                break;
            }
            i++;
        }
        servers.removeAt(i);
        itemModel->removeRow(servers_list->currentIndex().row());
        removeServer_btn->setDisabled(true);
        editServer_btn->setDisabled(true);
    }

    /**
    * @brief clears all input fields
    */
    void clearInputFields() const {
        srvHost_inp->clear();
        srvPort_inp->clear();
        srvLogin_inp->clear();
        srvPass_inp->clear();
        srvDB_inp->clear();
    }

    /**
     * @brief gets data from the input fields and adds it to the server list
     */
    void addServer() {
        Server server;
        server.host = srvHost_inp->text();
        server.port = srvPort_inp->text();
        server.user = srvLogin_inp->text();
        server.password = srvPass_inp->text();
        server.db = srvDB_inp->text();
        servers.push_back(server);
        auto *newServer = new QStandardItem(ServerManager::constructServerListString(server));
        itemModel->appendRow(newServer);
        clearInputFields();
    }

    /**
     * @brief fills vertical list with elements
     */
    void fillServerList() {
        itemModel->clear();
        for (auto &srv : servers) {
            auto *newServer = new QStandardItem(ServerManager::constructServerListString(srv));
            itemModel->appendRow(newServer);
        }
    }


    void saveServerEdit() {
        auto srv = getServerFromString(itemModel->index(servers_list->currentIndex().row(), 0).data().toString());
        addServer_btn->setVisible(true);
        addServer_btn->setDisabled(true);
        saveServer_btn->setVisible(false);
        saveServer_btn->setDisabled(true);
        if (srv == nullptr) {
            spdlog::error("Fascinating... how could this even happen? data in list doesn't correspond to server data in settings! Changes won't be saved");
            clearInputFields();
            return;
        }
        srv->host = srvHost_inp->text();
        srv->user = srvLogin_inp->text();
        srv->password = srvPass_inp->text();
        srv->db = srvDB_inp->text();
        srv->port = srvPort_inp->text();
        clearInputFields();
        fillServerList();
    }
    /**
     * @brief when server selected from Vertical list this function is called to fill input field with it's data
     * @param index index of the element that will be edited
     */
    void fillInputFields() {
        addServer_btn->setVisible(false);
        saveServer_btn->setVisible(true);
        auto srv = getServerFromString(itemModel->index(servers_list->currentIndex().row(), 0).data().toString());
        if (srv == nullptr) {
            spdlog::error("Fascinating... how could this even happen? data in list doesn't correspond to server data in settings! Changes won't be saved");
            clearInputFields();
            return;
        }
        srvHost_inp->setText(srv->host);
        srvPort_inp->setText(srv->port);
        srvLogin_inp->setText(srv->user);
        srvPass_inp->setText(srv->password);
        srvDB_inp->setText(srv->db);
    }

    void enableEditAndRemoveButtons() const {
        clearInputFields();
        removeServer_btn->setEnabled(true);
        editServer_btn->setEnabled(true);
        saveServer_btn->setVisible(false);
        addServer_btn->setVisible(true);
    }
    /**
     * @brief checks completion of form; enables and disables buttons accordingly
     */
    void checkFormFill() const {
        if (srvHost_inp->text().isEmpty() ||
            srvPort_inp->text().isEmpty() ||
            srvLogin_inp->text().isEmpty() ||
            srvPass_inp->text().isEmpty() ||
            srvDB_inp->text().isEmpty()) {
            addServer_btn->setDisabled(true);
            saveServer_btn->setDisabled(true);
        } else {
            addServer_btn->setDisabled(false);
            saveServer_btn->setDisabled(false);
        }
        if (!srvHost_inp->text().isEmpty() ||
            !srvPort_inp->text().isEmpty() ||
            !srvLogin_inp->text().isEmpty() ||
            !srvPass_inp->text().isEmpty() ||
            !srvDB_inp->text().isEmpty()) {
            clearData_btn->setDisabled(false);
        } else {
            clearData_btn->setDisabled(true);
        }
    }



    /**
     * @brief searches for server by serverString
     * @param serverString string in format of constructServerString()
     * @return returns pointer to server
     */
    Server *getServerFromString(const QString &serverString) {
        for (auto &srv : servers) {
            if (ServerManager::constructServerListString(srv) == serverString) {
                return &srv;
            }
        }
        return nullptr;
    }


    /**
     * @brief overrides default close behavior and add save? dialog
     * @param event Close event
     */
    void closeEvent(QCloseEvent *event) override {
        if (servers_initial_values == servers) {
            event->accept();
            QDialog::closeEvent(event);
        } else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Save changes?", "Save changes to server list?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                saveServerList();
                event->accept();
                QDialog::closeEvent(event);
            } else {
                event->accept();
                QDialog::closeEvent(event);
            }
        }
    }

    /**
     * @brief saves settings changes to hard drive
     */
    void saveServerList() {
        if (settings == nullptr)
            settings = std::make_unique<QSettings>("vhundef", "DB_Coursework");
        QStringList servers_from_list;
        for (int i = 0; i < itemModel->rowCount(); ++i) {
            servers_from_list << itemModel->index(i, 0).data().toString();
        }
        settings->beginWriteArray("db/servers");
        int i = 0;
        for (auto &srv : servers) {
            settings->setArrayIndex(i);
            settings->setValue("host", srv.host);
            settings->setValue("port", srv.port);
            settings->setValue("user", srv.user);
            settings->setValue("password", srv.password);
            settings->setValue("db", srv.db);
            i++;
        }
        settings->endArray();
    }
};

#endif//DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
