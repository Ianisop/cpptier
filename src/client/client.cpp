#include "client.h"

#include <iostream>
int main()
{
    auto client = ctier::Client(AF_INET, SOCK_STREAM, 0);
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
    return 0;
}
