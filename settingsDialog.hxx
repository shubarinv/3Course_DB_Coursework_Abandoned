//
// Created by vhund on 01.03.2021.
//

#ifndef DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
#define DB_QT_COURSEWORK_SETTINGSDIALOG_HXX

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStandardItemModel>
#include <iostream>
class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog() {
        setModal(true);
        setWindowTitle("Settings");

        ipAddress_inp = new QLineEdit(this);
        ipAddress_inp->setPlaceholderText("Server IP");

        itemModel = new QStandardItemModel(this);

        servers_list = new QListView(this);
        servers_list->setModel(itemModel);

        addServer_btn = new QPushButton(this);
        addServer_btn->setText("ADD");
        addServer_btn->setObjectName("add-btn");
       // addServer_btn->setStyleSheet("background:#0275d8;padding: 4px 25px 4px 20px;color:#ffffff;");

        removeServer_btn = new QPushButton(this);
        removeServer_btn->setText("Remove");
        removeServer_btn->setObjectName("remove-btn");
       // removeServer_btn->setStyleSheet("background:#d9534f;padding: 4px 25px 4px 20px;color:#ffffff;");

        layout = new QGridLayout(this);
        layout->addWidget(servers_list, 0, 0, 1, 1);
        layout->addWidget(removeServer_btn, 0, 1, 1, 1);
        layout->addWidget(ipAddress_inp, 1, 0, 1, 1);
        layout->addWidget(addServer_btn, 1, 1, 1, 1);

        connect(addServer_btn, &QPushButton::clicked, this, [this]() {
            addServer();
        });
        connect(removeServer_btn, &QPushButton::clicked, this, [this]() {
            removeServerFromList();
        });
    }
    QGridLayout *layout{};
    QStandardItemModel *itemModel;
    QListView *servers_list;
    QLineEdit *ipAddress_inp;
    QPushButton *addServer_btn;
    QPushButton *removeServer_btn;


private:
    void removeServerFromList() const {
        itemModel->removeRow(servers_list->currentIndex().row());
    }

    void addServer() const {
        if(ipAddress_inp->text().isEmpty()){return;}
        auto *newServer = new QStandardItem(ipAddress_inp->text());
        itemModel->appendRow(newServer);
        ipAddress_inp->clear();
    }
};

#endif//DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
