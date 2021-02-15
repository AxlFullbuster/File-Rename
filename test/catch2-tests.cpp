#include <catch.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::ofstream;
using std::string;
using std::vector;

namespace fs = boost::filesystem;

/*
 *methods and variables for testing
 */

string test_path = "/mnt/c/Users/Tdial/Desktop/coding-work/C++/Linux/File-Rename/test/build";

void createFiles(int n){
    fs::create_directory(test_path + "/Files");
    
    for(int i = 1; i <= n; i++){
        string num = std::to_string(i);
        
        string name = "Test " + num + ".txt";
        ofstream file ("./Files/" + name);
        file.close();
    }
}


/*
 *Catch2 Test cases
 */

TEST_CASE( "Sanity Case", "[multi-file:1]" ){
    SECTION( "Check that tests are possible"){
        int example = 1;
        int counter_example = 1;
        
        REQUIRE(example == counter_example);
    }
}

TEST_CASE("Base Case", "[base]"){
    SECTION("1 singular file and a directory can be created."){
        createFiles(1);
        
        REQUIRE(fs::exists(test_path + "/Files/Test1.txt") == true);
        fs::remove_all(test_path + "/Files");
    }
    
    SECTION("Multiple files can be created and put into a directory."){
        createFiles(10);
        
        for(int i = 1; i <= 10; i++){
            string num = std::to_string(i);
            string name = "Test" + num + ".txt";
            REQUIRE(fs::exists(test_path + "/Files/" + name) == true);
        }
        
        fs::remove_all(test_path + "/Files");   
    }
}

TEST_CASE("Renaming", "[rename]"){
    SECTION("A single file can be renamed to a predetermined name"){
        fs::create_directory(test_path + "/Files");
        
        string new_name = "Hello.txt";
        
        ofstream test_file("Files/Test.txt");
        test_file.close();
        
        fs::rename(test_path + "/Files/Test.txt", test_path + "/Files/" + new_name);
        
        REQUIRE(fs::exists(test_path + "/Files/" + new_name) == true);
        
        fs::remove_all(test_path + "/Files"); 
    }
}

TEST_CASE("Multiple Renaming", "[rename]"){
    vector<string> new_names = {"Hi.txt", "Hello.txt", "Greetings.txt", "Salutations.txt"};
    vector<string> old_names = {"Test 1.txt", "Test 2.txt", "Test 3.txt", "Test 4.txt"};
    createFiles(4);
    string path = test_path + "/Files/";

    SECTION("Multiple files in a directory can be renamed"){
        
        for(int i = 0; i < 4; i++){
            fs::rename(path + old_names[i], path + new_names[i]);
        }
        
        for(int i = 0; i < 4; i++){
            REQUIRE(fs::exists(path + new_names[i]) == true);
        }
        
        fs::remove_all(path);
    }
    
}

