-- workspace is the solution
workspace "pyro"
    architecture "x64"
    startproject "sandbox"

    configurations
    {
        "Debug",
        "Release", -- more like a faster debug
        "Dist" -- distribution build
    }

-- example: debug-win-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (sln dir)
IncludeDir = {}
IncludeDir["spdlog"] = "pyro/external/spdlog/include"
IncludeDir["GLFW"] = "pyro/external/GLFW/include/"
IncludeDir["Glad"] = "pyro/external/Glad/include/"
IncludeDir["ImGui"] = "pyro/external/imgui/"
IncludeDir["glm"] = "pyro/external/glm/"
IncludeDir["stb_image"] = "pyro/external/stb_image/"
IncludeDir["assimp"] = "pyro/external/assimp/include/"
IncludeDir["assimpcfg"] = "pyro/external/assimp/config/"

group "dependencies"
    -- iclude other premake files
    include "pyro/external/GLFW/"
    include "pyro/external/Glad/"
    include "pyro/external/ImGui/"
    include "pyro/external/assimp/"
group""

-- === Core Project: pyro =======================================================
project "pyro"
    -- location makes sure that everything below will be relative to the project directory
    location "pyro"
    kind "StaticLib" -- Static library (.lib)
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("inter/" .. outputdir .. "/%{prj.name}")
    
    pchheader "pyro_pch.h"
    pchsource "pyro/src/pyro_pch.cpp"

    files
    {
        -- ** means recursively search down that folder
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.shader",
        "%{prj.name}/res/**.shader",
        "%{prj.name}/external/glm/glm/**.hpp", 
        "%{prj.name}/external/glm/glm/**.inl",
        "%{prj.name}/external/stb_image/stb_image.h",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.assimpcfg}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "assimp",
    }

    -- filters are used to apply property to some specific configurations only
    filter "system:windows"
        systemversion "latest" -- windows SDK version
        linkoptions { "/ignore:4221" }

        defines
        {
            "PYRO_PLATFORM_WIN",
            -- "PYRO_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

        links { "opengl32.lib" }

    filter "configurations:Debug"
        defines "PYRO_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PYRO_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PYRO_DIST"
        runtime "Release"
        optimize "on"

-- === Core Project: sanbox =======================================================
project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
     objdir ("inter/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.shader",
        "%{prj.name}/res/**.shader",
        "%{prj.name}/src/**.glsl",
        "%{prj.name}/res/**.glsl",
    }

    includedirs
    {
        "pyro/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "pyro",
    }

    filter "system:windows"
        systemversion "latest"
        linkoptions { "/ignore:4221" }

        defines
        {
            "PYRO_PLATFORM_WIN",
        }

    filter "configurations:Debug"
        defines "PYRO_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PYRO_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PYRO_DIST"
        runtime "Release"
        optimize "on"