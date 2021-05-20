//
// Created by Vladimir on 19.05.2021.
//

#ifndef DB_QT_COURSEWORK__INPUT_DIALOG_HPP_
#define DB_QT_COURSEWORK__INPUT_DIALOG_HPP_


#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <utility>

class InputDialog : public QDialog {
    Q_OBJECT
public:
    InputDialog() {
        setModal(true);
        layout = std::make_unique<QGridLayout>();

        add_btn = std::make_unique<QPushButton>("Save", this);
        add_btn->setObjectName("add-btn");

        setLayout(layout.get());
        layout->addWidget(add_btn.get(), 0, 0, 1, 2);
    }

    void SetTitle(const QString &title) {
        setWindowTitle(title);
    }

    void AddField(const std::string &title) {
        auto label = new QLabel;
        auto inputField = new QLineEdit;

        label->setText(title.c_str());
        layout->addWidget(label, (int) textInputs.size() + 1, 0);
        layout->addWidget(inputField, (int) textInputs.size() + 1, 1);
        textInputs.push_back(inputField);
        update();
    }


private:
    std::unique_ptr<QGridLayout> layout;
    std::list<QLineEdit *> textInputs;
    std::unique_ptr<QPushButton> add_btn;
};


#endif//DB_QT_COURSEWORK__INPUT_DIALOG_HPP_
