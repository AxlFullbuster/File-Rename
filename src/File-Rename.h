#ifndef FILERENAME_H
#define FILERENAME_H

#include<string>
#include<vector>
#include<array>
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

/**
 * The File-Rename class.
 *
 * The class uses both the boost filesystem library
 * and ImGui Library for interacting with files and displaying
 * windows respectively.
*/
class FileRename{
    public:
        FileRename(); /**< class constructor */
        ~FileRename(); /**< class de-constructor */
        
        SDL_Window* window = NULL; /**< An SDL variable that holds data for the window */
        SDL_GLContext gl_context = NULL; /**< An OpenGL context created by SDl for rendering */
        
        bool initWindow(); /**< member function that initalizes the main window of the program */

        void closeWindow(); /**< member function that closes the main window and
                             *clears data used for rendering */
        
        void drawDisplay(); /**< member function that renders ImGui windows to the main window */ 
        
    private:
        int title_count = 0; /**< int variable that stores the amount of words found in an input file */
        int num = 0; /**< int variable that is used as an offset for the numbers displayed in front of renamed files */
        
        bool dir = false; /**< boolean variable that checks if the user is creating directories*/
        bool file = false; /**< boolean variable that checks if the user is renaming files */
        bool input = false; /**< boolean variable that checks if an input file has been read */
        bool ignore = false; /**< boolean variable that checks if input files are being ingnored
                              *   Note: if this is set to true only directories can be created */
      
        std::string dir_path; /**< string variable that stores the directory path where
                               *created directories/renamed files will be sent to*/
        std::string filename; /**< string variable that stores the name of the input file being read */
        std::string selectpath; /**< string variable that stores the full path of the input file being read */
        
        std::vector<std::string> titles; /**< vector that stores words read from an input file */
        std::vector<std::string> old_names; /**< vector that stores the files that are being renamed */
        std::vector<std::string> new_names; /**< vector that stores the names that will replace the filename in "old_names" */
        std::vector<std::string> keywords; /**< vector that stores a list of keywords */
        std::vector<std::string> file_ext; /**< vector that stores a list of file extensions */
        
        void selectionTool(); /**< member function that displays an ImGui window for users
                               * to select a directory location, and rename or create directories */
        
        void inputFile(); /**< member function that displays an ImGui window for users
                           * to select an input file, ignore an input file, and preview the
                           * list of words found in an input file */
        
        void filePreview(); /**< member function that displays an ImGui window for users
                             * to choose keywords and file extensions. As well as preview the
                             * names files will be renamed to */
        
        void editListValues(); /**< member function that displays an Imgui window for users
                                * to add/remove keywords and file extensions */
        
        void createDir(); /**< member function that creates directories named after elements
                           * stored in "new_names" */
        
        void renameFiles(); /**< member function that renames file names stored in "old_names"
                             * to the names of the elements stored in "new_names" */
        
        void debug(); /**< member function that displays the contents of "old_names" and "new_names
                       * for the user's reference */
        
        void createFiles(int); /**< member function that creates a set amount of empty text files
                                * determined by the value of the int variable given */
};

#endif