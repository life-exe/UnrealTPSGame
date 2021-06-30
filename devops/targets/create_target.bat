@echo off

del /q %TargetFilePath%

for /f "tokens=*" %%a in (%TargetTemplateFilePath%) do (
    call echo %%a>>%TargetFilePath%
)

call "%~dp0\..\misc\format_all_files.bat"
