#include <iostream>

#include "client/client.h"
int main()
{
    auto client = ctier::Client(AF_INET, SOCK_STREAM, 0);

    std::cout << "client ready!\n";
    if (client.connect("127.0.0.1", "1234"))
    {
        std::cout << "connected!\n";
        client.sendData("yooo", 4);
        std::cout << "sent payload\n";
    }
    else
    {
        std::cout << "connection failed!\n";
    }
    char buffer[1024];
    while (true)
    {
        if (client.receiveData(buffer, 1024))
        {
            std::cout << buffer << "\n";
        }
    }
    return 0;
}
