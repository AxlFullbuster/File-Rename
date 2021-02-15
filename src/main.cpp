#include "File-Rename.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;


int main(){
    FileRename test;
    //test.createFiles(8);
    
    if(!test.initWindow()){
        cout << "Failed to initialize display." << endl;
    }else{
        bool quit = false;
        
        while(!quit){
            ImGuiIO& io = ImGui::GetIO();
            int wheel = 0;
            SDL_Event e;
            while( SDL_PollEvent( &e ) != 0 ){
                if (e.type == SDL_QUIT) quit = true;
                	else if (e.type == SDL_WINDOWEVENT){
                        if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                            io.DisplaySize.x = static_cast<float>(e.window.data1);
                            io.DisplaySize.y = static_cast<float>(e.window.data2);
                        }
                    }
                    else if (e.type == SDL_MOUSEWHEEL){
                        wheel = e.wheel.y;
                    }
                    
                    else if(e.type == SDL_KEYDOWN && e.key.repeat == 0  ){
                        if(e.key.keysym.sym == SDLK_RCTRL) io.KeyCtrl = 1;
                        if(e.key.keysym.sym == SDLK_LCTRL) io.KeyCtrl = 1;
                    }
                    
                    else if(e.type == SDL_KEYUP && e.key.repeat == 0  ){
                        if(e.key.keysym.sym == SDLK_RCTRL) io.KeyCtrl = 0;
                        if(e.key.keysym.sym == SDLK_LCTRL) io.KeyCtrl = 0;
                    }
            }

            int mouseX, mouseY;
            const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    
            // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
            
            io.DeltaTime = 1.0f / 60.0f;
            io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
            io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
            io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
            io.MouseWheel = static_cast<float>(wheel);
            
            
            
            test.drawDisplay();
        }

        test.closeWindow();
    }
    
    return 0; 
}