	project "Bullet2FileLoader"
		
	kind "StaticLib"
	
	includedirs {
		"../../../src"
	}
	
    if os.get() == "linux" then
        buildoptions{"-fPIC"}
    end
	 
	files {
		"**.cpp",
		"**.h"
	}