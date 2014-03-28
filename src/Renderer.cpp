#include "Renderer.hpp"

GLFWwindow* Render::window = nullptr;
unsigned Render::bufferIndex = 0;
unsigned Render::colorburstBuffer = 0;
unsigned Render::CurrentFrame = 0;
unsigned Render::CurrentScanline = 0;
bool Render::FrameComplete = 0;