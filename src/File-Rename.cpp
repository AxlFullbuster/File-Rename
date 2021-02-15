#include <File-Rename.h>
#include<iostream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;


namespace fs = boost::filesystem;


FileRename::FileRename(){
    //Do Nothing
}

FileRename::~FileRename(){
    //Do Nothing
}

/*
 * Method for testing MAKE SURE TO REMOVE
 */
void FileRename::createFiles(int n){
    fs::create_directory(test_path + "/Files");
    
    for(int i = 1; i <= n; i++){
        string num = std::to_string(i);
        
        string name = "Test " + num + ".txt";
        ofstream file ("./Files/" + name);
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
    ImGui::NextColumn();
    
    ImGui::Text("Loaded Titles");
    ImGui::Separator();
    
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
        input = false;
    }
    
    title_count = titles.size();
    
    ImGui::End();
  
}


void FileRename::filePreview(){
    ImGui::Begin("File Rename Preview");
    
    ImGui::Text("Here you can select a keyword to prefix your titles, and see a preview of the renamed file");
    ImGui::NewLine();
    
    ImGui::Columns(2);
    
    static const char * keywords[]{"Arc", "Part", "Chapter", "Episode", "Volume"};
    static int selected_key = 0;
    
    static const char * file_ext[]{".txt", ".mp4", ".mvk"};
    static int selected_ext = 0;

    
    ImGui::Text("Keyword Selection");
    ImGui::ListBox("Keywords", &selected_key, keywords, IM_ARRAYSIZE(keywords));
    ImGui::ListBox("File Extensions", &selected_ext, file_ext, IM_ARRAYSIZE(file_ext));
    
    ImGui::NextColumn();
    ImGui::Text("Preview");
    ImGui::Separator();
    
    string new_filename = "";
    string key(keywords[selected_key]);
    string ext(file_ext[selected_ext]);
    
    
    for(int i = 0; i < titles.size(); i++){
        
        string numeral = std::to_string(i + 1);
        string title = titles[i];                              
        new_filename = key + " " + numeral + " - " + title;
        
        if(!dir) new_filename += ext;
        
        new_names[i] = new_filename;
        
        ImGui::Text("Item %i: %s", i + 1, new_filename.c_str());
    }

    titles.clear();
    
    ImGui::End();
     
}

void FileRename::createDir(){
    for(int i = 0; i < new_names.size(); i++){
        fs::create_directory(dir_path + '/' + new_names[i]);
    }
    
    old_names.clear();
}

void FileRename::renameFiles(){
    for(int i = 0; i < 8; i++){
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



