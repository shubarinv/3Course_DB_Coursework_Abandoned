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
    static QString constructServerListString(Server *serverData) {
        return serverData->user + "@" + serverData->host + ":" + serverData->port + "/" + serverData->db;
    }
    static void loadServers(QList<Server> *serverListToFill) {
        serverListToFill->clear();
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
            serverListToFill->push_back(server);
        }
        settings_loc->endArray();
    }

    SettingsDialog() {
        setModal(true);
        setWindowTitle("Settings");

        constructServerSettingsPage();
    }
    QList<Server> getServers() {
        if (!servers.empty()) {
            return servers;
        } else {
            int size = settings->beginReadArray("db/servers");
            for (int i = 0; i < size; ++i) {
                settings->setArrayIndex(i);
                Server server;
                server.host = settings->value("host").toString();
                server.port = settings->value("port").toString();
                server.user = settings->value("user").toString();
                server.password = settings->value("password").toString();
                server.db = settings->value("db").toString();
                servers.push_back(server);
            }
            settings->endArray();
            return servers;
        }
    }
    QGridLayout *layout{};
    QStandardItemModel *itemModel{};
    QListView *servers_list{};
    QLineEdit *srvHost_inp{};
    QLineEdit *srvPort_inp{};
    QLineEdit *srvLogin_inp{};
    QLineEdit *srvPass_inp{};
    QLineEdit *srvDB_inp{};

    QPushButton *addServer_btn{};
    QPushButton *removeServer_btn{};
    QPushButton *clearData_btn{};
    QSettings *settings{};
    QList<Server> servers;


private:
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
    void removeServerFromList() {
        int i = 0;
        for (auto &srv : servers) {
            if (constructServerListString(&srv) == itemModel->index(servers_list->currentIndex().row(), 0).data().toString()) {
                break;
            }
            i++;
        }
        servers.removeAt(i);
        itemModel->removeRow(servers_list->currentIndex().row());
    }

    void addServer() {
        Server server;
        server.host = srvHost_inp->text();
        server.port = srvPort_inp->text();
        server.user = srvLogin_inp->text();
        server.password = srvPass_inp->text();
        server.db = srvDB_inp->text();
        servers.push_back(server);
        auto *newServer = new QStandardItem(constructServerListString(&server));
        itemModel->appendRow(newServer);
        clearInputFields();
    }

    void fillServerList() {
        for (auto &srv : servers) {
            auto *newServer = new QStandardItem(constructServerListString(&srv));
            itemModel->appendRow(newServer);
        }
    }
    void saveServerList() const {
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
    void constructServerSettingsPage() {
        srvHost_inp = new QLineEdit(this);
        srvPort_inp = new QLineEdit(this);
        srvLogin_inp = new QLineEdit(this);
        srvPass_inp = new QLineEdit(this);
        srvDB_inp = new QLineEdit(this);

        srvPass_inp->setEchoMode(QLineEdit::Password);

        itemModel = new QStandardItemModel(this);

        servers_list = new QListView(this);
        servers_list->setModel(itemModel);

        addServer_btn = new QPushButton(this);
        addServer_btn->setText("Add");
        addServer_btn->setObjectName("add-btn");
        addServer_btn->setDisabled(true);

        clearData_btn = new QPushButton(this);
        clearData_btn->setText("Clear");
        clearData_btn->setObjectName("clear-btn");
        clearData_btn->setDisabled(true);

        removeServer_btn = new QPushButton(this);
        removeServer_btn->setText("Remove");
        removeServer_btn->setObjectName("remove-btn");
        removeServer_btn->setDisabled(true);

        layout = new QGridLayout(this);
        layout->addWidget(servers_list, 0, 0, 1, 2);
        layout->addWidget(removeServer_btn, 1, 0, 1, 2);
        layout->addWidget(clearData_btn, 7, 0, 1, 1);
        layout->addWidget(addServer_btn, 7, 1, 1, 1);


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
        layout->addWidget(srvHost_inp, 2, 1, 1, 1);

        layout->addWidget(srvPort_label, 3, 0, 1, 1);
        layout->addWidget(srvPort_inp, 3, 1, 1, 1);

        layout->addWidget(srvLogin_label, 4, 0, 1, 1);
        layout->addWidget(srvLogin_inp, 4, 1, 1, 1);

        layout->addWidget(srvPass_label, 5, 0, 1, 1);
        layout->addWidget(srvPass_inp, 5, 1, 1, 1);

        layout->addWidget(srvDB_label, 6, 0, 1, 1);
        layout->addWidget(srvDB_inp, 6, 1, 1, 1);

        // connections
        connect(addServer_btn, &QPushButton::clicked, this, [this]() {
            addServer();
        });
        connect(removeServer_btn, &QPushButton::clicked, this, [this]() {
            removeServerFromList();
        });

        connect(servers_list, &QListView::clicked, this, [this](const QModelIndex &index) {
            fillInputFields(index);
        });

        connect(srvHost_inp, &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvPort_inp, &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvLogin_inp, &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvPass_inp, &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });
        connect(srvDB_inp, &QLineEdit::textChanged, this, [this]() {
            checkFormFill();
        });


        // settings loading
        settings = new QSettings("vhundef", "DB_Coursework");

        int size = settings->beginReadArray("db/servers");
        for (int i = 0; i < size; ++i) {
            settings->setArrayIndex(i);
            Server server;
            server.host = settings->value("host").toString();
            server.port = settings->value("port").toString();
            server.user = settings->value("user").toString();
            server.password = settings->value("password").toString();
            server.db = settings->value("db").toString();
            servers.push_back(server);
        }
        settings->endArray();
        fillServerList();
        servers_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    void fillInputFields(const QModelIndex &index) {
        auto srv = getServerFromString(itemModel->index(servers_list->currentIndex().row(), 0).data().toString());
        srvHost_inp->setText(srv->host);
        srvPort_inp->setText(srv->port);
        srvLogin_inp->setText(srv->user);
        srvPass_inp->setText(srv->password);
        srvDB_inp->setText(srv->db);
    }

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
        }
        else{
            clearData_btn->setDisabled(true);
        }
    }
    void clearInputFields() const {
        srvHost_inp->clear();
        srvPort_inp->clear();
        srvLogin_inp->clear();
        srvPass_inp->clear();
        srvDB_inp->clear();
    }
    Server *getServerFromString(const QString &serverString) {
        for (auto &srv : servers) {
            if (constructServerListString(&srv) == serverString) {
                return &srv;
            }
        }
        return nullptr;
    }
};

#endif//DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
