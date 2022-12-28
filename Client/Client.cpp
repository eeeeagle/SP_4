#include "Common.hpp"

int main()
{
    auto dest_address = local_addr(SERVER_PORT);
    int sock_fd = check(make_socket(SOCKET_TYPE));
    check(connect(sock_fd, (sockaddr*)&dest_address, sizeof(dest_address)));

    std::string message;
    while (true)
    {
        std::getline(std::cin,  message);
        if (message == "q")
            break;
        send(sock_fd, message.c_str(), message.size() + 1, MSG_WAITALL);
    }

    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
}