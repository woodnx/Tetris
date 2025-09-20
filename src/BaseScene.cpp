#include "BaseScene.h"

BaseScene::BaseScene(ISceneChanger* changer): scene_changer(changer) {}

void BaseScene::initialize() {}

void BaseScene::finalize() {}

void BaseScene::update() {
  _frame_count++;

  if (_frame_count >= frame_count_limit) {
    _frame_count = -1;
  }
}

int BaseScene::frame_count() {
  return _frame_count;
}

void BaseScene::reset_frame_count() {
  _frame_count = -1;
}