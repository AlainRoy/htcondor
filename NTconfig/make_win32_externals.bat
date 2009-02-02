@echo off
REM 
REM Top level script that executes the building of the externals for Win32.
REM

REM First, set some environment variables.
call set_vars.bat
if %ERRORLEVEL% NEQ 0 (echo . && echo *** Failed to prepare build environment && exit /b 1)

for %%e in ( %EXTERNALS_NEEDED% ) do ( if not exist %EXT_TRIGGERS%\%%e ( perl -w %EXTERN_DIR%\build_external --extern_dir=%EXTERN_DIR% --package_name=%%e --extern_config=%cd% || exit /b 1 ) else echo The %%e external up to date. Done. )

:finish

REM now just exit with the return code of the last command
exit /B %ERRORLEVEL%
