#include "File-Rename.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

/**
 * The main method of the File-Rename class.
 *
 * The method initializes the main SDL window and
 * waits for SDL events such as keyboard input or
 * if the buttons on the window are clicked.
 *
 * The method will also render all of the ImGui windows
 * into the main window.
 *
 * The method will continue this until the main window is
 * closed.
 *
 * @see initWindow()
 * @see closeWindow()
 * @see drawDisplay()
 */
int main(){
    FileRename test;
    
    if(!test.initWindow()){
        cout << "Failed to initialize display." << endl;
    }else{
        bool quit = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        
        while(!quit){
            ImGuiIO& io = ImGui::GetIO();
            int wheel = 0;
            SDL_Event e;
            while( SDL_PollEvent( &e ) != 0 ){
                io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
                
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
                    
                    else if(e.type == SDL_TEXTINPUT){
                        io.AddInputCharactersUTF8(e.text.text);
                    }
                    
                    else if(e.type == SDL_KEYDOWN && e.key.repeat == 0  ){
                        if(e.key.keysym.sym == SDLK_RCTRL) io.KeyCtrl = 1;
                        if(e.key.keysym.sym == SDLK_LCTRL) io.KeyCtrl = 1;
                        if(e.key.keysym.sym == SDLK_BACKSPACE) io.KeysDown[SDLK_BACKSPACE] = 1;
                    }
                    
                    else if(e.type == SDL_KEYUP && e.key.repeat == 0  ){
                        if(e.key.keysym.sym == SDLK_RCTRL) io.KeyCtrl = 0;
                        if(e.key.keysym.sym == SDLK_LCTRL) io.KeyCtrl = 0;
                        if(e.key.keysym.sym == SDLK_BACKSPACE) io.KeysDown[SDLK_BACKSPACE] = 0;
                    }
            }
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(test.window);

            int mouseX, mouseY;
            const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    
            // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
            io.DeltaTime = 1.0f / 60.0f;
            io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
            io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
            io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
            io.MouseWheel = static_cast<float>(wheel);
            
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            
            
            test.drawDisplay();
        }
        test.closeWindow();
    }
    return 0; 
}