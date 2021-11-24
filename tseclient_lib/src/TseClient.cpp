#include "exdev/TseClient.h"
#include "exdev/Socket.h"

#include <map>
#include <regex>

using namespace exdev;

#define PING_RSP_OK "ping ok"
#define PING_RSP_TSE_BUSY "TSE busy"

static const std::map<TseClient::RequestResult, const std::string> REQUEST_RESULT = {
        {TseClient::RequestResult::OK,                     "OK"},
        {TseClient::RequestResult::ERROR,                  "ERROR"},
        {TseClient::RequestResult::ERROR_SOCKET_OPEN,      "ERROR_SOCKET_OPEN"},
        {TseClient::RequestResult::ERROR_SOCKET_WRITE,     "ERROR_SOCKET_WRITE"},
        {TseClient::RequestResult::ERROR_SOCKET_READ,      "ERROR_SOCKET_READ"},
        {TseClient::RequestResult::ERROR_UNKNOWN_RESPONSE, "ERROR_UNKNOWN_RESPONSE"},
        {TseClient::RequestResult::ERROR_TSE_BUSY,         "ERROR_TSE_BUSY"}
};

const std::string &TseClient::toString(const TseClient::RequestResult r) {
    return REQUEST_RESULT.at(r);
}

TseClient::TseClient(std::string guid, std::string host, const uint16_t port) :
        host(std::move(host)), port(port), guid(std::move(guid)) {}

TseClient::RequestResult TseClient::ping() {
    // open socket
    Socket socket{host, port};
    if (!socket.open()) {
        return RequestResult::ERROR_SOCKET_OPEN;
    }

    // write message
    const auto msg = "D-" + guid;
    auto res = socket.write(msg);
    if (res == 0) {
        return RequestResult::ERROR_SOCKET_WRITE;
    }

    // read response
    std::array<char, 128> buffer = {0};
    res = socket.read(buffer);
    if (res == 0) {
        return RequestResult::ERROR_SOCKET_READ;
    }

    // parse response
    const std::string response{&buffer[0]};
    if (response.find(PING_RSP_OK) == 0) {
        return RequestResult::OK;
    }
    if (response.find(PING_RSP_TSE_BUSY) == 0) {
        return RequestResult::ERROR_TSE_BUSY;
    }
    return RequestResult::ERROR_UNKNOWN_RESPONSE;
}

static bool parseStartTransactionResponse(const std::string &msg, TseClient::StartTransactionResponse &response) {
    static const std::regex regex(R"(([0-9]+)\@([A-Za-z0-9\.\s_\-:\.]+)\n)");
    std::smatch match;
    if (std::regex_match(msg, match, regex) && match.size() >= 2) {
        response.id = match[1];
        response.time = match[2];
        return true;
    }
    return false;
}

TseClient::RequestResult TseClient::startTransaction(StartTransactionResponse &response) {
    // open socket
    Socket socket{host, port};
    if (!socket.open()) {
        return RequestResult::ERROR_SOCKET_OPEN;
    }

    // write message
    const auto msg = "A-" + guid;
    auto res = socket.write(msg);
    if (res != msg.size()) {
        return RequestResult::ERROR_SOCKET_WRITE;
    }

    // read response
    std::array<char, 128> buffer = {0};
    res = socket.read(buffer);
    if (res == 0) {
        return RequestResult::ERROR_SOCKET_READ;
    }

    // parse response
    const std::string response_str{&buffer[0]};
    if (parseStartTransactionResponse(response_str, response)) {
        return RequestResult::OK;
    }

    return RequestResult::ERROR_UNKNOWN_RESPONSE;
}

void TseClient::init() { Socket::init(); }
