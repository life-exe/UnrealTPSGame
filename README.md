# UnrealTPSGame

Simple C++ project from UE templates.

To build game outside the Unreal Editor you can use this pattern:

```c++
[UE4Path]/Engine/Build/BatchFiles/RunUAT.batBuildCookRun
-project=[projectPath].uproject
-platform=[Platform]
-clientconfig=[Configuration]
-archivedirectory=[archivePath]
-stagingdirectory=[stagingPath]
-[buildArgs]
```

Example:

```c++
"c:/Epic Games/UE Binary/UE_4.26/Engine/Build/BatchFiles/RunUAT.bat" BuildCookRun
-project=c:/_Projects/_Jenkins/TPS/TPS.uproject
-platform=Win64 
-clientconfig=Development
-archivedirectory=c:/_Projects/_Jenkins/TPS/Build
-stagingdirectory=c:/_Projects/_Jenkins/TPS/Stage
-build -cook -stage -package -archive -pak -allmaps
```
