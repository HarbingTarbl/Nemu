#include "Renderer.hpp"

GLFWwindow* Render::window = nullptr;

unsigned Render::bufferIndex = 0;
unsigned Render::CurrentFrame = 0;
unsigned Render::CurrentScanline = 0;
unsigned Render::defaultVAO = 0;
unsigned Render::programId = 0;
unsigned Render::scanlineIndex = 0;
unsigned Render::scanlineIndexLocation = 0;
unsigned Render::shapeBuffer = 0;
unsigned Render::yiqMatrixLocation = 0;
unsigned Render::ppuPhaseLocation = 0;

float Render::orthoMatrix[4][4] = { 0 };
float Render::yiqMatrix[3][3] = { 0 };

std::array<unsigned, 10> Render::colorburstBuffers;
std::array<unsigned, 10> Render::colorburstVertexAO;
Render::clock::time_point Render::lastFrame = clock::now();


Render::Pixel* Render::PixelOut = nullptr;


bool Render::ScanlineComplete = false;
bool Render::FrameComplete = false;