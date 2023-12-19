 
-- premake5.lua
workspace "game"
	configurations { "Debug", "Release" }
    architecture "x86_64"
    cppdialect "C++latest"

project "game"
	kind "ConsoleApp"
	language "C++"
    cppdialect "c++20"
	targetdir "bin/%{cfg.buildcfg}"
	 
	files { "src/*.h", "src/*.cpp" }

	filter {"configurations:Debug","system:windows"}
	     defines { "DEBUG", "_WIN32"}
	     symbols "On"
	     warnings "Extra"  -- Show extra warnings in  mode
		--link locally installed SDL2 libraries
		includedirs{"WinDependencies/include"}
		libdirs{"WinDependencies/lib"}
		links {
            "SDL2.lib",
            "SDL2_ttf.lib",
            "SDL2_image.lib",
            "SDL2_mixer.lib",
            "SDL2main.lib",
        }



	filter { "configurations:Release", "system:windows" }
		defines { "NDEBUG", "_WIN32" }
		optimize "On"
		--link locally installed SDL2 libraries
		includedirs{"WinDependencies/include"}
		libdirs{"WinDependencies/lib"}
		links {
            "SDL2.lib",
            "SDL2_ttf.lib",
            "SDL2_image.lib",
            "SDL2_mixer.lib",
            "SDL2main.lib",
        }


	filter {"configurations:Debug","system:linux"}
		defines { "DEBUG","_UNIX"}
		symbols "On"
		warnings "Extra"  -- Show extra warnings in  mode
		links { "SDL2", "SDL2_ttf", "SDL2_image" , "SDL2_mixer"}


	filter { "configurations:Release", "system:linux" }
		defines { "NDEBUG", "_UNIX" }
		optimize "On"
		flags { "LinkTimeOptimization" } -- Enable link-time optimization
		links { "SDL2", "SDL2_ttf", "SDL2_image" , "SDL2_mixer"}

