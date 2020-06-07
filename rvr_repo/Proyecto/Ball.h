#include "Game.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Serializable"

class Ball: public Serializable{

public:
  static const size_t MESSAGE_SIZE = sizeof(uint16_t)*3 + sizeof(uint8_t);

  enum MessageType
  {
      LOGIN   = 0,
      POSITION = 1,
      LOGOUT  = 2
  };

  Ball(Vector2D pos, Texture* t, uint16_t radio = 5);
  Ball(Vector2D pos, bool r = true, uint16_t radio = 5, uint32_t color = 0x550000FF); //Las primeras dos (0xFF..)son el alpha
  ~Ball();

  void render(SDL_Renderer* cam);
  void update(uint32_t frameTime);
  void handleInput();

  virtual void to_bin();
  virtual void from_bin(char * data);

  uint16_t getRadius();
  void setRadius(uint16_t r);
  void addRadius(uint16_t r);
  //Este metodo se usa para que el jugador sea el punto medio
  //y todo vaya en funcion del mismo
  void desfase(Vector2D d);

  Vector2D getPos();
  void setPos(Vector2D v);

  void setGame(Game* g);
private:
  Game* g_;
  Texture* texture_;
  SDL_Texture* t_;
  Vector2D desfase_ = Vector2D(0, 0);
  uint32_t color_;
  Vector2D vel_;
  uint16_t speed_ = 1;

  //Variables a serializar
  uint8_t type;
  Vector2D pos_;
  uint16_t radio_;
};