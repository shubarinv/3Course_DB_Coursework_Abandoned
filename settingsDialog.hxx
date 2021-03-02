//
// Created by vhund on 01.03.2021.
//

#ifndef DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
#define DB_QT_COURSEWORK_SETTINGSDIALOG_HXX

#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardItemModel>
#include <iostream>
class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog() {
        setModal(true);
        setWindowTitle("Settings");

        constructServerSettingsPage();
    }
    QGridLayout *layout{};
    QStandardItemModel *itemModel;
    QListView *servers_list;
    QLineEdit *srvIP_inp;
    QLineEdit *srvPort_inp;
    QLineEdit *srvLogin_inp;
    QLineEdit *srvPass_inp;
    QLineEdit *srvDB_inp;

    QPushButton *addServer_btn;
    QPushButton *removeServer_btn;
    QSettings *settings;


private:
    void closeEvent(QCloseEvent *event) override {
        QMessageBox::StandardButton reply;
        QStringList servers_from_list;
        for (int i = 0; i < itemModel->rowCount(); ++i) {
            servers_from_list << itemModel->index(i, 0).data().toString();
        }
        if (settings->value("db/servers").toStringList() != servers_from_list) {
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
        event->accept();
        QDialog::closeEvent(event);
    }
    void removeServerFromList() const {
        itemModel->removeRow(servers_list->currentIndex().row());
    }

    void addServer() const {
        if (srvIP_inp->text().isEmpty()) { return; }
        auto *newServer = new QStandardItem(srvIP_inp->text());
        itemModel->appendRow(newServer);
        srvIP_inp->clear();
    }

    void fillServerList(const QStringList &loadedServer_list) const {
        for (auto &srv : loadedServer_list) {
            auto *newServer = new QStandardItem(srv);
            itemModel->appendRow(newServer);
        }
    }
    void saveServerList() const {
        QStringList servers_from_list;
        for (int i = 0; i < itemModel->rowCount(); ++i) {
            servers_from_list << itemModel->index(i, 0).data().toString();
        }

        settings->setValue("db/servers", QVariant::fromValue(servers_from_list));
    }
    void constructServerSettingsPage(){
        srvIP_inp = new QLineEdit(this);
        srvPort_inp=new QLineEdit(this);
        srvLogin_inp=new QLineEdit(this);
        srvPass_inp=new QLineEdit(this);
        srvDB_inp=new QLineEdit(this);


        auto *ipValidator = new QRegExpValidator(this);
        QRegExp ip_regexp("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
        ipValidator->setRegExp(ip_regexp);
        srvIP_inp->setValidator(ipValidator);

        itemModel = new QStandardItemModel(this);

        servers_list = new QListView(this);
        servers_list->setModel(itemModel);

        addServer_btn = new QPushButton(this);
        addServer_btn->setText("Add");
        addServer_btn->setObjectName("add-btn");

        removeServer_btn = new QPushButton(this);
        removeServer_btn->setText("Remove");
        removeServer_btn->setObjectName("remove-btn");

        layout = new QGridLayout(this);
        layout->addWidget(servers_list, 0, 0, 1, 1);
        layout->addWidget(removeServer_btn, 0, 1, 1, 1);
        layout->addWidget(srvIP_inp, 1, 0, 1, 1);
        layout->addWidget(addServer_btn, 1, 1, 1, 1);

        auto *srvPort_label = new QLabel(this);
        auto *srvLogin_label = new QLabel(this);
        auto *srvPass_label = new QLabel(this);
        auto *srvDB_label = new QLabel(this);

        // inputs
        layout->addWidget(srvPort_inp, 2, 1, 1, 1);
        layout->addWidget(srvLogin_inp, 3, 1, 1, 1);
        layout->addWidget(srvPass_inp, 4, 1, 1, 1);
        layout->addWidget(srvDB_inp, 5, 1, 1, 1);

        // connections
        connect(addServer_btn, &QPushButton::clicked, this, [this]() {
          addServer();
        });
        connect(removeServer_btn, &QPushButton::clicked, this, [this]() {
          removeServerFromList();
        });

        // settings loading
        settings = new QSettings("vh", "DB_Coursework");
        fillServerList(settings->value("db/servers").toStringList());
    }
};

#endif//DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
