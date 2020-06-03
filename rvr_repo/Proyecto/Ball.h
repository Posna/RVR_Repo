#include "Vector2D.h"
#include "Texture.h"
#include <SDL2_gfxPrimitives.h>

class Ball{

public:
  Ball(Vector2D pos, uint16_t radio = 5, uint32_t color = 0x51D1F6);

  void render();
  void update();
  void handleInput();
private:
  Texture* texture_;

  uint32_t color_;
  uint16_t radio_;
  Vector2D pos_;
  Vector2D vel_;
  uint16_t speed_;
};
