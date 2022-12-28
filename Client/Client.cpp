#include "Common.hpp"
#include <vector>
#include <algorithm>

bool is_receive(int sock_fd, int& buf)
{
    int size = check(recv(sock_fd, &buf, sizeof(buf), MSG_WAITFORONE));
    if (buf == -1)
        return false;
    check(size);
    return true;
}

int main() 
{
    auto dest_address = local_addr(SERVER_PORT);
    int sock_fd = check(make_socket(SOCK_SEQPACKET));
    check(connect(sock_fd, (sockaddr*)&dest_address, sizeof(dest_address)));

    std::cout << "Connected to " << dest_address << std::endl;

    std::vector<int> attempt = {};
    for (int i = -1000; i <= 1000; attempt.push_back(i++));
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(attempt.begin(), attempt.end(), mt);

    int message = 0;
    int buff;
    int count = 0;
    do
    {
        message = attempt.back();
        attempt.pop_back();

        std::cout << "[" << ++count << "] " << message << std::endl;
        check(send(sock_fd, &message, sizeof(message), MSG_WAITFORONE));

        if(!is_receive(sock_fd, buff))
            break;
    }
    while (buff != -1 || !(attempt.empty()));

    shutdown(sock_fd, SHUT_RDWR);
    std::cout << "Disconnected from " << dest_address << std::endl;

    close(sock_fd);
}