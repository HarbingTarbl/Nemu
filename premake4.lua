solution "Nemu"
	location "build"
	debugdir "working"
	buildoptions "-std=c++11"
	configurations {"Debug", "Release"}
	
	project "Nemu"
		kind "ConsoleApp"
		language "c++"
		files {"src/**.cpp", "src/**.hpp", "src/**.c", "src/**.h"}
		includedirs {"includes/**.hpp", "includes/**.h" }
		libdirs {"libs"}
		
		objdir "bin/obj"
		targetdir "bin"

		configuration "windows"
			defines "WIN32"
			links { "glfw3" }

		configuration "linux"
			defines "LINUX"
			
		configuration "Debug"
			targetsuffix "D"
			defines "_DEBUG"
			flags "Symbols"

		configuration "Release"
			defines "NDEBUG"
			flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"}