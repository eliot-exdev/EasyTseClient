#include <exdev/TseClient.h>

#include <iostream>

#define GUID "ABCD"
#define HOST "localhost"
#define PORT 4444

int main(int argc, char **argv) {
    // init tse
    exdev::TseClient::init();

    // create tse client
    exdev::TseClient tse{GUID, HOST, PORT};

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
