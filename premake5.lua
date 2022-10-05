workspace "uranus"
    configurations { "Debug", "Release"}

project "uranus"
   kind "GameBoyApp"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"
   linkoptions { "-Wa-l", "-Wl-m", "-Wl-j" }

   files { "**.h", "**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"