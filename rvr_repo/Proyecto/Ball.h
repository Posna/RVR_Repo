#include "Vector2D.h"
#include "Texture.h"
#include <SDL2/SDL2_gfxPrimitives.h>

class Ball{

public:
  Ball(Vector2D pos, Texture* t, uint16_t radio = 5);
  Ball(Vector2D pos, uint16_t radio = 5, uint32_t color = 0x550000FF); //Las primeras dos (0xFF..)son el alpha
  ~Ball();

  void render(SDL_Renderer* cam);
  void update();
  void handleInput();

  void addRadius(uint16_t r) { radio_+=r; }
private:
  Texture* texture_;
  SDL_Texture* t_;

  uint32_t color_;
  uint16_t radio_;
  Vector2D pos_;
  Vector2D vel_;
  uint16_t speed_ = 1;
};
