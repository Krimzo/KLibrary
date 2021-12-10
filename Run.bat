cls &&^
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" &&^
cl -I"./incl" "./src/TriangleInterpolation.cpp" -Fo"./out/testing.obj" -Fe"./out/testing.exe" &&^
cls &&^
"./out/testing.exe"
