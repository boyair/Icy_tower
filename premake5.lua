 
-- premake5.lua
workspace "game"
	configurations { "Debug", "Release" }
--	 system{"windows","linux"}
project "game"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	 
	files { "src/*.h", "src/*.cpp" }
	 links { "SDL2", "SDL2_ttf", "SDL2_image" , "SDL2_mixer"}  -- Link against SDL2 library

	filter {"configurations:Debug","system:windows"}
	     defines { "DEBUG", "_WIN32"}
	     symbols "On"
	     warnings "Extra"  -- Show extra warnings in  mode

	filter {"configurations:Debug","system:linux"}
	     defines { "DEBUG","_UNIX"}
	     symbols "On"
	     warnings "Extra"  -- Show extra warnings in  mode



	filter { "configurations:Release", "system:windows" }
		defines { "NDEBUG", "_WIN32" }
		optimize "On"

	filter { "configurations:Release", "system:linux" }
		defines { "NDEBUG", "_UNIX" }
		optimize "On"
		flags { "LinkTimeOptimization" } -- Enable link-time optimization
