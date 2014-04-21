solution "Nemu"
	configurations {"Debug", "Release"}
	location ("build")
	
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
			postbuildcommands { "copy $(TargetPath) $(TargetDir)..\\working\\Nemu.exe" }
			defines "WIN32"
			links { "glfw3", "opengl32", "glu32" }

		configuration "linux"
			postbuildcommands { "cp $@ ../working/Nemu" }
			defines "LINUX"
			links { "GL", "GLU", "glfw3", "X11", "Xxf86vm", "Xrandr", "Xcursor", "pthread", "Xi", "m" }
			
		configuration "macosx"
			defines "OSX"
			links { "glfw3" }
			linkoptions { "-framework Cocoa -framework OpenGL -framework CoreVideo -framework AGL" }
			
		configuration "Debug"
			targetsuffix "D"
			defines "_DEBUG"
			flags "Symbols"

		configuration "Release"
			defines "NDEBUG"
			flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"}
			
	
