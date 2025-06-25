#pragma once
#include "DxLib.h"

#include <filesystem>
#include <iostream>
#include <map>
#include <regex>
#include <string>

using namespace std;
using std::filesystem::directory_iterator;

class Sound {
  private:
    map<string, int> mp;
    bool getFileNames(string path, std::vector<string> &file_names);
    bool isNotFullWidthChar(string file_name);

  public:
    int add(string handle_name, string path);
    int addFromDirectory(string path);
    int play(string handle_name, int playmode);
    int stop(string handle_name);
    int changeVolume(string handle_name, int volume);
    int changeAllSoundVolume(int volume);

    void finalize();
};
