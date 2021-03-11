#include <File-Rename.h>
#include<iostream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::fstream;


namespace fs = boost::filesystem;


FileRename::FileRename(){
    //uncomment line below to create empty txt files for testing
    //createFiles(25);
}

FileRename::~FileRename(){
    //Do Nothing
}

/*
 * Method for testing MAKE SURE TO REMOVE
 */
void FileRename::createFiles(int n){
    string create_path = test_path + "/Files/";
    fs::create_directory(create_path);
    
    for(int i = 1; i <= n; i++){
        string num = std::to_string(i);
        
        string name = "Test " + num + ".txt";
        fstream file (create_path + name, fstream::out);
        file.close();
    }
}

void FileRename::selectionTool(){
    ImGui::Begin("Customize Selection Criteria");
    
    ImGui::Columns(2);
    
    ImGui::Text("Selection Buttons");
    
    if(!input){
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    
    if (ImGui::Button("Open Directory Selection")){
        igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseDir", "Choose a Directory", 0, windowpath);
    }
    
    ImGui::Checkbox("Check to allow renaming/creation.", &ready);
    ImGui::Checkbox("Check when creating directories.", &dir);

    
    if(ImGui::Button("Rename Files")){
        if(!ready){
            ImGui::Text("Rename Status: Error");
        }
        else if(ready && dir){
            ImGui::Text("Rename Status: Conflict between directory/files");
        }else{
            renameFiles();
        }
    }

    
    if(ImGui::Button("Create Directories")){
        if(!dir){
            ImGui::Text("Creation Status: Error");
        }else{
            createDir();
        }
    }
    
    if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseDir")){
      if (igfd::ImGuiFileDialog::Instance()->IsOk == true){
        dir_path = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
        for(auto& entry: fs::directory_iterator(dir_path)){
            old_names.push_back(entry.path().c_str());
        }
      }
      igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseDir");
    }
    
    ImGui::NextColumn();
    
    ImGui::Text("Selected Files/Directory");
    ImGui::Separator();
    
    ImGui::Text("Directory:%s", dir_path.c_str());
    
    if(!input){
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    
    ImGui::End();
}



void FileRename::inputFile(){
    ImGui::Begin("Input Text File");
    
    
    ImGui::Text("This is the window where you can load a text file with pre-defined titles.");
    ImGui::NewLine();
    
    ImGui::Columns(2);
    ImGui::Text("File Selection");
    
    if (ImGui::Button("Load input text file")){
        igfd::ImGuiFileDialog::Instance()->OpenDialog("InputFile", "Select Files", ".txt", windowpath);
    }

    
     if (igfd::ImGuiFileDialog::Instance()->FileDialog("InputFile")){
      if (igfd::ImGuiFileDialog::Instance()->IsOk == true){
        filename = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
        selectpath = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
        filepath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath() + '/';
      }
      igfd::ImGuiFileDialog::Instance()->CloseDialog("InputFile");
    }
    
    ImGui::Text("Current Loaded File:%s", filename.c_str());
    ImGui::Checkbox("Ignore input file.", &ignore);
    
    static int limit = 0;
    if(!ignore){
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    ImGui::InputInt("File Limit", &limit);
    
    if(limit > 10) limit = 10;
    else if(limit < 0) limit = 0;
    
    ImGui::NextColumn();
    
    ImGui::Text("Loaded Titles");
    
    ImGui::Separator();
    
    if(ignore){
        while(titles.size() != limit) titles.push_back("");
        title_count = titles.size();
        dir = true;
        input = true;
        ready = true;
        
        ImGui::Text("Ignoring Input File.");
        
        ImGui::End();
        return;
    }
    

    ifstream loaded_file(selectpath);
    string line;
    
    if(loaded_file.is_open()){
        while(getline(loaded_file, line)){
            ImGui::Text("%s", line.c_str());
            line = line.erase(line.size() - 1).c_str();
            titles.push_back(line);
        }
        
        loaded_file.close();
        input = true;
    }else{
        ImGui::Text("No Titles Found.");
    }
    
    title_count = titles.size();
    
    if(!ignore){
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    
    ImGui::End(); 
}

void FileRename::editListValues(){
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

void FileRename::filePreview(){
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
        string numeral = std::to_string(i + 1);
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

void FileRename::renameFiles(){
    for(int i = 0; i < old_names.size(); i++){
        fs::rename(old_names[i], dir_path + '/' + new_names[i]);
    }
    
    old_names.clear();
}

void FileRename::debug(){
    ImGui::Begin("Debugger");
    
    ImGui::Columns(4);
    
    ImGui::Text("Boolean/Int Values");
    ImGui::Text("Currently Creating Directories:%i", dir);
    ImGui::Text("Currently Renaming Files:%i", ready);
    ImGui::Text("Title Count:%i", title_count);
    
    ImGui::NextColumn();
    
    ImGui::Text("Old Filenames");
    for(int i = 0; i < old_names.size(); i++){
        ImGui::Text("%s", old_names[i].c_str());
    }
    
    ImGui::NextColumn();
    
    ImGui::Text("New FileNames");
    for(int i = 0; i < new_names.size(); i++){
        ImGui::Text("%s", new_names[i].c_str());
    }
    
    ImGui::NextColumn();
    
    ImGui::Text("File Paths");
    ImGui::Separator();
   
    
    ImGui::End();
    
}

void FileRename::drawDisplay(){
    SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
    SDL_RenderClear(renderer);
  
    ImGui::NewFrame();
    
    selectionTool();
    inputFile();
    filePreview();
    editListValues();
    debug();
    
    ImGui::EndFrame();
    
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());

	SDL_RenderPresent(renderer);
    
    SDL_Delay(10);
}

bool FileRename::initWindow(){
    bool success = true;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf ( "SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
	}
    
    
    window = SDL_CreateWindow("File_Rename", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    }
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 160*2, 144*2);
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, 640,320);
    
    return success;
}

void FileRename::closeWindow(){
    
    ImGuiSDL::Deinitialize();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    renderer = NULL;
    window = NULL;
    texture = NULL;
    
	ImGui::DestroyContext();
    SDL_Quit();
}



