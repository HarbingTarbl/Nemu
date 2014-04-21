#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_




#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <array>
#include <chrono>
#include <thread>

#include "NES.hpp"

#ifdef _WIN32
#pragma comment(lib, "XInput9_1_0.lib")
#define _X86_
#include <XInput.h>
#endif


class Render
{
public:
	using clock = std::chrono::system_clock;

	static GLFWwindow* window;
	
	static clock::time_point lastFrame;

	static unsigned programId;
	
	static unsigned shapeBuffer;
	static std::array<unsigned, 2> colorburstBuffers;
	static std::array<unsigned, 2> colorburstVertexAO;
	static unsigned bufferIndex;

	static unsigned scanlineIndex;
	static unsigned scanlineIndexLocation;

	static unsigned colorTexture;

	static unsigned defaultVAO;

	static float orthoMatrix[4][4];
	static float yiqMatrix[3][3];

	static unsigned yiqMatrixLocation;
	static unsigned ppuPhaseLocation;
	static unsigned clearColorLocation;


public:

	static unsigned ControllerStrobe;

	enum class ButtonState : unsigned int
	{
		A,
		B,
		SELECT,
		START,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum class ButtonTranslate : unsigned int
	{
		A = GLFW_KEY_LEFT,
		B = GLFW_KEY_DOWN,
		SELECT = GLFW_KEY_BACKSPACE,
		START = GLFW_KEY_ENTER,
		UP = GLFW_KEY_W,
		DOWN = GLFW_KEY_S,
		LEFT = GLFW_KEY_A,
		RIGHT = GLFW_KEY_D
	};

	static int ControllerStatus()
	{
#ifdef _WIN32
		XINPUT_STATE state{ 0 };

		if (XInputGetState(0, &state) == 0)
		{
			switch ((ButtonState)ControllerStrobe++)
			{
			case ButtonState::A:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
			case ButtonState::B:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
			case ButtonState::SELECT:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
			case ButtonState::START:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
			case ButtonState::UP:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
			case ButtonState::DOWN:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
			case ButtonState::LEFT:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
			case ButtonState::RIGHT:
				return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
			}
		}
#endif


#define BUTTON( mx ) case (unsigned)ButtonState::mx: return glfwGetKey(window, ((int)ButtonTranslate::mx))
		
		switch (ControllerStrobe++)
		{
			BUTTON(A);
			BUTTON(B);
			BUTTON(SELECT);
			BUTTON(START);
			BUTTON(UP);
			BUTTON(DOWN);
			BUTTON(LEFT);
			BUTTON(RIGHT);
		}

#undef BUTTON
	}


	static void Initalize(int windowWidth, int windowHeight)
	{
		using namespace std;
		if (!window)
		{
			CurrentFrame = 0;
			if(!glfwInit())
				throw runtime_error("Unable to initalize GLFW3");

			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_RESIZABLE, false);
			

			window = glfwCreateWindow(windowWidth, windowHeight, "NES Emulator", nullptr, nullptr);
			if(!window)
				throw runtime_error("Unable to create window");
				
			glfwMakeContextCurrent(window);
			glewExperimental = true;
			glewInit();

			int majorVersion, minorVersion;
			glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
			glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

			cout << "Initalized " << majorVersion << "." << minorVersion << endl;
			programId = glCreateProgram();
			unsigned vertex = glCreateShader(GL_VERTEX_SHADER);
			unsigned fragment = glCreateShader(GL_FRAGMENT_SHADER);
		
			vector<char> sourceBuffer;
			fstream fileHandle;
			fileHandle.open("ntsc-vertex.gl", fileHandle.in | fileHandle.binary);
			fileHandle.seekg(0, fileHandle.end);
			sourceBuffer.resize(1 + fileHandle.tellg());
			sourceBuffer[sourceBuffer.size() - 1] = 0;
			fileHandle.seekg(0, fileHandle.beg);
			fileHandle.read(sourceBuffer.data(), sourceBuffer.size() - 1);
			fileHandle.close();

			{
				const char* b = sourceBuffer.data();
				glShaderSource(vertex, 1, &b, nullptr);
			}

			fileHandle.open("ntsc-fragment.gl", fileHandle.in | fileHandle.binary);
			fileHandle.seekg(0, fileHandle.end);
			sourceBuffer.resize(1 + fileHandle.tellg());
			sourceBuffer[sourceBuffer.size() - 1] = 0;
			fileHandle.seekg(0, fileHandle.beg);
			fileHandle.read(sourceBuffer.data(), sourceBuffer.size() - 1);
			fileHandle.close();

			{
				const char* b = sourceBuffer.data();
				glShaderSource(fragment, 1, &b, nullptr);
			}

			int compileStatus;
			glCompileShader(fragment);
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &compileStatus);
			if (!compileStatus)
			{
				int logSize;
				glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &logSize);
				string shaderError;
				shaderError.resize(logSize);
				glGetShaderInfoLog(fragment, logSize, nullptr, &shaderError[0]);
				throw runtime_error(shaderError);
			}

			glCompileShader(vertex);
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &compileStatus);
			if (!compileStatus)
			{
				int logSize;
				glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &logSize);
				string shaderError;
				shaderError.resize(logSize);
				glGetShaderInfoLog(vertex, logSize, nullptr, &shaderError[0]);
				throw runtime_error(shaderError);
			}

			glAttachShader(programId, fragment);
			glAttachShader(programId, vertex);
			
			glLinkProgram(programId);
		
			glUseProgram(programId);

			glGenBuffers(colorburstBuffers.size(), colorburstBuffers.data());
			glGenVertexArrays(colorburstVertexAO.size(), colorburstVertexAO.data());
			glGenBuffers(1, &shapeBuffer);
			const float shape[] = {
				0, 0, 0, 1,
				1, 0, 0, 1,
				0, 1, 0, 1,
				1, 1, 0, 1
			};

			glBindBuffer(GL_ARRAY_BUFFER, shapeBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

			auto& cbb = colorburstBuffers;
			auto& cbv = colorburstVertexAO;

			for (int i = 0; i < colorburstBuffers.size(); i++)
			{
				glBindBuffer(GL_ARRAY_BUFFER, cbb[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Render::Pixel) * 256 * 240, nullptr, GL_STREAM_DRAW);
				glBindVertexArray(cbv[i]);
				
				glEnableVertexAttribArray(0);
				glVertexAttribIPointer(0, 1, GL_SHORT, 0, 0);
				glVertexAttribDivisor(0, 1);

				glBindBuffer(GL_ARRAY_BUFFER, shapeBuffer);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);
			}


			float matrix[] = {
				 2.0 / 256, 0, 0, -1 ,
				 0, -2.0 / 240, 0, 1 ,
				 0, 0, -2.0 / 20, 1 ,
				 0, 0, 0, 1 ,
			};


			glUniformMatrix4fv(glGetUniformLocation(programId, "PMatrix"), 1, true, matrix);
			scanlineIndexLocation = glGetUniformLocation(programId, "Scanline");
			yiqMatrixLocation = glGetUniformLocation(programId, "YIQMatrix");
			ppuPhaseLocation = glGetUniformLocation(programId, "PPUPhase");
			clearColorLocation = glGetUniformLocation(programId, "ClearColor");

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);

			glEnable(GL_BLEND);
			glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
			glClearColor(0, 0, 0, 0);
			FrameComplete = true;
			glfwSwapInterval(0);

		}
	}

	static void UploadYIQMatrix()
	{

	}

	static void Terminate()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	static void BeginFrame()
	{
		//if ((CurrentFrame % 1) == 0)
			glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_F1))
		{
			NES::Instance->mPPU.DumpVRAM();
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, 1);
	}

	static void EndFrame()
	{
		CurrentScanline = 0;
	}

	static void BeginScanline(int PPUCycle)
	{
		if (!FrameComplete)
			return;

		//gl::Uniform1i(ppuPhaseLocation, (PPUCycle * 12) % 12);

		glBindBuffer(GL_ARRAY_BUFFER, colorburstBuffers[CurrentFrame % 2]);

		BasePixel = PixelOut = (Pixel*)glMapBufferRange(GL_ARRAY_BUFFER,
			0, 256 * 240 * sizeof(Render::Pixel),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		FrameComplete = false;
	}

	static void SetClearColor(unsigned color)
	{
		glUniform1i(clearColorLocation, color);
	}

	static void EndScanline()
	{
		if (PixelOut == nullptr)
			return;

		if (CurrentScanline != -1)
		{	
			ScanlineComplete = true;
			FrameComplete = false;
		}
		else
		{
			if (FrameComplete)
				return;

			glBindBuffer(GL_ARRAY_BUFFER, colorburstBuffers[CurrentFrame % 2]);
			glUnmapBuffer(GL_ARRAY_BUFFER);

			glBindVertexArray(colorburstVertexAO[CurrentFrame % 2]);
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 256 * 240);
			glfwSwapBuffers(window);
			glClear(GL_COLOR_BUFFER_BIT);
			FrameComplete = true;
			Render::CurrentFrame++;
			auto old = lastFrame;
			lastFrame = clock::now();
			//std::cout << "FPS : " << 1.0f / std::chrono::duration_cast<std::chrono::duration<float>>(lastFrame - old).count() << std::endl;
		}
		//CurrentScanline++;
		//CurrentScanline %= 256;
	}

	static bool WindowOpen()
	{
		return !glfwWindowShouldClose(window);
	}


	struct Pixel
	{
		uint16_t Color;
	};
	
	static Pixel* PixelOut;
	static Pixel* BasePixel;
	
	static unsigned CurrentScanline;
	static unsigned CurrentFrame;

	static bool FrameComplete;
	static bool ScanlineComplete;
};


#endif
