@echo off
for %%i in (*.vert *.frag *.comp) do echo Compiling: %%~i && "glslc.exe" "%%~i" -o "%%~nxi.spv"