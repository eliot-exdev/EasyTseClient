#include <exdev/Socket.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <array>
#include <cstring>

#define MSG "Hello World!\n"
#define HOST "localhost"
#define PORT 4444

int main(int argc, char **argv) {
    // init socket
    exdev::Socket::init();

    try {
        // open connection
        exdev::Socket socket(HOST, PORT);
        if (!socket.open()) {
            std::cout << "[ERROR] could not open socket" << std::endl;
            return 1;
        }

        // write
        auto read_count = 0;
        auto res = socket.write(MSG, strlen(MSG));
        std::cout << "bytes written to socket=" << res << std::endl;

        // read
        std::array<char, 64> buffer = {0};
        res = 0;
        while (res == 0 && read_count < 10) {
            std::cout << "read count=" << read_count++ << std::endl;
            if (socket.available() > 0) {
                res = socket.read(buffer);
                std::cout << "bytes read from socket=" << res << std::endl;
                if (res > 0) {
                    std::cout << "read=" << &buffer[0] << std::endl;
                }
            } else {
                std::cout << "nothing to read, waiting one second ..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

    } catch (std::runtime_error &e) {
        std::cout << "[ERROR] " << e.what() << std::endl;
    }

    return 0;
}
