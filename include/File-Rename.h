#ifndef FILERENAME_H
#define FILERENAME_H

#include<string>
#include<vector>
#include<array>
#include<map>
#include<fstream>
#include<SDL2/SDL.h>
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_internal.h"
#include "ImGuiFileDialog.h"
#include<boost/filesystem.hpp>

class FileRename{
    public:
        FileRename();
        ~FileRename();
        
        bool initWindow();
        void closeWindow();
        void drawDisplay();
        void createFiles(int n);
        
        
    private:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Texture* texture = NULL;
        
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 320;
        
        int sx = SCREEN_WIDTH / 160;
        int sy = SCREEN_HEIGHT / 144;
        
        int title_count = 0;
        
        bool dir = false;
        bool ready = false;
        bool input = false;
        
        std::string windowpath = "/mnt/c/Users/TDial/";
        std::string test_path = "/mnt/c/Users/TDial/Desktop/coding-work/C++/Linux/File-Rename/test";
        std::string filename = "";
        std::string filepath = "";
        std::string selectpath = "";
        std::string dir_path = "";
        std::vector<std::string> titles;
        std::vector<std::string> old_names;
        std::array<std::string, 100> new_names;
        
        std::string intToRoman(int);
        void selectionTool();
        void inputFile();
        void filePreview();
        void createDir();
        void renameFiles();
        void debug();
    
};

#endif