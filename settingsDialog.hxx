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
struct Server {
    QString host, port, db, user, password;
};

class SettingsDialog : public QDialog {
    Q_OBJECT


public:
    /**
     * @brief Constructs string that is used in Server list
     * @param serverData pointer to Server(instance) that contains data about connection
     * @return string in format  [user]@[host]:[port]/[database_name]
     */
    static QString constructServerListString(Server &serverData) {
        return serverData.user + "@" + serverData.host + ":" + serverData.port + "/" + serverData.db;
    }
    /**
     * @brief loads data about servers from computer storage
     * @param serverListToFill ref to QList<Server> that you want to fill
     */
    static void loadServers(QList<Server> &serverListToFill) {
        serverListToFill.clear();
        auto settings_loc = new QSettings("vhundef", "DB_Coursework");
        int size = settings_loc->beginReadArray("db/servers");
        for (int i = 0; i < size; ++i) {
            settings_loc->setArrayIndex(i);
            Server server;
            server.host = settings_loc->value("host").toString();
            server.port = settings_loc->value("port").toString();
            server.user = settings_loc->value("user").toString();
            server.password = settings_loc->value("password").toString();
            server.db = settings_loc->value("db").toString();
            serverListToFill.push_back(server);
        }
        settings_loc->endArray();
    }

    SettingsDialog() {
        setModal(true);
        setWindowTitle("Settings");

        constructServerSettingsPage();
    }

    /**
     * @brief Wrapper of loadServers; used for optimization (will not load servers hard drive if they were loaded previously)
     * @return QList<Server> List of servers
     */
    QList<Server> getServers() {
        if (!servers.empty()) {
            return servers;
        } else {
            SettingsDialog::loadServers(servers);
            return servers;
        }
    }
    std::unique_ptr<QGridLayout> layout;
    std::unique_ptr<QStandardItemModel> itemModel{};
    std::unique_ptr<QListView> servers_list{};
    std::unique_ptr<QLineEdit> srvHost_inp{};
    std::unique_ptr<QLineEdit> srvPort_inp{};
    std::unique_ptr<QLineEdit> srvLogin_inp{};
    std::unique_ptr<QLineEdit> srvPass_inp{};
    std::unique_ptr<QLineEdit> srvDB_inp{};

    std::unique_ptr<QPushButton> addServer_btn{};
    std::unique_ptr<QPushButton> editServer_btn{};
    std::unique_ptr<QPushButton> removeServer_btn{};
    std::unique_ptr<QPushButton> clearData_btn{};
    std::unique_ptr<QSettings> settings{};
    QList<Server> servers;


private:
    /**
     * @brief overrides default close behavior and add save? dialog
     * @param event Close event
     */
    void closeEvent(QCloseEvent *event) override {
        QMessageBox::StandardButton reply;
        QStringList servers_from_list;
        for (int i = 0; i < itemModel->rowCount(); ++i) {
            servers_from_list << itemModel->index(i, 0).data().toString();
        }
        reply = QMessageBox::question(this, "Save changes?", "Save changes to server list?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            saveServerList();
            event->accept();
            QDialog::closeEvent(event);
        } else {
            event->accept();
            QDialog::closeEvent(event);
        }
        event->accept();
        QDialog::closeEvent(event);
    }

    /**
     * @brief removes selected(in Vertical List)  server from list
     */
    void removeServerFromList() {
        int i = 0;
        for (auto &srv : servers) {
            if (constructServerListString(srv) == itemModel->index(servers_list->currentIndex().row(), 0).data().toString()) {
                break;
            }
            i++;
        }
        servers.removeAt(i);
        itemModel->removeRow(servers_list->currentIndex().row());
        removeServer_btn->setDisabled(true);
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
        auto *newServer = new QStandardItem(constructServerListString(server));
        itemModel->appendRow(newServer);
        clearInputFields();
    }

    /**
     * @brief fills vertical list with elements
     */
    void fillServerList() {
        for (auto &srv : servers) {
            auto *newServer = new QStandardItem(constructServerListString(srv));
            itemModel->appendRow(newServer);
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
    /**
     * @brief constructs server settings page
     */
    void constructServerSettingsPage() {
        srvHost_inp = std::make_unique<QLineEdit>(this);
        srvPort_inp = std::make_unique<QLineEdit>(this);
        srvLogin_inp = std::make_unique<QLineEdit>(this);
        srvPass_inp = std::make_unique<QLineEdit>(this);
        srvDB_inp = std::make_unique<QLineEdit>(this);

        srvPass_inp->setEchoMode(QLineEdit::Password);

        itemModel = std::make_unique<QStandardItemModel>(this);

        servers_list = std::make_unique<QListView>(this);
        servers_list->setModel(itemModel.get());

        addServer_btn = std::make_unique<QPushButton>(this);
        addServer_btn->setText("Add");
        addServer_btn->setObjectName("add-btn");
        addServer_btn->setDisabled(true);

        clearData_btn = std::make_unique<QPushButton>(this);
        clearData_btn->setText("Clear");
        clearData_btn->setObjectName("clear-btn");
        clearData_btn->setDisabled(true);

        editServer_btn = std::make_unique<QPushButton>(this);
        editServer_btn->setText("Edit");
        editServer_btn->setObjectName("edit-btn");
        editServer_btn->setDisabled(true);

        removeServer_btn = std::make_unique<QPushButton>(this);
        removeServer_btn->setText("Remove");
        removeServer_btn->setObjectName("remove-btn");
        removeServer_btn->setDisabled(true);

        layout = std::make_unique<QGridLayout>(this);
        layout->addWidget(servers_list.get(), 0, 0, 1, 2);
        layout->addWidget(editServer_btn.get(), 1, 0, 1, 1);
        layout->addWidget(removeServer_btn.get(), 1, 1, 1, 1);
        layout->addWidget(clearData_btn.get(), 7, 0, 1, 1);
        layout->addWidget(addServer_btn.get(), 7, 1, 1, 1);


        auto *srvHost_label = new QLabel(this);
        srvHost_label->setText("Host:");
        auto *srvPort_label = new QLabel(this);
        srvPort_label->setText("Port:");
        auto *srvLogin_label = new QLabel(this);
        srvLogin_label->setText("Login:");
        auto *srvPass_label = new QLabel(this);
        srvPass_label->setText("Password:");
        auto *srvDB_label = new QLabel(this);
        srvDB_label->setText("Database:");

        // inputs and labels
        layout->addWidget(srvHost_label, 2, 0, 1, 1);
        layout->addWidget(srvHost_inp.get(), 2, 1, 1, 1);

        layout->addWidget(srvPort_label, 3, 0, 1, 1);
        layout->addWidget(srvPort_inp.get(), 3, 1, 1, 1);

        layout->addWidget(srvLogin_label, 4, 0, 1, 1);
        layout->addWidget(srvLogin_inp.get(), 4, 1, 1, 1);

        layout->addWidget(srvPass_label, 5, 0, 1, 1);
        layout->addWidget(srvPass_inp.get(), 5, 1, 1, 1);

        layout->addWidget(srvDB_label, 6, 0, 1, 1);
        layout->addWidget(srvDB_inp.get(), 6, 1, 1, 1);

        // connections
        connect(addServer_btn.get(), &QPushButton::clicked, this, [this]() {
            addServer();
        });
        connect(removeServer_btn.get(), &QPushButton::clicked, this, [this]() {
            removeServerFromList();
        });
        connect(clearData_btn.get(), &QPushButton::clicked, this, [this]() {
            clearInputFields();
        });
        connect(servers_list.get(), &QListView::clicked, this, [this](const QModelIndex &index) {
            fillInputFields(index);
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


        getServers();
        fillServerList();
        servers_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    /**
     * @brief when server selected from Vertical list this function is called to fill input field with it's data
     * @param index index of the element that will be edited
     */
    void fillInputFields(const QModelIndex &index) {
        auto srv = getServerFromString(itemModel->index(servers_list->currentIndex().row(), 0).data().toString());
        srvHost_inp->setText(srv->host);
        srvPort_inp->setText(srv->port);
        srvLogin_inp->setText(srv->user);
        srvPass_inp->setText(srv->password);
        srvDB_inp->setText(srv->db);
        removeServer_btn->setEnabled(true);
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
        } else {
            addServer_btn->setDisabled(false);
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
     * @brief searches for server by serverString
     * @param serverString string in format of constructServerString()
     * @return returns pointer to server
     */
    Server *getServerFromString(const QString &serverString) {
        for (auto &srv : servers) {
            if (constructServerListString(srv) == serverString) {
                return &srv;
            }
        }
        return nullptr;
    }
};

#endif//DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
