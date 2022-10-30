@echo off

call "%~dp0\..\config.bat"

rem build sources
call "%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-platform="%Platform%" ^
-clientconfig="%Configuration%" ^
-build -cook

rem run tests
set TestRunner="%EditorPath%" "%ProjectPath%" -ExecCmds="Automation RunTests %TestNames%;Quit" ^
-log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath="%ReportOutputPath%"

rem run code coverage
::set ExportType=cobertura:%ReportOutputPath%\Coverage\CodeCoverageReport.xml
set ExportType=html:%ReportOutputPath%\Coverage\CodeCoverageReport

"%OpenCPPCoveragePath%" --modules="%ProjectRoot%" --sources="%SourceCodePath%" ^
--excluded_sources="%SourceCodePath%\TPS\Tests" --export_type="%ExportType%" -- %TestRunner% -v

rem clean obsolete artifacts
del /q LastCoverageResults.log

rem copy test artifacts
set TestsDir=%~dp0
set TestsDataDir=%~dp0data
robocopy "%TestsDataDir%" "%ReportOutputPath%" /E

rem start local server and show report
set Port=8081
set Localhost=http://localhost:%Port%

pushd "%ReportOutputPath%"
start "" "%Localhost%"
start "" "%Localhost%\Coverage\CodeCoverageReport\index.html"
call http-server -p="%Port%"
popd