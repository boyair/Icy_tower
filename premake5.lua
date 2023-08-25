 
-- premake5.lua
workspace "game"
   configurations { "Debug", "Release" }

project "game"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "src/*.h", "src/*.cpp" }
    links { "SDL2", "SDL2_ttf", "SDL2_image" , "SDL2_mixer"}  -- Link against SDL2 library

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      warnings "Extra"  -- Show extra warnings in  mode


   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
