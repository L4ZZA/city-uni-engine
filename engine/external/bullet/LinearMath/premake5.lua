project "LinearMath"
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
		"*.cpp",
		"*.h",
		"TaskScheduler/*.cpp",
		"TaskScheduler/*.h"
	}
