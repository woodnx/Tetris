#pragma once
#include "DxLib.h"

#include <filesystem>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;
using std::filesystem::directory_iterator;

class Sound {
  private:
    map<string, int> mp;
    bool getFileNames(string path, std::vector<string>& file_names);
    bool isNotFullWidthChar(string file_name);

  public:
    int add(const string& handle_name, const string& file_path);
    int addFromDirectory(string path);
    int play(const string& handle_name, int playmode);
    int stop(const string& handle_name);
    int changeVolume(const string& handle_name, int volume);
    int changeAllSoundVolume(int volume);

    void finalize();
};
