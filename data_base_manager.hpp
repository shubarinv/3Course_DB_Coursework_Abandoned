#include <QQueue>
#include <pqxx/connection>
#include <spdlog/spdlog.h>

//
// Created by Vladimir on 22.05.2021.
//

#ifndef DB_QT_COURSEWORK__DATA_BASE_MANAGER_HPP_
#define DB_QT_COURSEWORK__DATA_BASE_MANAGER_HPP_

struct message {
    enum MessageType { add,
                       remove,
                       edit,
                       connect,
                       disconnect,
                       undefined };
    MessageType type{undefined};
    QList<QString> parameters{};
    message(MessageType type, QList<QString> params) : type(type), parameters(std::move(params)) {}
    message(MessageType type) : type(type) {}
    message() = default;
};
class DataBaseManager {
private:
    QQueue<message> messages;
    bool quit = false;
    std::mutex m;
    pqxx::connection *connection{};

public:
    void EnqueueMessage(const message &message) {
        messages << message;
    }

    DataBaseManager() {
        std::thread t1([&] {
            while (!quit) {
                m.lock();

                if (messages.empty()) {
                    m.unlock();
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(1ms);
                } else {
                    while (!messages.empty()) {
                        auto msg = messages.dequeue();
                        switch (msg.type) {
                            case message::add:
                                break;
                            case message::remove:
                                break;
                            case message::edit:
                                break;
                            case message::connect:
                                try {
                                    connection = new pqxx::connection(msg.parameters.first().toStdString());
                                } catch (const std::exception &e) {
                                    spdlog::error(e.what());
                                    connection = nullptr;
                                }
                                if (connection != nullptr) spdlog::info("Connected to {}", connection->hostname());
                                break;
                            case message::disconnect:
                                break;

                            case message::undefined:
                                // nothing to do here
                                break;
                        }
                        m.unlock();
                    }
                }
                m.unlock();
            }
        });
        t1.detach();
    }
    void Quit() {
        quit = true;
    }
};


#endif//DB_QT_COURSEWORK__DATA_BASE_MANAGER_HPP_
