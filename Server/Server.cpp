#include "Common.hpp"

char buffer[64];

int main()
{
    auto server_address = local_addr(SERVER_PORT);
    auto listening_socket = check(make_socket(SOCKET_TYPE));
    int connected_socket = 0;

    check(bind(listening_socket, (sockaddr*)&server_address, sizeof(server_address)));
    check(listen(listening_socket, 2));

    while (1)
    {
        sockaddr_in connected_address{};
        socklen_t addr_len = sizeof(connected_socket);
        connected_socket = check(accept(listening_socket, (sockaddr*)&connected_address,
                                        &addr_len));

        std::cout << "Connected from " << connected_address << std::endl;

        while (true)
        {
            int size = recv(connected_socket, buffer, sizeof(buffer), MSG_WAITALL);

            if (size == 0 || (size < 0 && errno == ENOTCONN))
                break; //disconnected

            check(size);
            std::cout   << connected_address
                        << " send a message of a size " << size
                        << ":" << std::string_view(buffer, size) << std::endl;
        }
        std::cout << "Disconnected from " << connected_address << std::endl;
    }
}