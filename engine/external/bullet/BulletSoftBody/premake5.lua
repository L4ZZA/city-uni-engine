project "BulletSoftBody"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"
    staticruntime "On"

    includedirs {
        "..",
    }

    if os.is("Linux") then
        buildoptions{"-fPIC"}
    end

    files {
        "**.cpp",
        "**.h"
    }