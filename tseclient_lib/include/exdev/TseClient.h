#ifndef SOCKET_TSECLIENT_H
#define SOCKET_TSECLIENT_H

#include <string>
#include <cinttypes>

namespace exdev {
    class TseClient {
    public:
        enum class RequestResult : int {
            OK,
            ERROR,
            ERROR_SOCKET_OPEN,
            ERROR_SOCKET_WRITE,
            ERROR_SOCKET_READ,
            ERROR_UNKNOWN_RESPONSE,
            ERROR_TSE_BUSY
        };

        struct StartTransactionResponse {
            std::string id;
            std::string time;
        };

        static const std::string &toString(RequestResult r);

        TseClient(std::string guid, std::string host, uint16_t port);

        TseClient(const TseClient &) = delete;

        TseClient(TseClient &&) = delete;

        TseClient operator=(const TseClient &) = delete;

        TseClient operator=(TseClient &&) = delete;

        virtual ~TseClient() = default;

        RequestResult ping();

        RequestResult startTransaction(StartTransactionResponse &response);

        static void init();

    private:
        const std::string host;
        const uint16_t port;
        const std::string guid;
    };
}

#endif //SOCKET_TSECLIENT_H
