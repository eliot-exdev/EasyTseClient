#include "exdev/Socket.h"
#include <boost/asio.hpp>

using namespace exdev;

using boost::asio::ip::tcp;

static std::unique_ptr<boost::asio::io_context> io_context;

struct Socket::InternalData {
    std::unique_ptr<tcp::socket> socket;
};


Socket::Socket(std::string host, const uint16_t port) : host(std::move(host)), port(port), opened(false) {
    if (!io_context) {
        throw std::runtime_error("you need to call Socket::init() first!");
    }
}

Socket::~Socket() {
    close();
}

bool Socket::open() {
    if (opened) {
        return false;
    }
    opened = true;

    try {
        data = std::make_unique<Socket::InternalData>();
        tcp::resolver resolver(*io_context);
        tcp::resolver::query query(host, std::to_string(port));
        tcp::resolver::results_type endpoints = resolver.resolve(query);
        data->socket = std::make_unique<tcp::socket>(*io_context);
        boost::asio::connect(*data->socket, endpoints);
    } catch (boost::system::system_error &e) {
        opened = false;
        throw std::runtime_error{e.what()};
    }
    return true;
}

bool Socket::isOpen() const {
    return opened;
}

bool Socket::close() {
    if (!opened) {
        return false;
    }
    opened = false;

    data.reset();

    return true;
}

size_t Socket::write(const char *buffer, const size_t size) {
    if (!opened) {
        throw std::runtime_error{"you need to call Socket::open() first!"};
    }
    return data->socket->write_some(boost::asio::const_buffer(buffer, size));
}

size_t Socket::read(char *buffer, const size_t size) {
    if (!opened) {
        throw std::runtime_error{"you need to call Socket::open() first!"};
    }
    return data->socket->read_some(boost::asio::buffer(buffer, size));
}


size_t exdev::Socket::available() const {
    if (!opened) {
        throw std::runtime_error{"you need to call Socket::open() first!"};
    }
    return data->socket->available();
}

void Socket::init() {
    if (!io_context) {
        io_context = std::make_unique<boost::asio::io_context>();
    }
}
