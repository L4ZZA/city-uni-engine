project "BulletInverseDynamics"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"
    staticruntime "On"

    if os.is("Linux") then
        buildoptions{"-fPIC"}
    end
    includedirs {
        "..",
    }
    files {
        "IDMath.cpp",
        "MultiBodyTree.cpp",
        "details/MultiBodyTreeInitCache.cpp",
        "details/MultiBodyTreeImpl.cpp",
    }
