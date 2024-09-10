#include <netdb.h>
#include<stdio.h>
int main()
{
    const char host[] = "www.google.com"; // an IP address or a hostname, like "www.google.com" by itself

    struct addrinfo hints = {0};
    hints.ai_flags = AI_NUMERICHOST;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    struct addrinfo *addr = NULL;
    int ret = getaddrinfo(host, NULL, &hints, &addr);
    if (ret == EAI_NONAME) // not an IP, retry as a hostname
    {
        hints.ai_flags = 0;
        ret = getaddrinfo(host, NULL, &hints, &addr);
    }
    if (ret == 0)
    {
        //target = *(sockaddr_in*)(addr->ai_addr);
        freeaddrinfo(addr);
    }
    return 0;
}
