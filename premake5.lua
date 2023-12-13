 
-- premake5.lua
workspace "game"
	configurations { "Debug", "Release" }

project "game"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	 
	files { "src/*.h", "src/*.cpp" }

	filter {"configurations:Debug","system:windows"}
	     defines { "DEBUG", "_WIN32"}
	     symbols "On"
	     warnings "Extra"  -- Show extra warnings in  mode
		--link locally installed SDL2 libraries
		includedir{"WinDependencies/include"}
		libdirs{"WinDependencies/lib"}
		links {
            "SDL2.dll",
            "SDL2_ttf.dll",
            "SDL2_image.dll",
            "SDL2_mixer.dll",
        }

	filter {"configurations:Debug","system:linux"}
		defines { "DEBUG","_UNIX"}
		symbols "On"
		warnings "Extra"  -- Show extra warnings in  mode
		links { "SDL2", "SDL2_ttf", "SDL2_image" , "SDL2_mixer"}



	filter { "configurations:Release", "system:windows" }
		defines { "NDEBUG", "_WIN32" }
		optimize "On"
		--link locally installed SDL2 libraries
		includedir{"WinDependencies/include"}
		libdirs{"WinDependencies/lib"}
		links {
            "SDL2.dll",
            "SDL2_ttf.dll",
            "SDL2_image.dll",
            "SDL2_mixer.dll",
        }

	filter { "configurations:Release", "system:linux" }
		defines { "NDEBUG", "_UNIX" }
		optimize "On"
		flags { "LinkTimeOptimization" } -- Enable link-time optimization
		links { "SDL2", "SDL2_ttf", "SDL2_image" , "SDL2_mixer"}

