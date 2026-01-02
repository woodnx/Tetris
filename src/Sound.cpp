#include "Sound.h"

bool Sound::getFileNames(string directory_path, vector<string>& file_names) {
  WIN32_FIND_DATAA fd;
  HANDLE hFind = FindFirstFileA((directory_path + "\\*").c_str(), &fd);

  if (hFind == INVALID_HANDLE_VALUE) {
    return false;
  }

  do {
    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      int required_size =
        MultiByteToWideChar(CP_ACP, 0, (CHAR*) fd.cFileName, -1, nullptr, 0);
      if (required_size > 0) {
        std::vector<wchar_t> wbuf(required_size);
        MultiByteToWideChar(
          CP_ACP,
          0,
          (CHAR*) fd.cFileName,
          -1,
          wbuf.data(),
          required_size);
        std::wstring ws(wbuf.data());
        string str(ws.begin(), ws.end());
        file_names.push_back(str);
      }
    }
  } while (FindNextFile(hFind, &fd));

  FindClose(hFind);

  return true;
}

bool Sound::isNotFullWidthChar(string file_name) {
  for (int i = 0; i < file_name.size(); i++) {
    if (file_name.c_str()[i] > 128) {
      return false;
    }
  }
  return true;
}

int Sound::add(const string& handle_name, const string& file_path) {
  int h = LoadSoundMem(file_path.c_str());
  if (h == -1) {
    MessageBoxA(
      NULL,
      ("LoadSoundMem failed:\n" + file_path).c_str(),
      "Sound error",
      MB_OK);
    return -1;
  }
  mp[handle_name] = h;
  return 0;
}

int Sound::addFromDirectory(string directory_path) {
  vector<string> file_names;
  smatch m;

  if (!getFileNames(directory_path, file_names)) {
    return -1;
  }
  for (string name : file_names) {
    if (isNotFullWidthChar(name)) {
      regex_match(name, m, regex(R"((\w+)(\.))"));
      add(m[0].str(), m[0].str());
    }
  }

  return 0;
}

int Sound::play(const string& handle_name, int play_type) {
  auto it = mp.find(handle_name);
  if (it == mp.end()) {
    MessageBoxA(
      NULL,
      ("Sound key not found:\n" + handle_name).c_str(),
      "Sound error",
      MB_OK);
    return -1;
  }
  if (it->second == -1) {
    MessageBoxA(
      NULL,
      ("Invalid sound handle:\n" + handle_name).c_str(),
      "Sound error",
      MB_OK);
    return -1;
  }
  if (PlaySoundMem(it->second, play_type) == -1) {
    MessageBoxA(
      NULL,
      ("PlaySoundMem failed:\n" + handle_name).c_str(),
      "Sound error",
      MB_OK);
    return -1;
  }
  return 0;
}

int Sound::stop(const string& handle_name) {
  auto it = mp.find(handle_name);
  if (it == mp.end() || it->second == -1) {
    MessageBoxA(
      NULL,
      ("Sound key not found or invalid:\n" + handle_name).c_str(),
      "Sound error",
      MB_OK);
    return -1;
  }
  return (StopSoundMem(it->second) == -1) ? -1 : 0;
}

int Sound::changeVolume(const string& handle_name, int volume) {
  if (volume < 0 || volume > 100)
    return -1;

  auto it = mp.find(handle_name);
  if (it == mp.end() || it->second == -1) {
    MessageBoxA(
      NULL,
      ("Sound key not found or invalid:\n" + handle_name).c_str(),
      "Sound error",
      MB_OK);
    return -1;
  }
  return (ChangeVolumeSoundMem(255 * volume / 100, it->second) == -1) ? -1 : 0;
}

int Sound::changeAllSoundVolume(int volume) {
  if (volume <= 100 && volume >= 0) {
    for (auto itr = mp.begin(); itr != mp.end(); ++itr) {
      ChangeVolumeSoundMem(255 * volume / 100, itr->second);
    }
    return 0;
  } else {
    return -1;
  }
}

void Sound::finalize() {
  for (auto& [k, h] : mp) {
    if (h != -1)
      DeleteSoundMem(h); // DxLibの関数
  }
  mp.clear();
}
