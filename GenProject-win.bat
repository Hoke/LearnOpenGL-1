pushd %~dp0
if not exist win (
    mkdir win
) else (
    del /S /Q /F win
    rd /S /Q win
    mkdir win
)
 
pushd win
@echo off
cmake ..  
popd
popd
@echo on