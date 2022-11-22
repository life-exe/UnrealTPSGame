@echo off
call "%~dp0devops\config.bat"

devops\misc\generate_project_files.bat "%EnginePath_5.0%" "%UBTRelativePath5%" "%VersionSelector%" "%ProjectName%"
