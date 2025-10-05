#pragma once
#include <memory>

#include "websock.h"

namespace ctier
{

    class Client
    {
      private:
        std::unique_ptr<WebSock> _socket;

      public:
        // Constructor: creates the socket on the heap and takes ownership
        Client(int domain, int type, int protocol) : _socket(std::make_unique<WebSock>(domain, type, protocol)) {}

        // Disable copying (unique_ptr cannot be copied)
        Client(const Client&)            = delete;
        Client& operator=(const Client&) = delete;

        // Allow move semantics
        Client(Client&&) noexcept            = default;
        Client& operator=(Client&&) noexcept = default;

        // Accessor for the socket (non-owning)
        WebSock* getSocket() const { return _socket.get(); }

        bool sendData(const char* data, size_t size)
        {
            if (!_socket)
                return false;
            return _socket->send(data, size);
        }

        bool receiveData(char* buffer, size_t size)
        {
            if (!_socket)
                return false;
            return _socket->receive(buffer, size);
        }

        bool connect(const char* address, const char* port) { return _socket->connect(address, port); }
        ~Client() = default;
    };

}  // namespace ctier
