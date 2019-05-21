project "BulletSoftBody"
	kind "StaticLib"
	
	includedirs 
	{
		"..",
	}
	
	if os.get() == "linux" then
        buildoptions{"-fPIC"}
    end
	
	files 
	{
		"**.cpp",
		"**.h"
	}