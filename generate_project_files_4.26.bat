@echo off
call "%~dp0devops\config.bat"

devops\misc\generate_project_files.bat "%EnginePath_4.26%" "%UBTRelativePath4%" "%VersionSelector%" "%ProjectName%"
