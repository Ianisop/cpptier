#if defined(_WIN32) || defined(_WIN64)
#define IS_WINDOWS 1
#else
#define IS_WINDOWS 0
#endif
#if (IS_WINDOWS)
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif
#include <arpa/inet.h>
#include <error.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
namespace ctier
{
    class WebSock
    {
      private:
        int _socket;

      public:
        WebSock(int domain, int type, int protocol)
        {
            if (IS_WINDOWS)
            {
                // TODO: implement for windows
            }
            else
            {
                _socket = socket(domain, type, protocol);
            }
        }
        WebSock(int clientFD) : _socket(clientFD) {}
        bool connect(const char* address, const char* port);  // should be used for clients
        bool bind(const char* address, const char* port);     // should be used for servers
        bool send(const char* data, size_t size);
        bool receive(char* buffer, size_t size);
        bool listen(int backlog = 5);
        void close_socket(bool reuse);
        int  accept(sockaddr_in* clientAddr = nullptr);
    };
}  // namespace ctier
