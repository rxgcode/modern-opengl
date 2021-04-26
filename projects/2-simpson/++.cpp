#include <iostream>
#include <string>

const std::string LIB_PATH = "\
-L libs\\glew\\lib\\Win32 \
-L libs\\glfw\\lib-mingw \
-L libs\\irrklang\\lib \
";

const std::string INCLUDE_PATH = "\
-I libs\\glew\\include \
-I libs\\glfw\\include \
-I libs\\glm \
-I libs\\imgui \
-I libs\\irrklang \
";

const std::string LIB = "\
-l glew32s \
-l glfw3dll \
-l opengl32 \
-l gdi32 \
-l irrKlang \
"; 

const std::string MCHN_OPT = "\
-mwindows \
-mconsole";

const std::string SRC =  "\
main.cpp \
curvas.cpp \
libs\\imgui\\imgui.cpp \
libs\\imgui\\imgui_demo.cpp \
libs\\imgui\\imgui_draw.cpp \
libs\\imgui\\imgui_impl_glfw.cpp \
libs\\imgui\\imgui_impl_opengl3.cpp \
libs\\imgui\\imgui_widgets.cpp \
";

const std::string MACROS = "\
-D GLEW_STATIC \
";

int main(int argc, char const *argv[])
{
    //  -o2 -Wall
    const std::string CMD = "g++ " + 
                            SRC + " -o main " +
                            INCLUDE_PATH + " " +
                            LIB_PATH + " " +  
                            LIB + " " + 
                            MCHN_OPT + " " + 
                            MACROS + " " ;
                            
    const std::string CMD_beau = " g++ \n [SRC] " + SRC +
                            "\n [-L] " + LIB_PATH + LIB + MCHN_OPT +
                            "\n [-I] " + INCLUDE_PATH +
                            "\n [-D] " + MACROS +
                            "\n [EXE] -o main ";

    std::cout<<" ---------------------------"<<std::endl;
    std::cout<<" [COMMAND] "<<CMD_beau<<std::endl;
    std::cout<<" ---------------------------"<<std::endl;

    system(CMD.c_str());

    return 0;
}