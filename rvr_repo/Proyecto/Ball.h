#include "Game.h"
#include <SDL2/SDL2_gfxPrimitives.h>

class Ball{

public:
  Ball(Vector2D pos, Texture* t, uint16_t radio = 5);
  Ball(Vector2D pos, bool r = true, uint16_t radio = 5, uint32_t color = 0x550000FF); //Las primeras dos (0xFF..)son el alpha
  ~Ball();

  void render(SDL_Renderer* cam);
  void update(uint32_t frameTime);
  void handleInput();

  void addRadius(uint16_t r) { radio_+=r; }
  Vector2D getPos() { return pos_;}
  void desfase(Vector2D d);

  void setGame(Game* g) { g_ = g; };
private:
  Game* g_;
  Texture* texture_;
  SDL_Texture* t_;
  Vector2D desfase_ = Vector2D(0, 0);
  uint32_t color_;
  uint16_t radio_;
  Vector2D pos_;
  Vector2D vel_;
  uint16_t speed_ = 1;
};
