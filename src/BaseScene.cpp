#include "BaseScene.h"

BaseScene::BaseScene(ISceneChanger* changer): mSceneChanger(changer) {}

void BaseScene::Initialize() {}

void BaseScene::Finalize() {}

void BaseScene::Update() {
  _frame_count++;

  if (_frame_count >= FRAME_COUNT_LIMIT) {
    _frame_count = -1;
  }
}

int BaseScene::frame_count() {
  return _frame_count;
}

void BaseScene::reset_frame_count() {
  _frame_count = -1;
}