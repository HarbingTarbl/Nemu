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

#include "NES.hpp"


class Render
{
private:
	static GLFWwindow* window;
	
	static unsigned programId;
	
	static unsigned shapeBuffer;
	static unsigned colorburstBuffer;
	static unsigned bufferIndex;

	static unsigned scanlineIndex;
	static unsigned scanlineIndexLocation;

	static unsigned colorTexture;

	static unsigned defaultVAO;

	static float orthoMatrix[4][4];
	static float yiqMatrix[3][3];

	static unsigned yiqMatrixLocation;
	static unsigned ppuPhaseLocation;


public:
	static void Initalize(int windowWidth, int windowHeight)
	{
		using namespace std;
		if (!window)
		{
			glfwInit();
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

			gl::GenBuffers(1, &colorburstBuffer);
			gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffer);
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(Render::Pixel) * 256, nullptr, gl::STREAM_DRAW);

			gl::GenVertexArrays(1, &defaultVAO);
			gl::BindVertexArray(defaultVAO);

			gl::EnableVertexAttribArray(0);
			gl::VertexAttribIPointer(0, 1, gl::SHORT, 0, 0);
			gl::VertexAttribDivisor(0, 1);

			gl::GenBuffers(1, &shapeBuffer);
			gl::BindBuffer(gl::ARRAY_BUFFER, shapeBuffer);
			const float shape[] = {
				0, 0, 0, 1,
				1, 0, 0, 1,
				0, 1, 0, 1,
				1, 1, 0, 1
			};
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(shape), shape, gl::STATIC_DRAW);
			gl::EnableVertexAttribArray(1);
			gl::VertexAttribPointer(1, 4, gl::FLOAT, false, 0, 0);

			gl::Enable(gl::VERTEX_PROGRAM_POINT_SIZE);

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

			gl::Disable(gl::DEPTH_TEST);
			gl::Disable(gl::CULL_FACE);
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
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_F1))
		{
			NES::Instance->mPPU.DumpVRAM();
		}
	}

	static void EndFrame()
	{
		CurrentFrame++;
		CurrentScanline = 0;
		FrameComplete = true;


	}

	static void BeginScanline(int PPUCycle)
	{
		if (!ScanlineComplete)
			EndScanline();

		//gl::Uniform1i(ppuPhaseLocation, (PPUCycle * 12) % 12);
		gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffer);
		gl::BufferData(gl::ARRAY_BUFFER, sizeof(Render::Pixel) * 256, nullptr, gl::STREAM_DRAW);
		PixelOut = (Pixel*)gl::MapBufferRange(gl::ARRAY_BUFFER, 0, sizeof(Render::Pixel) * 256, gl::MAP_WRITE_BIT | gl::MAP_INVALIDATE_BUFFER_BIT | gl::MAP_UNSYNCHRONIZED_BIT);
		ScanlineComplete = false;
	}

	static void EndScanline()
	{
		gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffer);
		gl::UnmapBuffer(gl::ARRAY_BUFFER);
		gl::Uniform1i(scanlineIndexLocation, CurrentScanline);
		gl::DrawArraysInstanced(gl::TRIANGLE_STRIP, 0, 4, 256);
		ScanlineComplete = true;

		if (CurrentScanline == 0)
		{
			glfwSwapBuffers(window);
			gl::Clear(gl::DEPTH_BUFFER_BIT);
		}
		CurrentScanline++;
		CurrentScanline %= 256;
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
	
	static unsigned CurrentScanline;
	static unsigned CurrentFrame;

	static bool FrameComplete;
	static bool ScanlineComplete;
};


#endif