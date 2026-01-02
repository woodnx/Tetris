#include "Bag.h"

void Bag::init() {
  bag.resize(BAG_SIZE);
  fill(bag.begin(), bag.end(), BlockId::Empty);

  join_nexts(0);
  join_nexts(TOTAL_MINOS);
}

shared_ptr<BlockId> Bag::increase() {
  shared_ptr<BlockId> head = bag[0];

  for (int i = 0; i < BAG_SIZE - 1; i++) {
    bag[i] = bag[i + 1];
  }
  bag[BAG_SIZE - 1] = BlockId::Empty;

  if (count_nexts() <= NEXT_REFERS) {
    join_nexts(NEXT_REFERS);
  }

  return head;
}

shared_ptr<BlockId> Bag::get_mino_id(int index) {
  return bag[index];
}

vector<shared_ptr<BlockId>> Bag::generate_block_ids() {
  vector<shared_ptr<BlockId>> b = vector<shared_ptr<BlockId>>(TOTAL_MINOS);

  for (int i = 0; i < TOTAL_MINOS; i++) {
    b[i] = BlockId::from_id(i + 1);
  }
  return b;
}

vector<shared_ptr<BlockId>> Bag::generate_shuffled_nexts() {
  vector<shared_ptr<BlockId>> b = generate_block_ids();

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

int Bag::count_nexts() {
  int sum = 0;
  for (int i = 0; i < BAG_SIZE; i++) {
    if (bag[i] && bag[i] != BlockId::Empty)
      sum++;
  }
  return sum;
}

void Bag::join_nexts(int pivot) {
  vector<shared_ptr<BlockId>> newNexts = generate_shuffled_nexts();
  int end = min<int>(BAG_SIZE, pivot + static_cast<int>(newNexts.size()));
  for (int i = pivot; i < end; i++) {
    bag[i] = newNexts[i - pivot];
  }
}