#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <string>
#include <cinttypes>
#include <exception>
#include <memory>
#include <array>

namespace exdev {
    class Socket {
    private:
        struct InternalData;
    public:
        Socket(std::string host, uint16_t port);

        virtual ~Socket();

        bool open();

        bool close();

        bool isOpen() const;

        size_t write(const char *buffer, size_t size);

        template<uint64_t size>
        size_t write(const std::array<char, size> &array) {
            return write(&array[0], size);
        }

        size_t write(const std::string &s) {
            return write(s.c_str(), s.size());
        }

        size_t read(char *buffer, size_t size);

        template<uint64_t size>
        size_t read(std::array<char, size> &array) {
            return read(&array[0], size);
        }

        size_t available() const;

        Socket(const Socket &) = delete;

        Socket(Socket &&) = delete;

        Socket operator=(const Socket &) = delete;

        Socket operator=(Socket &&) = delete;

        static void init();

    private:
        const std::string host;
        const uint16_t port;
        bool opened;
        std::unique_ptr<InternalData> data;
    };
}

#endif //SOCKET_SOCKET_H
