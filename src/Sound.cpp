#include "Sound.h"

bool Sound::getFileNames(string directory_path, vector<string> &file_names) {
  HANDLE hFind;
  WIN32_FIND_DATA win32fd;
  string serch_name = directory_path + "\\*";

  hFind = FindFirstFile(serch_name.c_str(), &win32fd);

  if (hFind == INVALID_HANDLE_VALUE) {
    return false;
  }

  do {
    if (!(win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      file_names.push_back(win32fd.cFileName);
    }
  } while (FindNextFile(hFind, &win32fd));

  FindClose(hFind);

  return true;
}

bool Sound::isNotFullWidthChar(string file_name) {
  for (int i = 0; file_name.size(); i++) {
    if (file_name.c_str()[i] > 128) {
      return false;
    }
  }
  return true;
}

int Sound::add(string handle_name, string file_path) {
  int handle      = LoadSoundMem(file_path.c_str());
  mp[handle_name] = handle;

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

/// <summary>
/// Mapに格納した音声データを再生
/// </summary>
/// <param name="handle_name"></param>
/// <param name="playmode">
/// DX_PLAYTYPE_NORMAL : ノーマル再生,
/// DX_PLAYTYPE_BACK : バックグラウンド再生,
/// DX_PLAYTYPE_LOOP : ループ再生
/// </param>
/// <returns></returns>
int Sound::play(string handle_name, int play_type) {
  PlaySoundMem(mp[handle_name], play_type);
  return 0;
}

int Sound::stop(string handle_name) {
  StopSoundMem(mp[handle_name]);
  return 0;
}

/// <summary>
/// Soundの音量を調整
/// </summary>
/// <param name="handle_name"></param>
/// <param name="volume">音量（0~100）</param>
/// <returns></returns>
int Sound::changeVolume(string handle_name, int volume) {
  if (volume <= 100 && volume >= 0) {
    ChangeVolumeSoundMem(255 * volume / 100, mp[handle_name]);
    return 0;
  }
  else {
    return -1;
  }
}

int Sound::changeAllSoundVolume(int volume) {
  if (volume <= 100 && volume >= 0) {
    for (auto itr = mp.begin(); itr != mp.end(); ++itr) {
      ChangeVolumeSoundMem(255 * volume / 100, itr->second);
    }
    return 0;
  }
  else {
    return -1;
  }
}

void Sound::finalize() {
  InitSoundMem();
}
