#include <exdev/TseClient.h>

#include <iostream>

static struct App {
    std::string host;
    uint16_t port;
    std::string guid;
} app;

static void printUsage() {
    std::cout << "test_tse <host> <port> <guid>" << std::endl;
}

static bool parseArgs(const int argc, char **argv) {
    if (argc != 4) {
        return false;
    }
    app.host = std::string{argv[1]};
    app.port = atoi(argv[2]);
    app.guid = std::string{argv[3]};
    return true;
}

int main(int argc, char **argv) {
    if (!parseArgs(argc, argv)) {
        printUsage();
        return 1;
    } else {
        std::cout << "host=" << app.host << std::endl;
        std::cout << "port=" << app.port << std::endl;
        std::cout << "guid=" << app.guid << std::endl;
    }

    // init tse
    exdev::TseClient::init();

    // create tse client
    exdev::TseClient tse{app.guid, app.host, app.port};

    try {
        // ping
        {
            const auto res = tse.ping();
            std::cout << "ping res=" << exdev::TseClient::toString(res) << std::endl;
        }

        // start new transaction
        {
            exdev::TseClient::StartTransactionResponse response;
            const auto res = tse.startTransaction(response);
            std::cout << "start transaction res=" << exdev::TseClient::toString(res) << std::endl;
            if (res == exdev::TseClient::RequestResult::OK) {
                std::cout << "id=" << response.id << ", time=" << response.time << std::endl;
            }
        }
    } catch (const std::runtime_error &e) {
        std::cout << "[ERROR] " << e.what() << std::endl;
    }

    return 0;
}
