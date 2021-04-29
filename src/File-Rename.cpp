#include "File-Rename.h"
#include<iostream>

using std::vector;
using std::string;
using std::ifstream;
using std::fstream;
using std::sort;
using std::min;


namespace fs = boost::filesystem;


FileRename::FileRename(){
    dir_path = "";
    
    //uncomment line below to create empty txt files for testing
    //createFiles(10);
}

FileRename::~FileRename(){
    //Do Nothing
}

void FileRename::createFiles(int n){
    string create_path = "../test/Files/";
    fs::create_directory(create_path);
    
    for(int i = 1; i <= n; i++){
        string num = "";
        if(i < 10) num += '0';
        num += std::to_string(i);
        
        string name = "Test " + num + ".txt";
        fstream file (create_path + name, fstream::out);
        file.close();
    }
}

/**
 * The window displays two columns
 * the first column contains buttons for directory selection,
 * renaming files, and creating directories. It also contains
 * checkboxes for user confirmation. 
 *
 * The second column contains the current selected directory.
 *
 * The method will add the file names found in the chosen directory
 * into the "old_names" vector.
 *
 * This window will be inaccessable to the user if an input file
 * is not loaded and the user is not ignoring this requirement.
 *
 * @see old_names
 * @see input
 * @see ignore
 * @see file
 * @see dir
 * @see dir_path
 * @see inputFile()
 */
void FileRename::selectionTool(){
    ImGui::SetNextWindowPos(ImVec2(231,9), ImGuiCond_FirstUseEver, ImVec2(0.0f,0.0f));
    ImGui::SetNextWindowSize(ImVec2(898,193), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Customize Selection Criteria");
    
    ImGui::Columns(2);
    
    ImGui::Text("Selection Buttons");
    
    if(!input){
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    
    if (ImGui::Button("Open Directory Selection")){
        old_names.clear();
        ImGuiFileDialog::Instance()->OpenDialog("ChooseDir", "Choose a Directory", 0, "/home/");
        ImGui::SetNextWindowPos(ImVec2(60,60), ImGuiCond_FirstUseEver, ImVec2(0.0f,0.0f));
        ImGui::SetNextWindowSize(ImVec2(882,361), ImGuiCond_FirstUseEver);
    }
    
    ImGui::Checkbox("Check when renaming files.", &file);
    ImGui::Checkbox("Check when creating directories.", &dir);

    
    if(ImGui::Button("Rename Files") && file){
        renameFiles();
    }

    if(ImGui::Button("Create Directories") && dir){
        createDir();
    }
    
    if (ImGuiFileDialog::Instance()->Display("ChooseDir")){
      if (ImGuiFileDialog::Instance()->IsOk()){
        dir_path = ImGuiFileDialog::Instance()->GetCurrentPath();
        for(auto& entry: fs::directory_iterator(dir_path)){
            if (fs::is_regular_file(entry.path())) {
               old_names.push_back(entry.path().c_str());
            }
        }
      }
      sort(old_names.begin(), old_names.end());
      ImGuiFileDialog::Instance()->Close();
    }
    
    ImGui::NextColumn();
    
    fs::path dir = dir_path;
    ImGui::Text("Selected Directory");
    ImGui::Separator();
    
    
    ImGui::Text("Directory:%s%s", "../" , dir.filename().c_str());
    
    if(!input){
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    
    ImGui::End();
}

/**
 * The window is resonsible for handling input files.
 *
 * Contains a button for selecting an input file, a checkbox to
 * ignore the input file requirement, and two text fields that will
 * hold integer values.
 *
 * The first text field is resposible for the numeral offset of the number
 * displayed before the new name of a file. The limit is between 1 and 1,000.
 * The value of "num" is then updated to this value.
 *
 * The second text field is used when the user is ignoring the input file
 * requirement and is used to set a limit of how many directories to create.
 * The limit is between 1 and 10. This field is inaccessable otherwise.
 *
 * Regardless of wether or not an input file was chosen. The "titles" vector
 * has elements added into it and "title_count" is updated to hold the current
 * size of the vector.
 *
 * "new_names" is also resized to the current value of "title_count".
 *
 * Note: method contains a check for the carriage return character('/r')
 * when reading from an input file on a windows machine. This is done to
 * prevent an unrecognizeable character from appearing in the renamed file.
 *
 * @see new_names
 * @see titles
 * @see title_count
 * @see num
 * @see filename
 * @see selectpath
 * @see ignore
 * @see input
 * @see file
 * @see dir
 */
void FileRename::inputFile(){
    ImGui::SetNextWindowPos(ImVec2(6,282), ImGuiCond_FirstUseEver, ImVec2(0.0f,0.0f));
    ImGui::SetNextWindowSize(ImVec2(539,202), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Input Text File");
    
    
    ImGui::Text("This is the window where you can load a text file with pre-defined titles.");
    ImGui::NewLine();
    
    ImGui::Columns(2);
    ImGui::Text("File Selection");
    
    if (ImGui::Button("Load input file")){
        ImGuiFileDialog::Instance()->OpenDialog("InputFile", "Select Files", ".frename", "/home/");
        ImGui::SetNextWindowPos(ImVec2(60,60), ImGuiCond_FirstUseEver, ImVec2(0.0f,0.0f));
        ImGui::SetNextWindowSize(ImVec2(803,377), ImGuiCond_FirstUseEver);
    }
    
    if (ImGuiFileDialog::Instance()->Display("InputFile")){
      if (ImGuiFileDialog::Instance()->IsOk()){
        filename = ImGuiFileDialog::Instance()->GetCurrentFileName();
        selectpath = ImGuiFileDialog::Instance()->GetFilePathName();
      }
      ImGuiFileDialog::Instance()->Close();
    }
    
    ImGui::Text("Current Loaded File:%s", filename.c_str());
    ImGui::Checkbox("Ignore input file.", &ignore);
    
    static int offset = 0;
    ImGui::InputInt("Numeral Offset", &offset);
    if(offset > 1000)  offset = 1000;
    else if(offset < 1) offset = 1;
    num = offset;
    
    static int limit = 0;
    if(!ignore){
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    ImGui::InputInt("File Limit", &limit);
    
    if(limit > 10) limit = 10;
    else if(limit < 1) limit = 1;
    
    ImGui::NextColumn();
    
    ImGui::Text("Loaded Titles");
    
    ImGui::Separator();
    
    if(ignore){
        while(titles.size() != limit) titles.push_back("");
        title_count = titles.size();
        new_names.resize(title_count);
        dir = true;
        input = true;
        file = false;
        
        ImGui::Text("Ignoring Input File.");
        
        ImGui::End();
        return;
    }
    

    ifstream loaded_file(selectpath);
    string line;
    
    if(loaded_file.is_open()){
        while(getline(loaded_file, line)){
            ImGui::Text("%s", line.c_str());
            if(line.at(line.size() - 1) == '\r') line = line.erase(line.size() - 1).c_str();
            titles.push_back(line);
        }
        
        loaded_file.close();
        input = true;
    }else{
        ImGui::Text("No Titles Found.");
    }
    
    title_count = titles.size();
    new_names.resize(title_count);
    
    if(!ignore){
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    
    ImGui::End(); 
}

/**
 * The window gives the user the ability to add
 * and remove keywords and file extensions.
 *
 * Two text fields that accept string values are created here.
 * One is for keywords and the other is for file extensions.
 *
 * Two buttons are put at the bottom of each text field that will
 * add or remove the value from "keywords" and "file_ext".
 *
 * Note: The length of the string characters cannot exceed 20 characters.
 * 
 * Note: A limit is set for both vectors to have no more than
 * 10 values.
 * 
 * @see keywords
 * @see file_ext
 * @see filePreview()
 */
void FileRename::editListValues(){
    ImGui::SetNextWindowPos(ImVec2(547,281), ImGuiCond_FirstUseEver, ImVec2(0.0f,0.0f));
    ImGui::SetNextWindowSize(ImVec2(335,203), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Add/Remove Keywords/File Extensions");
    
    ImGui::Text("Total Keys:%i", keywords.size());
    ImGui::Text("Total Extensions:%i", file_ext.size());
    
    static char key_buffer[20] = "";
    ImGui::InputText("Add Keyword", key_buffer, IM_ARRAYSIZE(key_buffer));
    
    if(ImGui::Button("Add a new keyword")){
        string key_input = key_buffer;
        if(keywords.size() < 10){
            keywords.push_back(key_input);
        }
    }
    
    if(ImGui::Button("Remove entered keyword")){
        string key_input = key_buffer;
        if(!keywords.empty()){
            for(int i = 0; i < keywords.size(); i++){
                if(keywords[i].compare(key_input) == 0){
                    keywords.erase(keywords.begin() + i);
                }
            }
        }
    }
    
    
    static char ext_buffer[20] = "";
    
    ImGui::InputText("Add Extension", ext_buffer, IM_ARRAYSIZE(ext_buffer));
    if(ImGui::Button("Add a new extension")){
        string ext_input = ext_buffer;
        if(file_ext.size() < 10){
            file_ext.push_back(ext_input);
        }
    }
    
     if(ImGui::Button("Remove entered extension")){
        string ext_input = ext_buffer;
        if(!file_ext.empty()){
            for(int i = 0; i < file_ext.size(); i++){
                if(file_ext[i].compare(ext_input) == 0){
                    file_ext.erase(file_ext.begin() + i);
                }
            }
        }
    }
    
    
    ImGui::End();
}

/**
 * The window displays the keyword and file extensions 
 * that are selectable, and a preview of what the renamed files
 * will look like. It is split into 2 columns.
 *
 * The first column displays two boxes where the user can select a keyword and
 * file extension. The contents of these boxes contain the elements of
 * "keyword" and "file_ext".
 *
 * The second column displays the contents of "new_names" where each
 * element contains:
 * the combined values of a keyword, a numeral value determined by "num",
 * the elements in "titles", and a file extension.
 *
 * Note: the file extension will be ommited if the user is creating directories
 * Note: the "titles "vector" is cleared at the end of this method
 *
 * @see new_names
 * @see titles
 * @see keywords
 * @see file_ext
 */
void FileRename::filePreview(){
    ImGui::SetNextWindowPos(ImVec2(883,208), ImGuiCond_FirstUseEver, ImVec2(0.0f,0.0f));
    ImGui::SetNextWindowSize(ImVec2(651,358), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("File Rename Preview");
    
    ImGui::Text("Here you can select a keyword to prefix your titles, and see a preview of the renamed file");
    ImGui::NewLine();
    
    ImGui::Columns(2);
    
    static int selected_key = 0;
    static int selected_ext = 0;
    
    ImGui::Text("Keyword/File Selection");
    
    if(ImGui::BeginListBox("Keywords")){

        for(int i = 0; i < keywords.size(); i++){
            const bool selected = (selected_key == i);
            string value = keywords[i];
            
            if(ImGui::Selectable(value.c_str(), selected)){
               selected_key = i;
            }
        }
        
        ImGui::EndListBox();
    }
    
    if(ImGui::BeginListBox("File Extensions")){

        for(int i = 0; i < file_ext.size(); i++){
            const bool selected = (selected_ext == i);
            string value = file_ext[i];
            
            if(ImGui::Selectable(value.c_str(), selected)){
               selected_ext = i;
            }
        }
        
        ImGui::EndListBox();
    }
    
    ImGui::NextColumn();
    
    ImGui::Text("Preview");
    ImGui::Separator();
    
    string new_filename = "";
    string key;
    string ext;
    
    if(!keywords.empty() && !file_ext.empty() ){
        key = keywords[selected_key];
        ext = file_ext[selected_ext];
    }
    
    
    for(int i = 0; i < titles.size(); i++){
        string numeral = "";
        if((i + num) <= 9) numeral += '0';
        numeral += std::to_string(i + num);
        
        string title = titles[i];
        
        if(ignore && dir){
            new_filename = key + " " + numeral + title;
        }else{
            new_filename = key + " " + numeral + " - " + title;
        }
        
        if(!dir) new_filename += ext;
        
        new_names[i] = new_filename;
        
        ImGui::Text("Item %i: %s", i + 1, new_filename.c_str());
    }

    titles.clear();
    
    ImGui::End();  
}

/**
 * Creates a set amount of directories specified either by
 * the value of "title_count" or by the size of "new_names".
 *
 * The directories will be put in the path specificed in
 * "dir_path".
 *
 * @see ignore
 * @see title_count
 * @see new_names
 * @see dir_path
 */
void FileRename::createDir(){
    if(ignore){
        for(int i = 0; i < title_count; i++){
            fs::create_directory(dir_path + '/' + new_names[i]);
        }
    }else{
        for(int i = 0; i < new_names.size(); i++){
            fs::create_directory(dir_path + '/' + new_names[i]);
        }
    }
    
}

/**
 * Renames the filenames in "old_names" to the
 * ones in "new_names". The method will not rename
 * files if both elements in "new_names" and "old_names"
 * are equal.
 *
 * Note: "old_names' is cleared at the end of this method.
 * 
 * @see new_names
 * @see old_names
 * @see dir_path
 *
 */
void FileRename::renameFiles(){
    int file_count = old_names.size();
    int size_count = min(file_count, title_count);
    
    string old_name = old_names[0].substr(dir_path.length() + 1);
    
    for(int i = 0; i < size_count ; i++){
        if(old_name.compare(new_names[i]) != 0){
            fs::rename(old_names[i], dir_path + '/' + new_names[i]);
        }else{
            continue;
        }
    }
    
    old_names.clear();
}


void FileRename::debug(){
    ImGui::SetNextWindowPos(ImVec2(99,620), ImGuiCond_FirstUseEver, ImVec2(0.0f,0.0f));
    ImGui::SetNextWindowSize(ImVec2(1284,128), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Debugger");
    
    int file_count = old_names.size();
    int size_count = min(file_count, title_count);
    ImGui::Columns(3);
    
    ImGui::Text("Boolean/Int Values");
    ImGui::Text("Currently Creating Directories:%i", dir);
    ImGui::Text("Currently Renaming Files:%i", file);
    ImGui::Text("Title Count:%i", title_count);
    ImGui::Text("Size Count:%i", size_count);
    
    ImGui::NextColumn();
    
    ImGui::Text("Old Filenames");
    for(int i = 0; i < size_count; i++){
        string old_name = old_names[i].substr(dir_path.length() + 1);
        ImGui::Text("%s", old_name.c_str());
    }
    
    ImGui::NextColumn();
    
    ImGui::Text("New FileNames");
    ImGui::Separator();
    
    for(int i = 0; i < new_names.size(); i++){
        ImGui::Text("%s", new_names[i].c_str());
    }
    
    
    ImGui::End();
}

/**
 * @see selectionTool()
 * @see inputFile()
 * @see editListValues()
 * @see debug()
 * @see window
 */
void FileRename::drawDisplay(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    
    ImGui::NewFrame();
    
    selectionTool();
    inputFile();
    filePreview();
    editListValues();
    debug();
    
    ImGui::EndFrame();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

/**
 * @see window
 * @see gl_context
 */
bool FileRename::initWindow(){
    bool success = true;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf ( "SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
	}
    
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
     // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("FileRename", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    
 
    // Initialize OpenGL loader
    #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
        bool err = gl3wInit() != 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
        bool err = glewInit() != GLEW_OK;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
        bool err = gladLoadGL() == 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
        bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
        bool err = false;
        glbinding::Binding::initialize();
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
        bool err = false;
        glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
    #else
        bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
    #endif
    
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
    
    
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    return success;
}

/**
 * @see window
 * @see gl_context
 */
void FileRename::closeWindow(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}