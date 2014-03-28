#pragma once

#define GLFW_INCLUDE_NONE
#include "gl_core_3_3.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>


class Render
{
private:
	static GLFWwindow* window;
	
	static unsigned programId;
	
	static unsigned colorburstBuffer;
	static unsigned bufferIndex;

	static unsigned scanlineIndex;
	static unsigned scanlineIndexLocation;

	static unsigned colorTexture;

	static unsigned defaultVAO;


public:
	static void Initalize()
	{
		using namespace std;
		if (!window)
		{
			glfwInit();
			window = glfwCreateWindow(800, 600, "NES Emulator", nullptr, nullptr);
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

			gl::CompileShader(fragment);
			gl::CompileShader(vertex);

			gl::AttachShader(programId, fragment);
			gl::AttachShader(programId, vertex);
			
			gl::LinkProgram(programId);
		
			gl::UseProgram(programId);

			gl::GenBuffers(1, &colorburstBuffer);
			gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffer);

			gl::GenVertexArrays(1, &defaultVAO);
			gl::BindVertexArray(defaultVAO);

			gl::EnableVertexAttribArray(0);
			gl::VertexAttribPointer(0, 2, gl::SHORT, false, 0, 0);

		}
	}

	static void Terminate()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	static void StartFrame()
	{
		glfwPollEvents();
	}

	static void EndFrame()
	{
		glfwSwapBuffers(window);
	}

	static void BeginScanline()
	{
		gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffer);
		PixelOut = (uint8_t*)gl::MapBuffer(gl::ARRAY_BUFFER, colorburstBuffer);
	}

	static void EndScanline()
	{
		gl::BindBuffer(gl::ARRAY_BUFFER, colorburstBuffer);
		gl::UnmapBuffer(gl::ARRAY_BUFFER);
		gl::Uniform1ui(scanlineIndexLocation, scanlineIndex);
	}

	static uint8_t* PixelOut;
	
	static unsigned CurrentScanline;
	static unsigned CurrentFrame;

	static bool FrameComplete;
	static bool ScanlineComplete;
};

