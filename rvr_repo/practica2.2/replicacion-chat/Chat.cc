#include "Chat.h"

void ChatMessage::to_bin()
{
    //Serializar los campos type, nick y message en el buffer _data
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    char* tmp = _data;

    memcpy(tmp, &type, 1);
    tmp += sizeof(uint8_t);

    memcpy(tmp, &nick, 8);
    tmp += 8*sizeof(char);

    memcpy(tmp, &message, 80);
    tmp += 80*sizeof(char);
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data

    char* tmp = _data;

    memcpy(&type, tmp, 1);
    tmp += sizeof(uint8_t);

    memcpy(&nick, tmp, 8);
    tmp += 8*sizeof(char);

    memcpy(&message, tmp, 80);
    tmp += 80*sizeof(char);
    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
  //Recibir Mensajes en y en función del tipo de mensaje
  // - LOGIN: Añadir al vector clients
  // - LOGOUT: Eliminar del vector clients
  // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
    char buffer[ChatMessage::MESSAGE_SIZE];
    while (true)
    {
      ChatMessage cm;
      Socket* s;
      std::cout << "Pre recv\n";
      socket.recv(cm, s);
      std::cout << "Mensaje\n";
      switch(cm.type)
      {
        case ChatMessage::LOGIN:
        clients.push_back(s);
        break;

        case ChatMessage::LOGOUT:{
        auto it = clients.begin();
        while(it != clients.end() && *it != s)
        {
          it++;
        }
        if(it == clients.end())
        {
          printf("No está conectado\n");
        }
        else{
          clients.erase(it);
          delete *it;
          *it = nullptr;
        }
      }
        break;

        case ChatMessage::MESSAGE:
        auto it = clients.begin();
        while(it != clients.end())
        {
          if(*it != s)
            socket.send(cm, *(*it));
          it++;
        }
        break;
      }
    }
}

void ChatClient::login()
{
    std::string msg;
    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
  std::string msg;

  ChatMessage em(nick, msg);
  em.type = ChatMessage::LOGOUT;

  socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin, msg);

        ChatMessage cm (nick, msg);
        cm.type = ChatMessage::MESSAGE;
        socket.send(cm, socket);
        std::cout << "Enviado mensaje\n";
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
      //Recibir Mensajes de red
      //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
      ChatMessage cm;
      Socket* s;
      socket.recv(cm);
      std::cout << cm.nick << ": " << cm.message << "\n";
    }
}
