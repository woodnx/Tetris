#include "BlockId.h"

BlockId::BlockId(int id, const std::string& name, int r, int g, int b):
    id(id),
    name(name),
    r(r),
    g(g),
    b(b) {}

const shared_ptr<BlockId> BlockId::Null =
  make_shared<BlockId>(-1, "Null", 0, 0, 0);
const shared_ptr<BlockId> BlockId::Empty =
  make_shared<BlockId>(0, "Empty", 0, 0, 0);
const shared_ptr<BlockId> BlockId::Imino =
  make_shared<BlockId>(1, "Imino", 0, 191, 255);
const shared_ptr<BlockId> BlockId::Lmino =
  make_shared<BlockId>(2, "Lmino", 255, 165, 0);
const shared_ptr<BlockId> BlockId::Jmino =
  make_shared<BlockId>(3, "Jmino", 65, 105, 225);
const shared_ptr<BlockId> BlockId::Smino =
  make_shared<BlockId>(4, "Smino", 50, 205, 50);
const shared_ptr<BlockId> BlockId::Zmino =
  make_shared<BlockId>(5, "Zmino", 255, 99, 71);
const shared_ptr<BlockId> BlockId::Omino =
  make_shared<BlockId>(6, "Omino", 255, 255, 102);
const shared_ptr<BlockId> BlockId::Tmino =
  make_shared<BlockId>(7, "Tmino", 218, 112, 214);

int BlockId::color() {
  return GetColor(r, g, b);
}

const shared_ptr<BlockId> BlockId::from_id(int id) {
  switch (id) {
    case -1:
      return Null;
    case 0:
      return Empty;
    case 1:
      return Imino;
    case 2:
      return Lmino;
    case 3:
      return Jmino;
    case 4:
      return Smino;
    case 5:
      return Zmino;
    case 6:
      return Omino;
    case 7:
      return Tmino;
    default:
      return Empty;
  }
}
