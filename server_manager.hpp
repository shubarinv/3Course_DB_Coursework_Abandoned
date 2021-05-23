#include <utility>

//
// Created by vhund on 25.03.2021.
//

#ifndef DB_QT_COURSEWORK__SERVER_MANAGER_HPP_
#define DB_QT_COURSEWORK__SERVER_MANAGER_HPP_

struct Server {
    QString host{}, port{}, db{}, user{}, password{};
    bool operator==(const Server &rhs) const {
        return (this->host == rhs.host &&
                this->port == rhs.port &&
                this->user == rhs.user &&
                this->db == rhs.db &&
                this->password == this->password);
    }
};

class ServerManager {
    QList<Server> servers;
public:
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


    /**
     * @brief Wrapper of loadServers; used for optimization (will not load servers hard drive if they were loaded previously)
     * @return QList<Server> List of servers
     */
    QList<Server> getServers() {
        if (!servers.empty()) {
            return servers;
        } else {
            loadServers(servers);
            return servers;
        }
    }
    QList<Server> &getServersRef() {
        return servers;
    }


    /**
     * @brief Constructs string that is used in Server list
     * @param serverData pointer to Server(instance) that contains data about connection
     * @return string in format  [user]@[host]:[port]/[database_name]
     */
    static QString constructServerListString(Server &serverData) {
        return serverData.user + "@" + serverData.host + ":" + serverData.port + "/" + serverData.db;
    }

    void UpdateServerList() {
        loadServers(servers);
    }
    void UpdateServerList(QList<Server> _servers) {
        servers = std::move(_servers);
    }

    static QString constructConnectionString(Server &server) {
        return "host= " + server.host + " user=" + server.user + " port= " + server.port + " dbname= " + server.db + " password= " + server.password + " connect_timeout= 4";
    }

    static pqxx::connection *tryConnectingToServer(const QString &connectString) {
        pqxx::connection *connection{nullptr};
        try {
            connection = new pqxx::connection(connectString.toStdString());
        } catch (const std::exception &e) {
            spdlog::error(e.what());
            return nullptr;
        }
        spdlog::info(std::string("Connected to: ") + connection->username() + "@" + connection->hostname() + ":" + connection->port() + "/" + connection->dbname());
        return connection;
    }
    static pqxx::connection *tryConnectingToServer(Server &server) {
        return tryConnectingToServer(constructConnectionString(server));
    }
};


#endif//DB_QT_COURSEWORK__SERVER_MANAGER_HPP_
