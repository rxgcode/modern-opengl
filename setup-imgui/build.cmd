@echo off
echo compilando ...

set CPP_IMGUI=libs\imgui\imgui.cpp libs\imgui\imgui_demo.cpp libs\imgui\imgui_draw.cpp libs\imgui\imgui_impl_glfw.cpp libs\imgui\imgui_impl_opengl3.cpp libs\imgui\imgui_tables.cpp libs\imgui\imgui_widgets.cpp

set CMD=g++ -D GLEW_STATIC main.cpp %CPP_IMGUI% -o main -I libs\imgui\include -I libs\glew\include -I libs\glfw\include -I libs\glm -L libs\glew\lib\Win32 -L libs\glfw\lib-mingw -l glew32s -l glfw3dll -l opengl32 -l gdi32 -l imm32

echo ------Command
echo %CMD%
%CMD%
pause