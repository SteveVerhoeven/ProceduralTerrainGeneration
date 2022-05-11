IncludeDir = {}
IncludeDir["VLD"] = "%{wks.location}/Libs/Visual Leak Detector/include"
IncludeDir["DXTEX"] = "%{wks.location}/Libs/DX_Tex/include"
IncludeDir["DXERROR"] = "%{wks.location}/Libs/DX_Error/include"
IncludeDir["DXEFFECTS"] = "%{wks.location}/Libs/DX_Effects11/include"
IncludeDir["IMGUI"] = "%{wks.location}/Libs/IMGUI"
IncludeDir["yaml_cpp"] = "%{wks.location}/Libs/yaml_cpp/include"

LibraryDir = {}
LibraryDir["VLD32"] = "%{wks.location}/Libs/Visual Leak Detector/lib/Win32"
LibraryDir["VLD64"] = "%{wks.location}/Libs/Visual Leak Detector/lib/Win64"
LibraryDir["DXTEX"] = "%{wks.location}/Libs/DX_Tex/lib"
LibraryDir["DXERROR"] = "%{wks.location}/Libs/DX_Error/lib"
LibraryDir["DXEFFECTS"] = "%{wks.location}/Libs/DX_Effects11/lib"

Library = {}
Library["VLD"] = "%{LibraryDir.VLD32}/vld.lib"
Library["DXTEX_DEBUG"] = "%{LibraryDir.DXTEX}/DxTex_vc14_Debug.lib"
Library["DXTEX_RELEASE"] = "%{LibraryDir.DXTEX}/DxTex_vc14_Release.lib"
Library["DXERROR"] = "%{LibraryDir.DXERROR}/DX_Error.lib"
Library["DXEFFECTS_DEBUG"] = "%{LibraryDir.DXEFFECTS}/DxEffects11_vc14_Debug.lib"
Library["DXEFFECTS_RELEASE"] = "%{LibraryDir.DXEFFECTS}/DxEffects11_vc14_Release.lib"