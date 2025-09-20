#include "Bag.h"

Bag::Bag() {
  bag.resize(Bag::BAG_SIZE);
  // すべて Empty で初期化（nullptr を残さない）
  fill(bag.begin(), bag.end(), BlockId::Empty);

  joinNexts(0);
  joinNexts(TOTAL_MINOS);
}

shared_ptr<BlockId> Bag::increase() {
  shared_ptr<BlockId> head = bag[0];

  for (int i = 0; i < BAG_SIZE - 1; i++) {
    bag[i] = bag[i + 1];
  }
  bag[BAG_SIZE - 1] = BlockId::Empty;

  if (countNexts() <= NEXT_REFERS) {
    joinNexts(NEXT_REFERS);
  }

  return head;
}

shared_ptr<BlockId> Bag::getMinoId(int index) {
  return bag[index];
}

vector<shared_ptr<BlockId>> Bag::generateBlockIds() {
  vector<shared_ptr<BlockId>> b = vector<shared_ptr<BlockId>>(TOTAL_MINOS);

  for (int i = 0; i < TOTAL_MINOS; i++) {
    b[i] = BlockId::from_id(i + 1);
  }
  return b;
}

vector<shared_ptr<BlockId>> Bag::generateShuffledNexts() {
  vector<shared_ptr<BlockId>> b = generateBlockIds();

  // Fisher–Yates: j は [0, i]
  for (int i = static_cast<int>(b.size()) - 1; i > 0; --i) {
    int j = this->random(i);
    swap(b[i], b[j]);
  }
  return b;
}

int Bag::random(int max) {
  // 高速ループでも偏らないよう、一度だけ seed
  static thread_local mt19937 gen(random_device{}());
  uniform_int_distribution<int> dist(0, max);
  return dist(gen);
}

int Bag::countNexts() {
  int sum = 0;
  for (int i = 0; i < BAG_SIZE; i++) {
    // nullptr も Empty と同義で数えない
    if (bag[i] && bag[i] != BlockId::Empty)
      sum++;
  }
  return sum;
}

void Bag::joinNexts(int pivot) {
  vector<shared_ptr<BlockId>> newNexts = generateShuffledNexts();
  // 入る分だけ詰める（超えたら打ち切り）
  int end = min<int>(BAG_SIZE, pivot + static_cast<int>(newNexts.size()));
  for (int i = pivot; i < end; i++) {
    bag[i] = newNexts[i - pivot];
  }
}