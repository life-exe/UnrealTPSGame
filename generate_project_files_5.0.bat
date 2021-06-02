@echo off

set EnginePath=c:\Epic Games\UE Binary\UE_5.0EA
set UBTRelativePath=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=c:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe
set ProjectName=TPS.uproject

devops/generate_project_files.bat "%EnginePath%" "%UBTRelativePath%" "%VersionSelector%" "%ProjectName%"