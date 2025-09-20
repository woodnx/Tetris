#pragma once
#include "DxLib.h"

#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

class BlockId {
  public:
    const int id;
    const string name;
    const int r, g, b;

    BlockId(int id, const std::string& name, int r, int g, int b);
    int color();
    static const shared_ptr<BlockId> from_id(int id);

    static const shared_ptr<BlockId> Null;
    static const shared_ptr<BlockId> Empty;
    static const shared_ptr<BlockId> Imino;
    static const shared_ptr<BlockId> Lmino;
    static const shared_ptr<BlockId> Jmino;
    static const shared_ptr<BlockId> Smino;
    static const shared_ptr<BlockId> Zmino;
    static const shared_ptr<BlockId> Omino;
    static const shared_ptr<BlockId> Tmino;

  private:
    static const unordered_map<int, shared_ptr<BlockId>>& id_map();
};