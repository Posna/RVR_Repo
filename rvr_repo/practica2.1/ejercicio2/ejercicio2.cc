#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <time.h>

#define FALSE 0
#define TRUE 1

size_t hora(char* buf, size_t max){
  time_t t;
  t = time(NULL);
  char format[] = "%r";

  return strftime(buf, max, format , localtime(&t));
}

size_t fecha(char* buf, size_t max){
  time_t t;
  t = time(NULL);
  char format[] = "%F";
  return strftime(buf, max, format, localtime(&t));
}

int main(int argc, char **argv)
{
  struct addrinfo hints;
  struct addrinfo * res;

  // ---------------------------------------------------------------------- //
  // INICIALIZACIÓN SOCKET & BIND //
  // ---------------------------------------------------------------------- //

  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

  if ( rc != 0 )
  {
    std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
    return -1;
  }

  // res contiene la representación como sockaddr de dirección + puerto

  int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
  {
    std::cerr << "bind: " << std::endl;
    return -1;
  }

  freeaddrinfo(res);

  // ---------------------------------------------------------------------- //
  // RECEPCIÓN MENSAJE DE CLIENTE //
  // ---------------------------------------------------------------------- //
  char buffer[40];
  char respuesta[40];
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  struct sockaddr client_addr;
  socklen_t client_len = sizeof(struct sockaddr);
  int salir = TRUE;

  while (salir) {

    //Espera hasta recibir mensaje del cliente
    ssize_t bytes = recvfrom(sd, buffer, 39 * sizeof(char), 0, &client_addr,
    &client_len);

    if ( bytes == -1)
    {
      std::cerr << "recvfrom: " << std::endl;
      return -1;
    }

    getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
    NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

    std::cout << bytes << " bytes de " << host << ":" << service << std::endl;

    // ---------------------------------------------------------------------- //
    // RESPUESTA AL CLIENTE //
    // ---------------------------------------------------------------------- //

    if(strcmp(buffer, "t") == 0){
      size_t tbytes = hora(respuesta, 39 * sizeof(char));
      sendto(sd, respuesta, tbytes+1, 0, &client_addr, client_len);
    }
    else if(strcmp(buffer, "d") == 0){
      size_t tbytes = fecha(respuesta, 39 * sizeof(char));
      sendto(sd, respuesta, tbytes+1, 0, &client_addr, client_len);
    }
    else if(strcmp(buffer, "q") == 0){
      char saliendo[] = "Saliendo...";
      sendto(sd, saliendo, strlen(saliendo) + 1, 0, &client_addr, client_len);
      salir = FALSE;
    }
    else{
      char warn[] = "Comando no soportado ";
      size_t tam = bytes + strlen(warn) + 1;
      strcat(warn, buffer);
      sendto(sd, warn, tam, 0, &client_addr, client_len);
    }
    memset(buffer, 0, sizeof buffer);
  }
  std::cout << "Saliendo..." << std::endl;
  return 0;
}
