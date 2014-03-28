#include "Renderer.hpp"

GLFWwindow* Render::window = nullptr;

unsigned Render::bufferIndex = 0;
unsigned Render::colorburstBuffer = 0;
unsigned Render::CurrentFrame = 0;
unsigned Render::CurrentScanline = 0;
unsigned Render::defaultVAO = 0;
unsigned Render::programId = 0;
unsigned Render::scanlineIndex = 0;
unsigned Render::scanlineIndexLocation = 0;
unsigned Render::shapeBuffer = 0;

float Render::orthoMatrix[4][4] = { 0 };

Render::Pixel* Render::PixelOut = nullptr;


bool Render::ScanlineComplete = false;
bool Render::FrameComplete = false;