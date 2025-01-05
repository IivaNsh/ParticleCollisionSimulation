-- premake5.lua
workspace "Experement"
   configurations { "Debug", "Release" }
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   
project "particels_box"
   kind "ConsoleApp"
  
  files { "src/**", "imgui/*.cpp", "imgui/*.h", "imgui/backends/imgui_impl_glfw*", "imgui/backends/imgui_impl_opengl3*", "lib/glad/src/**", "Vector2/**" }
  

  includedirs "lib/GLFW/Include"
  includedirs "lib/linmath"
  includedirs "lib/glad/include"
  includedirs "Vector2"
  includedirs "imgui"
  includedirs "imgui/backends"
  
  links "glfw"
  links "GL"

