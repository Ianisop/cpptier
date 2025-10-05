#pragma once
#include <sys/types.h>

#include <memory>

#include "websock.h"

namespace ctier
{

    class Server
    {
      private:
        std::unique_ptr<WebSock> _listening_socket;

      public:
        // creates the socket on the heap and takes ownership
        Server(const char* address, const char* port, int domain, int type, int protocol) :
            _listening_socket(std::make_unique<WebSock>(domain, type, protocol))
        {
            if (!_listening_socket->bind(address, port))
                throw std::runtime_error("Bind failed");

            if (!_listening_socket->listen())
                throw std::runtime_error("Listen failed");

            printf("Listening on %s:%s\n", address, port);
        }

        // Disable copying (unique_ptr cannot be copied)
        Server(const Server&)            = delete;
        Server& operator=(const Server&) = delete;

        // Allow move semantics
        Server(Server&&) noexcept            = default;
        Server& operator=(Server&&) noexcept = default;

        // Accessor for the socket (non-owning)
        WebSock* getSocket() const { return _listening_socket.get(); }

        bool sendData(const char* data, size_t size)
        {
            if (!_listening_socket)
                return false;
            return _listening_socket->send(data, size);
        }

        bool receiveData(char* buffer, size_t size)
        {
            if (!_listening_socket)
                return false;
            return _listening_socket->receive(buffer, size);
        }

        void stop() { _listening_socket->close_socket(false); }

        // automatically deletes the WebSock instance
        ~Server() = default;
    };

}  // namespace ctier
