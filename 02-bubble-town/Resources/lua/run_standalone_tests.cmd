@echo off

rem ====================================================
rem  Note these tests are run in its parent directory 
rem 	so that the package paths could be reused

cd..
lua lua\run_standalone_tests.lua

IF %ERRORLEVEL% NEQ 0 (
	cd lua
    EXIT /B %ERRORLEVEL%
) ELSE (
	cd lua
	echo Passed all tests!
)
