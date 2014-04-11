#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#define GLFW_INCLUDE_NONE
#include "gl_core_3_3.hpp"
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
#define BUTTON( mx ) case (unsigned)ButtonState::##mx: return glfwGetKey(window, ((int)ButtonTranslate::##mx))
		
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
			glfwInit();

			glfwWindowHint(GLFW_RESIZABLE, false);
			//glfwWindowHint(GLFW_DECORATED, false);


			window = glfwCreateWindow(windowWidth, windowHeight, "NES Emulator", nullptr, nullptr);
			glfwMakeContextCurrent(window);
			gl::sys::LoadFunctions();
			cout << "Initalized " << gl::sys::GetMajorVersion() << "." << gl::sys::GetMinorVersion() << endl;
			programId = gl::CreateProgram();
			unsigned vertex = gl::CreateShader(gl::VERTEX_SHADER);
			unsigned fragment = gl::CreateShader(gl::FRAGMENT_SHADER);
		
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
				gl::ShaderSource(vertex, 1, &b, nullptr);
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
				gl::ShaderSource(fragment, 1, &b, nullptr);
			}

			int compileStatus;
			gl::CompileShader(fragment);
			gl::GetShaderiv(fragment, gl::COMPILE_STATUS, &compileStatus);
			if (!compileStatus)
			{
				int logSize;
				gl::GetShaderiv(fragment, gl::INFO_LOG_LENGTH, &logSize);
				string shaderError;
				shaderError.resize(logSize);
				gl::GetShaderInfoLog(fragment, logSize, nullptr, &shaderError[0]);
				throw runtime_error(shaderError);
			}

			gl::CompileShader(vertex);
			gl::GetShaderiv(vertex, gl::COMPILE_STATUS, &compileStatus);
			if (!compileStatus)
			{
				int logSize;
				gl::GetShaderiv(vertex, gl::INFO_LOG_LENGTH, &logSize);
				string shaderError;
				shaderError.resize(logSize);
				gl::GetShaderInfoLog(vertex, logSize, nullptr, &shaderError[0]);
				throw runtime_error(shaderError);
			}

			gl::AttachShader(programId, fragment);
			gl::AttachShader(programId, vertex);
			
			gl::LinkProgram(programId);
		
			gl::UseProgram(programId);

			gl::GenBuffers(colorburstBuffers.size(), colorburstBuffers.data());
			gl::GenVertexArrays(colorburstVertexAO.size(), colorburstVertexAO.data());
			gl::GenBuffers(1, &shapeBuffer);
			const float shape[] = {
				0, 0, 0, 1,
				1, 0, 0, 1,
				0, 1, 0, 1,
				1, 1, 0, 1
			};

			gl::BindBuffer(gl::ARRAY_BUFFER, shapeBuffer);
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(shape), shape, gl::STATIC_DRAW);

			auto& cbb = colorburstBuffers;
			auto& cbv = colorburstVertexAO;

			for (int i = 0; i < colorburstBuffers.size(); i++)
			{
				gl::BindBuffer(gl::ARRAY_BUFFER, cbb[i]);
				gl::BufferData(gl::ARRAY_BUFFER, sizeof(Render::Pixel) * 256 * 240, nullptr, gl::STREAM_DRAW);
				gl::BindVertexArray(cbv[i]);
				
				gl::EnableVertexAttribArray(0);
				gl::VertexAttribIPointer(0, 1, gl::SHORT, 0, 0);
				gl::VertexAttribDivisor(0, 1);

				gl::BindBuffer(gl::ARRAY_BUFFER, shapeBuffer);
				gl::EnableVertexAttribArray(1);
				gl::VertexAttribPointer(1, 4, gl::FLOAT, false, 0, 0);
			}


			float matrix[] = {
				 2.0 / 256, 0, 0, -1 ,
				 0, -2.0 / 240, 0, 1 ,
				 0, 0, -2.0 / 20, 1 ,
				 0, 0, 0, 1 ,
			};


			gl::UniformMatrix4fv(gl::GetUniformLocation(programId, "PMatrix"), 1, true, matrix);
			scanlineIndexLocation = gl::GetUniformLocation(programId, "Scanline");
			yiqMatrixLocation = gl::GetUniformLocation(programId, "YIQMatrix");
			ppuPhaseLocation = gl::GetUniformLocation(programId, "PPUPhase");
			clearColorLocation = gl::GetUniformLocation(programId, "ClearColor");

			gl::Disable(gl::DEPTH_TEST);
			gl::Disable(gl::CULL_FACE);

			gl::Enable(gl::BLEND);
			gl::BlendEquationSeparate(gl::FUNC_ADD, gl::MAX);
			gl::BlendFuncSeparate(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA, gl::ONE, gl::ONE);
			gl::ClearColor(0, 0, 0, 0);
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
		if ((CurrentFrame % 20) == 0)
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

		gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffers[CurrentFrame % 2]);

		BasePixel = PixelOut = (Pixel*)gl::MapBufferRange(gl::ARRAY_BUFFER, 
			0, 256 * 240 * sizeof(Render::Pixel),
			gl::MAP_WRITE_BIT | gl::MAP_INVALIDATE_BUFFER_BIT | gl::MAP_UNSYNCHRONIZED_BIT);
		FrameComplete = false;
	}

	static void SetClearColor(unsigned color)
	{
		gl::Uniform1i(clearColorLocation, color);
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

			gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffers[CurrentFrame % 2]);
			gl::UnmapBuffer(gl::ARRAY_BUFFER);

			gl::BindVertexArray(colorburstVertexAO[CurrentFrame % 2]);
			gl::DrawArraysInstanced(gl::TRIANGLE_STRIP, 0, 4, 256 * 240);
			glfwSwapBuffers(window);
			gl::Clear(gl::COLOR_BUFFER_BIT);
			FrameComplete = true;
			Render::CurrentFrame++;
			auto old = lastFrame;
			lastFrame = clock::now();
			std::cout << "FPS : " << 1.0f / std::chrono::duration_cast<std::chrono::duration<float>>(lastFrame - old).count() << std::endl;
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