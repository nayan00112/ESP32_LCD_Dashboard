@echo off
:prompt
set /p command="CustomPrompt>"
if exist error.txt (
    del error.txt
)
%command% 2>> error.txt
if errorlevel 1 (
    type error.txt
    python python_file.py %ERRORLEVEL%
)
goto prompt
