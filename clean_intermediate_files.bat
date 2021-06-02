@echo off

set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs
set filesToRemove=*.sln

for %%f in (%dirsToRemove%) do (
    rmdir /s /q %%f
)

for %%f in (%filesToRemove%) do (
    del /q %%f
)
