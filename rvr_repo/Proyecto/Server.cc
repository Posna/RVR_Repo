#include "Server.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

std::shared_mutex mutex_clients;
std::shared_mutex mutex_jugadores;
std::shared_mutex mutex_comida;

void Server::recieve_messages()
{
  //Recibir Mensajes en y en función del tipo de mensaje
  // - LOGIN: Añadir al vector clients y jugadores
  // - LOGOUT: Eliminar del vector clients y jugadores
  // - POSITION: Cambia la posicion de las bolas dentro del vector de jugadores
    socket.bind();
    char buffer[Ball::MESSAGE_SIZE];

    mutex_comida.lock();
    //Genera la comida aleatoria por el mapa
    for (size_t i = 0; i < NUM_BOLITAS; i++) {
      comida.push_back(getRandomBall(i));
    }
    mutex_comida.unlock();

    while (true)
    {
      Ball cm;
      Socket* s;
      socket.recv(cm, s);

      switch(cm.getType())
      {
        case Ball::LOGIN:{
          mutex_clients.lock();
          clients.push_back(s);
          mutex_clients.unlock();

          mutex_comida.lock_shared();
          for(Ball b: comida){
            b.setType(Ball::LOGIN);
            socket.send(b, *s);
          }
          mutex_comida.unlock_shared();

          mutex_jugadores.lock();
          cm.setId(id_actual);
          id_actual++;
          jugadores.push_back(cm);
          mutex_jugadores.unlock();

          cm.setType(Ball::ID);
          mutex_clients.lock_shared();
          socket.send(cm, *s);
          cm.setType(Ball::LOGIN);
          int i = 0;
          for (auto it = clients.begin(); it < clients.end(); it++)
          {
            if (!((*(*it)) == *s))
            {
              socket.send(cm, *(*it));
              mutex_jugadores.lock();
              jugadores[i].setType(Ball::LOGIN);
              mutex_jugadores.unlock();
              mutex_jugadores.lock_shared();
              socket.send(jugadores[i], *s);
              mutex_jugadores.unlock_shared();
            }
            i++;
          }
          mutex_clients.unlock_shared();
        }
        break;

        case Ball::LOGOUT:{
          int i = 0;
          mutex_clients.lock_shared();
          auto it = clients.begin();
          while(it != clients.end() && !((*(*it)) == *s))
          {
            i++;
            it++;
          }
          if(it == clients.end())
          {
            printf("No está conectado\n");
            mutex_clients.unlock_shared();
          }
          else{
            printf("Cliente %d desconectado\n", cm.getId());
            for(Socket* s1: clients){
              socket.send(cm, *s1);
            }
            mutex_clients.unlock_shared();
            mutex_clients.lock();
            clients.erase(it);
            cm.setType(Ball::DEAD);
            socket.send(cm, *s);
            mutex_clients.unlock();
            mutex_jugadores.lock();
            if(!jugadores[i].IsDead())
              jugadores.erase(jugadores.begin()+i);
            mutex_jugadores.unlock();
            delete *it;
          }

        }
        break;

        case Ball::POSITION:
        int i = 0;
        mutex_jugadores.lock_shared();
        auto it = clients.begin();
        while(jugadores.size() > i && cm.getId() != jugadores[i].getId()){
          i++;
        }
        int n = jugadores.size();
        mutex_jugadores.unlock_shared();
        if(i != n){
          mutex_jugadores.lock();
          jugadores[i].setPos(cm.getPos());
          mutex_jugadores.unlock();
        }
        break;
      }
    }
}

void Server::update(){
  while(true){
    collision_detection();
    //sleep(0.1); //Prueba de lag
    send_positions();
  }
}

//Comprueba colisiones primero entre jugadores y luego entre jugadores y comida
void Server::collision_detection()
{
  int x = 0, y = 0;
  mutex_jugadores.lock_shared();
  for(auto it = jugadores.begin(); it != jugadores.end(); ++it){
    Ball o1 = *it;
    y = 0;
    for(auto itt = jugadores.begin(); itt != jugadores.end(); ++itt){
      Ball o2 = *itt;
      if (o1.getId() != o2.getId() && !o1.IsDead() && !o2.IsDead())
      {
        Vector2D aux = o1.getPos() - o2.getPos();
        if((o1.getRadius() > o2.getRadius() + o1.getRadius()/4)&&
        (aux.magnitude() < o1.getRadius()))
        {
          mutex_clients.lock_shared();
          o2.setType(Ball::DEAD);
          jugadores[y].kill();
          socket.send(o2, *clients[y]);
          o1.setType(Ball::EAT);
          o1.addRadius(o2.getRadius()/5);
          socket.send(o1, *clients[x]);
          o2.setType(Ball::LOGOUT);
          for(Socket* s1: clients){
            socket.send(o2, *s1);
          }
          mutex_clients.unlock_shared();

          eliminador.push_back(y);

          mutex_jugadores.unlock_shared();
          mutex_jugadores.lock();
          jugadores[x].setRadius(o1.getRadius());
          mutex_jugadores.unlock();
          mutex_jugadores.lock_shared();
        }
      }
      y++;
    }
    y = 0;
    mutex_comida.lock_shared();
    for(Ball b: comida){
      Vector2D aux = o1.getPos() - b.getPos();
      if((aux.magnitude() < o1.getRadius() + b.getRadius()))
      {
        mutex_clients.lock_shared();
        float incremento = (50.0f*b.getRadius())/(49.0f * o1.getRadius()+ b.getRadius());
        o1.setType(Ball::EAT);
        o1.addRadius(incremento);
        socket.send(o1, *clients[x]);
        mutex_clients.unlock_shared();

        mutex_jugadores.unlock_shared();
        mutex_jugadores.lock();
        jugadores[x].addRadius(incremento);
        mutex_jugadores.unlock();
        mutex_jugadores.lock_shared();

        mutex_comida.unlock_shared();
        mutex_comida.lock();
        comida[y] = getRandomBall(comida[y].getId());
        comida[y].setType(Ball::POSITION);
        mutex_comida.unlock();
        mutex_comida.lock_shared();

        mutex_clients.lock_shared();
        for(Socket* s: clients){
          socket.send(comida[y], *s);
        }
        mutex_clients.unlock_shared();
      }
      y++;
    }
    mutex_comida.unlock_shared();
    x++;
  }
  //Elimina todas la bolas que hayan muerto en esta iteracion
  mutex_jugadores.unlock_shared();
  for(int i = 0; i < eliminador.size(); i++){
    mutex_jugadores.lock();
    jugadores.erase(jugadores.begin() + (eliminador[i]-i));
    mutex_jugadores.unlock();
    mutex_clients.lock();
    clients.erase(clients.begin() + (eliminador[i]-i));
    mutex_clients.unlock();
  }
  eliminador.clear();
}

//Envia la posicion de cada jugador a los demas jugadores
void Server::send_positions()
{
  int i = 0;
  mutex_jugadores.lock_shared();
  for(auto it = jugadores.begin(); it != jugadores.end(); ++it){
    Ball o1 = *it;
    for(auto itt = jugadores.begin(); itt != jugadores.end(); ++itt){
      Ball o2 = *itt;
      if (o1.getId() != o2.getId())
      {
        mutex_clients.lock_shared();
        o2.setType(Ball::POSITION);
        socket.send(o2, *clients[i]);
        mutex_clients.unlock_shared();
      }
    }
    i++;
  }
  mutex_jugadores.unlock_shared();
}

Ball Server::getRandomBall(){
  int x = rand() % LEVEL_WIDTH;
  int y = rand() % LEVEL_HEIGHT;
  return Ball(Vector2D(x, y), true);

}

Ball Server::getRandomBall(int id){
  Ball b = getRandomBall();
  b.setId(id);
  return b;
}
