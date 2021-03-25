#ifndef FILERENAME_H
#define FILERENAME_H

#include<string>
#include<vector>
#include<array>
#include<map>
#include<fstream>
#include<SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiFileDialog.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include<boost/filesystem.hpp>

class FileRename{
    public:
        FileRename();
        ~FileRename();
        
        SDL_Window* window = NULL;
        SDL_GLContext gl_context = NULL;
        
        
        bool initWindow();
        void closeWindow();
        void drawDisplay();
        void createFiles(int n);
        
        
    private:
        
        int title_count = 0;
        int num = 0;
        
        bool dir = false;
        bool ready = false;
        bool input = false;
        bool ignore = false;
        
        std::string windowpath = "/home";
        std::string test_path = "../test";
        std::string filename = "";
        std::string filepath = "";
        std::string selectpath = "";
        std::string dir_path = "";
        std::vector<std::string> titles;
        std::vector<std::string> old_names;
        std::array<std::string, 100> new_names;
        std::vector<std::string> keywords;
        std::vector<std::string> file_ext;
        
        
        std::string intToRoman();
        void selectionTool();
        void inputFile();
        void filePreview();
        void editListValues();
        void createDir();
        void renameFiles();
        void debug();
    
};

#endif