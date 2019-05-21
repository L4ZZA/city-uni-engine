	-- project "Bullet3Dynamics"

	-- language "C++"
				
	-- kind "StaticLib"

	-- includedirs {
	-- 	".."
	-- }		
	
    -- if os.is("Linux") then
    --     buildoptions{"-fPIC"}
    -- end

	-- files {
	-- 	"**.cpp",
	-- 	"**.h"
	-- }

-- engine core project
project "Bullet3Dynamics"
-- location makes sure that everything below will be relative to the project directory
-- location "Bullet3Collision"
kind "StaticLib" -- Static library (.lib)
language "C++"
-- staticruntime "on"

-- targetdir ("bin/" .. outputdir .. "/%{prj.name}")
-- objdir ("inter/" .. outputdir .. "/%{prj.name}")

files
{
	-- ** means recursively search down that folder
	"**.cpp",
	"**.h"
}

includedirs {".."}

if os.get() == "linux" then
	buildoptions{"-fPIC"}
end
