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

        ipAddress_inp = new QLineEdit(this);
        ipAddress_inp->setPlaceholderText("Server IP");
        auto *v = new QRegExpValidator(this);
        QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
        v->setRegExp(rx);
        ipAddress_inp->setValidator(v);

        itemModel = new QStandardItemModel(this);

        servers_list = new QListView(this);
        servers_list->setModel(itemModel);

        addServer_btn = new QPushButton(this);
        addServer_btn->setText("Add");
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
        settings = new QSettings("vh", "DB_Coursework");
        fillServerList(settings->value("db/servers").toStringList());
        qDebug()<<settings->value("db/servers").toStringList();
    }
    QGridLayout *layout{};
    QStandardItemModel *itemModel;
    QListView *servers_list;
    QLineEdit *ipAddress_inp;
    QPushButton *addServer_btn;
    QPushButton *removeServer_btn;
    QSettings *settings;


private:
    void closeEvent(QCloseEvent *event) override {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Test", "Quit?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
            saveServerList();
            event->accept();
            QDialog::closeEvent(event);
        } else {
            qDebug() << "Yes was *not* clicked";
            event->ignore();
        }
    }
    void removeServerFromList() const {
        itemModel->removeRow(servers_list->currentIndex().row());
    }

    void addServer() const {
        if (ipAddress_inp->text().isEmpty()) { return; }
        auto *newServer = new QStandardItem(ipAddress_inp->text());
        itemModel->appendRow(newServer);
        ipAddress_inp->clear();
    }

    void fillServerList(const QStringList &loadedServer_list) const {
        for (auto &srv : loadedServer_list) {
            auto *newServer = new QStandardItem(srv);
            itemModel->appendRow(newServer);
        }
    }
    void saveServerList() {
        QStringList servers_from_list;
        for (int i = 0; i < itemModel->rowCount(); ++i) {
            servers_from_list << itemModel->index(i, 0).data().toString();
        }

        settings->setValue("db/servers", QVariant::fromValue(servers_from_list));
    }
};

#endif//DB_QT_COURSEWORK_SETTINGSDIALOG_HXX
