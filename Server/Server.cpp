#include "Common.hpp"

#include <chrono>
#include <sys/wait.h>

typedef std::chrono::high_resolution_clock HRC;

int buffer[64];

int main()
{
    auto server_address = local_addr(SERVER_PORT);
    auto listening_socket = check(make_socket(SOCK_SEQPACKET));
    int connected_socket = 0;

    check(bind(listening_socket, (sockaddr *) &server_address, sizeof(server_address)));
    check(listen(listening_socket, 2));

    while (true)
    {
        sockaddr_in connected_address{};
        socklen_t addr_len = sizeof(connected_socket);

        connected_socket = check(accept(listening_socket, (sockaddr *) &connected_address, &addr_len));

        std::cout << "Connected from " << connected_address << std::endl;

        static std::mt19937 gen(HRC::now().time_since_epoch().count());
        std::uniform_int_distribution<> uid(-1000, 1000);
        int guessed_value = uid(gen);
        std::cout << "Guessed value = " << guessed_value << std::endl;

        pid_t p = fork();

        if (p)
        {
            int count = 0;
            while (true)
            {
                int tmp = 1;

                int st = 0;
                if (waitpid(p, &st, WNOHANG) > 0)
                {
                    std::cout << "Terminating..." << std::endl;
                    tmp = -1;
                    check(send(connected_socket, &tmp, sizeof(int), MSG_WAITFORONE));
                    break;
                }

                int size = check(recv(connected_socket, buffer, sizeof(buffer), MSG_WAITFORONE));
                if (size == 0 || (size < 0 && errno == ENOTCONN))
                    break; //disconnected

                if (buffer[0] == guessed_value)
                    tmp = -1;
                else
                    tmp = 0;
                check(send(connected_socket, &tmp, sizeof(int), MSG_WAITFORONE));

                check(size);
                std::cout   << "[proc: " << p << "] ["
                            << connected_address
                            << " send a message of a size " << size
                            << "] ---- [" << ++count << "] " << buffer[0] << std::endl;

            }
            std::cout << "Disconnected from " << connected_address << std::endl;
        }
    }
}