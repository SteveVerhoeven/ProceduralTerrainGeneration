-- Game
project "Game"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "GamePCH.h"
	pchsource "GamePCH.cpp"

	files
	{
		"**.h",
		"**.cpp"
	}
	includedirs
	{
		"../VanaheimEngine"
	}
	links
	{
		"VanaheimEngine"
	}
	defines
	{
		"WIN32",
		"_CONSOLE"
	}
	characterset ("MBCS")
	warnings "Extra"
	flags "FatalCompileWarnings"


	filter "system:windows"
		system "windows"
		systemversion "latest"
			
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"		
		includedirs
		{	
			-- Mutual
			"%{IncludeDir.DXTEX}",	
			"%{IncludeDir.DXEFFECTS}",
			-- Specific
			"%{IncludeDir.VLD}"
		}
		libdirs
		{			
			-- Mutual
			"%{LibraryDir.DXTEX}",
			"%{LibraryDir.DXEFFECTS}",
			-- Specific
			"%{LibraryDir.VLD32}"
			
		}
		links
		{
			-- Mutual
			"DxTex_vc14_Debug.lib",
			"DxEffects11_vc14_Debug.lib",
			-- Specific
			"vld.lib"			
		}
		defines
		{
			"_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"		
		includedirs
		{
			-- Mutual
			"%{IncludeDir.DXTEX}",	
			"%{IncludeDir.DXEFFECTS}",
			-- Specific
			
		}
		libdirs
		{
			-- Mutual
			"%{LibraryDir.DXTEX}",
			"%{LibraryDir.DXEFFECTS}",
			-- Specific
			
		}
		links
		{
			-- Mutual
			"DxTex_vc14_Release.lib",
			"DxEffects11_vc14_Release.lib"
			-- Specific
			
		}
		defines
		{
			"NDEBUG"
		}