@echo off

rem Engine params
set EnginePath_4.26=C:\Epic Games\UE Binary\UE_4.26
set EnginePath_5.0=C:\Epic Games\UE Binary\UE_5.0EA
set EnginePath_4.26_InstalledBuild=C:\Epic Games\UE Source\UE_4.26_InstalledBuild

set UBTRelativePath4=Engine\Binaries\DotNET\UnrealBuildTool.exe
set UBTRelativePath5=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=c:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem !! Engine version for packaging !!
set RunUATPath=%EnginePath_4.26_InstalledBuild%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=C:\_Projects\_Jenkins\TPS
set ProjectName=TPS.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set filesToRemove=*.sln
