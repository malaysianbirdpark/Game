workspace "Game"  
    architecture "x64"
    configurations { "Debug", "Release" } 
    startproject "Game"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

project "Core"  
    location "Core"
    kind "StaticLib"   
    staticruntime "Off"
    language "C++"   
    cppdialect "C++20" 
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Core/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/**.hlsl",
    }
    includedirs {
        "%{prj.name}/src/",
        "%{prj.name}/vendor/spdlog/include",
    }

    links {
        "d3d11",
        "dxgi",
        "dxguid",
        "D3DCompiler",
    }

    nuget { "Microsoft.Direct3D.D3D12:1.610.3" }

    optimize "Speed"

    floatingpoint "Fast"

    characterset "MBCS"

    filter "files:**.hlsl"
        shaderentry "main"
        shadermodel "5.0"
        shaderobjectfileoutput "%{wks.location}/Game/ShaderLib/%{file.basename}.cso"

    filter "files:**PS.hlsl"
        shadertype "Pixel"

    filter "files:**VS.hlsl"
        shadertype "Vertex"

    filter "system:windows"
        systemversion "latest"
        defines {
            "PLATFORM_WINDOWS",
            --"BUILD_DLL",
        }    
        
    filter "system:linux"
        systemversion "latest"
        defines {
            "PLATFORM_LINUX",
            --"BUILD_DLL",
        }

    filter "configurations:Debug"
        defines { "DEBUG=1" }  
        runtime "Debug"
        symbols "on" 

    filter "configurations:Release"  
        defines { "DEBUG=0", "RELEASE" }    
        runtime "Release"
        optimize "on" 

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"   
    cppdialect "C++20" 
    staticruntime "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs {
        "Core/src/",
    }

    links {
        "Core"
    }

    optimize "Speed"

    floatingpoint "Fast"

    characterset "MBCS"

    filter "system:windows"
        systemversion "latest"
        defines {
            "PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines { "DEBUG=1" }  
        runtime "Debug"
        symbols "on" 

    filter "configurations:Release"  
        defines { "DEBUG=0", "RELEASE" }    
        runtime "Release"
        optimize "on" 