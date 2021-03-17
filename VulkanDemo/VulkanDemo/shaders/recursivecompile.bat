@echo off
for /r %%i in (*.frag, *.vert) do C:\VulkanSDK\1.2.154.1\Bin32\glslc.exe %%i -o %%~ni.spv