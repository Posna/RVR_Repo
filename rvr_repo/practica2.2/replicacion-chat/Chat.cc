#include "Chat.h"

void ChatMessage::to_bin()
{
    //Serializar los campos type, nick y message en el buffer _data
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    _size = MESSAGE_SIZE;
    char* tmp = _data;

    memcpy(tmp, &type, 1);
    tmp += sizeof(uint8_t);
    nick[8] = '\n';
    memcpy(tmp, nick.c_str(), 8);
    tmp += 8*sizeof(char);
    message[80] = '\0';
    memcpy(tmp, message.c_str(), 80);
    tmp += 80*sizeof(char);
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);
    _size = MESSAGE_SIZE;
    //Reconstruir la clase usando el buffer _data

    char* tmp = _data;

    memcpy(&type, tmp, 1);
    tmp += sizeof(uint8_t);

    memcpy(&nick[0], tmp, 8*sizeof(char));
    tmp += 8*sizeof(char);
    nick[8] = '\0';

    memcpy(&message[0], tmp, 80*sizeof(char));
    message[80] = '\0';
    //tmp += 80*sizeof(char);
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
    socket.bind();
    char buffer[ChatMessage::MESSAGE_SIZE];

    while (true)
    {
      //cm = ChatMessage();
      ChatMessage cm;
      Socket* s;
      socket.recv(cm, s);
      switch(cm.type)
      {
        case ChatMessage::LOGIN:
        printf("%s Se ha conectado\n", &cm.nick[0]);
        clients.push_back(s);
        break;

        case ChatMessage::LOGOUT:{
          auto it = clients.begin();
          while(it != clients.end() && !(*(*it) == *s))
          {
            it++;
          }
          if(it == clients.end())
          {
            printf("No está conectado\n");
          }
          else{
            printf("%s Se ha desconectado\n", &cm.nick[0]);
            clients.erase(it);
            delete *it;
            //*it = nullptr;
          }
        }
        break;

        case ChatMessage::MESSAGE:
        auto it = clients.begin();
        for (Socket* a: clients){
          if(!(*a == *s)){
            socket.send(cm, *a);
          }
        }
        break;
      }
    }
}

void ChatClient::login()
{
    std::string msg("-SE HA UNIDO-");
    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
    em.type = ChatMessage::MESSAGE;
    socket.send(em, socket);
}

void ChatClient::logout()
{
  std::string msg("-SE HA DESCONECTADO-");

  ChatMessage em(nick, msg);
  em.type = ChatMessage::MESSAGE;
  socket.send(em, socket);

  em.type = ChatMessage::LOGOUT;
  socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (!terminar)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin, msg);

        ChatMessage cm (nick, msg);
        cm.type = ChatMessage::MESSAGE;
        if(msg.compare("/q") == 0){
          logout();
          terminar = true;
        }
        else
          socket.send(cm, socket);
    }
}

void ChatClient::net_thread()
{
    while(!terminar)
    {
      //Recibir Mensajes de red
      //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
      ChatMessage cm;
      Socket* s;
      socket.recv(cm);
      std::cout << &cm.nick[0] << ": " << &cm.message[0] << "\n";
    }
}
