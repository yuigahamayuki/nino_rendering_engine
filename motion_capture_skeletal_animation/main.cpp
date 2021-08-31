#include "engine.h"

int main(int argc, char** argv) {
  motion_animation::Engine engine(1920, 1080);
  engine.SetRendererToOpenGL();
  engine.LoadScene();
  engine.Run();

  return 0;
}