#include <QQueue>
#include <pqxx/connection>
#include <spdlog/spdlog.h>

//
// Created by Vladimir on 22.05.2021.
//

#ifndef DB_QT_COURSEWORK__DATA_BASE_MANAGER_HPP_
#define DB_QT_COURSEWORK__DATA_BASE_MANAGER_HPP_

class DataBaseManager {
private:
public:
    pqxx::connection *serverConnection{nullptr};
    void setConnection(pqxx::connection *connection) {
        if (connection == nullptr) {
            spdlog::warn("DataBaseManager::setConnection() got null instead of normal connection");
            return;
        }
        serverConnection = connection;
    }
    [[nodiscard]] const pqxx::connection *connection() const { return serverConnection; }
};


#endif//DB_QT_COURSEWORK__DATA_BASE_MANAGER_HPP_
