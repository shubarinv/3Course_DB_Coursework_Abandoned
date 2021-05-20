//
// Created by Vladimir on 19.05.2021.
//

#ifndef DB_QT_COURSEWORK__INPUT_DIALOG_HPP_
#define DB_QT_COURSEWORK__INPUT_DIALOG_HPP_


#include <QDialog>
#include <QGridLayout>
#include <QCloseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <utility>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class InputDialog : public QDialog {
    Q_OBJECT
public:
    InputDialog() {
        setModal(true);
        layout = std::make_unique<QGridLayout>();

        add_btn = std::make_unique<QPushButton>("Save", this);
        add_btn->setObjectName("add-btn");
        cancel_btn = std::make_unique<QPushButton>("Cancel", this);
        cancel_btn->setObjectName("clear-btn");

        setLayout(layout.get());
        layout->addWidget(add_btn.get(), 0, 0, 1, 2);

        layout->addWidget(cancel_btn.get(), 0, 0);
        layout->addWidget(add_btn.get(), 0, 1);


        connect(cancel_btn.get(), &QPushButton::clicked, this, [this]() {
          spdlog::info("dialog->addSuppliers::Cancel");
          close();
        });
        connect(add_btn.get(), &QPushButton::clicked, this, [this]() {
          spdlog::info("dialog->addSuppliers::Add");
          for(auto&inputField:findChildren<QLineEdit*>()){
              results.push_back(inputField->text());
          }
          spdlog::info("results size {}",results.size());
          close();
        });
    }

    void SetTitle(const QString &title) {
        setWindowTitle(title);
    }

    void AddField(const std::string &title) {
        auto label = new QLabel;
        auto inputField = new QLineEdit;

        label->setText(title.c_str());
        layout->addWidget(label, (int) countInputFields() + 1, 0);
        layout->addWidget(inputField, (int) countInputFields() + 1, 1);
        update();
    }

    std::list<QString>&getResults(){
        return results;
    }


private:
    std::unique_ptr<QGridLayout> layout;
    std::list<QString> results;
    std::unique_ptr<QPushButton> add_btn;
    std::unique_ptr<QPushButton> cancel_btn;

    int countInputFields(){
        int inputCount=0;
        for(auto&inputField:findChildren<QLineEdit*>()){
           inputCount++;
        }
        spdlog::info("InputDialog->countInputFields: {}",inputCount);
        return inputCount;
    }
};


#endif//DB_QT_COURSEWORK__INPUT_DIALOG_HPP_
