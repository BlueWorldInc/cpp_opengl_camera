@echo off
echo Compiling...
g++ -L lib -I include .\main_opengl_learn_com.cpp .\glad.c .\lib\bmpread.c -o app -lglew32 -lopengl32 -lglu32 -lglfw3 -lgdi32 && powershell write-host -fore Green Compile success && .\app.exe