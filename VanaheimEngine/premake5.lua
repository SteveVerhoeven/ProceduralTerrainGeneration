-- Engine
project "VanaheimEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "VanaheimPCH.h"
	pchsource "VanaheimPCH.cpp"
	
	files
	{
		"**.h",
		"**.cpp"
	}
	includedirs
	{}
	libdirs
	{}
	links
	{}
	defines
	{
		"WIN32",
		"_CONSOLE"
	}
	characterset ("MBCS")
	warnings "Extra"
	flags 
	{
		"FatalCompileWarnings", "NoPCH"
	}
	
	filter "files:Libs/IMGUI/**.cpp"
	flags { "NoPCH" }
	
	filter "system:windows"
		system "windows"
		systemversion "latest"
		defines
		{
			"WIN32",
			"_CONSOLE"
		}		

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"		
		includedirs
		{	
			-- Mutual
			"%{IncludeDir.IMGUI}",
			"%{IncludeDir.yaml_cpp}",
			"%{IncludeDir.DXTEX}",
			"%{IncludeDir.DXEFFECTS}",
			-- Specific
			"%{IncludeDir.DXERROR}",
			"%{IncludeDir.VLD}"
		}
		libdirs
		{			
			-- Mutual
			"%{LibraryDir.DXTEX}",
			"%{LibraryDir.DXEFFECTS}",
			-- Specific
			"%{LibraryDir.VLD32}",
			"%{LibraryDir.DXERROR}"
		}
		links
		{
			-- Mutual
			"IMGUI",
			"yaml-cpp",
			"DxTex_vc14_Debug.lib",
			"DxEffects11_vc14_Debug.lib",
			-- Specific
			"vld.lib",
			"DX_Error.lib"
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
			"%{IncludeDir.IMGUI}",
			"%{IncludeDir.yaml_cpp}",
			"%{IncludeDir.DXTEX}",
			"%{IncludeDir.DXEFFECTS}"
			-- Specific
			
		}
		libdirs
		{
			-- Mutual
			"%{LibraryDir.DXTEX}",
			"%{LibraryDir.DXEFFECTS}"
			-- Specific
			
		}
		links
		{
			-- Mutual
			"IMGUI",
			"yaml-cpp",
			"DxTex_vc14_Release.lib",
			"DxEffects11_vc14_Release.lib"			
			-- Specific
		}
		defines
		{
			"NDEBUG"
		}