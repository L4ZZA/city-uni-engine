-- Implement the workspace files command for solution-scope files
require('vstudio')
premake.api.register {
	name = "workspace_files",
	scope = "workspace",
	kind = "list:string",
}

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
	if wks.workspace_files and #wks.workspace_files > 0 then
		premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "configs", "configs", "{' .. os.uuid("configs:"..wks.name) .. '}"')
		premake.push("ProjectSection(SolutionItems) = preProject")
		for _, path in ipairs(wks.workspace_files) do
			premake.w(path.." = "..path)
		end
		premake.pop("EndProjectSection")
		premake.pop("EndProject")
	end
	base(wks)
end)

-- workspace is the solution
workspace "engine"
    architecture "x64"
    startproject "game"

    configurations
    {
        "Debug",
        "Release", -- more like a faster debug
        "Dist" -- distribution build
    }

	workspace_files 
	{
		".editorconfig"
    }
    
-- example: debug-win-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (sln dir)
IncludeDir = {}
IncludeDir["spdlog"]    = "engine/external/spdlog/include"
IncludeDir["GLFW"]      = "engine/external/GLFW/include"
IncludeDir["Glad"]      = "engine/external/Glad/include"
IncludeDir["glm"]       = "engine/external/glm"
IncludeDir["stb_image"] = "engine/external/stb_image"
IncludeDir["ImGui"]     = "engine/external/imgui"
IncludeDir["assimp"]    = "engine/external/assimp/include/"
IncludeDir["assimpcfg"] = "engine/external/assimp/config/"
IncludeDir["bullet"]    = "engine/external/bullet/"


group "dependencies"
    -- iclude other premake files
    include "engine/external/GLFW/"
    include "engine/external/Glad/"
	include "engine/external/stb_image"
    include "engine/external/assimp/"
	include "engine/external/bullet/BulletCollision/"
	include "engine/external/bullet/BulletDynamics/"
	include "engine/external/bullet/LinearMath/"
group""

-- === Core Project: engine =======================================================
project "engine"
    -- location makes sure that everything below will be relative to the project directory
    location "engine"
    kind "StaticLib" -- Static library (.lib)
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("inter/" .. outputdir .. "/%{prj.name}")
    
    pchheader "pch.h"
    pchsource "engine/src/pch.cpp"

    files
    {
        -- ** means recursively search down that folder
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/external/glm/glm/**.hpp", 
        "%{prj.name}/external/glm/glm/**.inl",
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
		"%{IncludeDir.stb_image}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.assimpcfg}",
		"%{IncludeDir.bullet}",
    }

    links
    {
        "GLFW",
        "Glad",
		"opengl32.lib",
        "stb_image",
        "assimp",
		"BulletCollision",
		"BulletDynamics",
		"LinearMath",
    }

    -- filters are used to apply property to some specific configurations only
    filter "system:windows"
        systemversion "latest" -- windows SDK version

        defines
        {
            "ENGINE_PLATFORM_WIN",
            "ENGINE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "on"

-- === Core Project: sanbox =======================================================
project "game"
    location "game"
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
    }

    includedirs
    {
        "engine/src",
        "engine/external",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.bullet}",
    }

    links
    {
        "engine",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ENGINE_PLATFORM_WIN",
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "on"
