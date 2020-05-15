#include <string.h>

#include "Serializable.h"
#include "Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    //Construir un socket de tipo AF_INET y SOCK_DGRAM usando getaddrinfo.
    //Con el resultado inicializar los miembros sd, sa y sa_len de la clase
    struct addrinfo hints;
    struct addrinfo * res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(address, port, &hints, &res);
    if ( rc != 0 )
    {
    std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
    }

    sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sd < 0)
    {
      std::cerr << "socket error: \n";
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    sa_len = sizeof(struct sockaddr);
    getnameinfo(&sa, sa_len, host, NI_MAXHOST, service,
      NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
}

int Socket::recv(Serializable &obj, Socket * &sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &sa, &sa_len);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    obj.to_bin();
    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
    NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

    sendto(sock.sd, obj.data(), obj.size(), 0, &client_addr, client_len);
    //Serializar el objeto
    //Enviar el objeto binario a sock usando el socket sd
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere
    struct sockaddr_in* aux1 = (struct sockaddr_in *)(&s1.sa);
    struct sockaddr_in* aux2 = (struct sockaddr_in *)&s2.sa;
    return (aux1->sin_family == aux2->sin_family &&
      aux1->sin_addr.s_addr == aux2->sin_addr.s_addr &&
      aux1->sin_port == aux2->sin_port);
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};
