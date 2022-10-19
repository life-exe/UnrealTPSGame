# UnrealTPSGame

**TPS** is a simple **C++** project from **UE** templates. We are using it for training **CI/CD** pipelines with **Jenkins** and **bat scripts**

## Package game

```c++
[UE4Path]/Engine/Build/BatchFiles/RunUAT.bat BuildCookRun
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

## Cook game content

```c++
[UE4Path]/Engine/Build/BatchFiles/RunUAT.bat BuildCookRun
-project=[projectPath].uproject
-platform=[Platform]
-clientconfig=[Configuration]
-[buildArgs]
```

Example:

```c++
"c:/Epic Games/UE Binary/UE_4.26/Engine/Build/BatchFiles/RunUAT.bat" BuildCookRun
-project=c:/_Projects/_Jenkins/TPS/TPS.uproject
-platform=Win64 
-clientconfig=Development
-build -cook -skippackage
```

## Make Installed Build

```c++
[UE4Path]/Engine/Build/BatchFiles/RunUAT.bat BuildGraph 
-target="Make Installed Build [Platform]" 
-script=[UE4Path]/Engine/Build/InstalledEngineBuild.xml
-clean 
-set:HostPlatformOnly=true 
-set:VS2019=true 
-set:WithClient=true 
-set:WithServer=true 
-set:WithDDC=false 
-set:BuildDir=[BuildDir]
```

Example:

```c++
"c:/Epic Games/UE Binary/UE_4.26/Engine/Build/BatchFiles/RunUAT.bat" BuildGraph 
-target="Make Installed Build Win64" 
-script="c:/Epic Games/UE Binary/UE_4.26/Engine/Build/InstalledEngineBuild.xml"
-clean 
-set:HostPlatformOnly=true 
-set:VS2019=true 
-set:WithClient=true 
-set:WithServer=true 
-set:WithDDC=false 
-set:BuildDir="c:/Epic Games/UE Binary/UE_4.26_InstalledBuild"
```

You should modify `InstalledEngineBuild.xml` for using **BuildDir** param:

```c++
<!-- Output dir for build -->
<Option Name="BuildDir" DefaultValue="$(RootDir)/LocalBuilds/Engine" Description="Output dir for build"/>

<!-- The local output directory -->
<Property Name="LocalInstalledDir" Value="$(BuildDir)"/>
<Property Name="LocalInstalledDirMac" Value="$(BuildDir)/Mac"/>
<Property Name="LocalInstalledDirLinux" Value="$(BuildDir)/Linux"/>
```

test pull request
