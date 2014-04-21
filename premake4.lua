solution "Nemu"
	configurations {"Debug", "Release"}
	location ("build/" .. apr.platform_get())
	
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
			links { "glfw3", "gdi32", "glu32" }

		configuration "linux"
			defines "LINUX"
			links { "GL", "GLU", "glfw3", "X11", "Xxf86vm", "Xrandr", "pthread", "Xi", "m" }
			
		configuration "macosx"
			defines "OSX"
			links { "glfw3" }
			linkoptions { "-framework Carbon -framework OpenGL" }
			
		configuration "Debug"
			targetsuffix "D"
			defines "_DEBUG"
			flags "Symbols"

		configuration "Release"
			defines "NDEBUG"
			flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"}
