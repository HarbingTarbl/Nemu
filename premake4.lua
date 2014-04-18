solution "Nemu"
	location "build"

	configurations {"Debug", "Release"}
	
	project "Nemu"
		kind "ConsoleApp"
		language "c++"
		files {"src/**.cpp", "src/**.hpp", "src/**.c", "src/**.h"}
		includedirs {"includes"}
		libdirs {"libs"}
		
		buildoptions "-std=c++11"
		
		objdir "bin/obj"
		targetdir "bin"

		configuration "windows"
			defines "WIN32"
			debugdir "working"
			links { "glfw3", "opengl32", "glu32" }

		configuration "linux"
			defines "LINUX"
			links { "X11", "glfw3", "opengl32", "glu32" }
			
		configuration "Debug"
			targetsuffix "D"
			defines "_DEBUG"
			flags "Symbols"

		configuration "Release"
			defines "NDEBUG"
			flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"}