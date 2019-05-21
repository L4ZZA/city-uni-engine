project "BulletInverseDynamics"
	kind "StaticLib"

    if os.get() == "linux" then
        buildoptions{"-fPIC"}
    end

	includedirs 
	{
		"..",
	}

	files 
	{
		"IDMath.cpp",
		"MultiBodyTree.cpp",
		"details/MultiBodyTreeInitCache.cpp",
		"details/MultiBodyTreeImpl.cpp",
	}
