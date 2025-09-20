#pragma once
#include "ISceneChanger.h"
#include "Task.h"

// シーンの基底クラス。
class BaseScene : public Task {
  private:
    int _frame_count = -1;

    const int FRAME_COUNT_LIMIT = 60 * 10 * 10;

  protected:
    ISceneChanger*
      mSceneChanger; // クラス所有元にシーン切り替えを伝えるインターフェイス

  public:
    BaseScene(ISceneChanger* changer);
    virtual ~BaseScene() {}
    virtual void Initialize() override; // 初期化処理をオーバーライド。
    virtual void Finalize() override;   // 終了処理をオーバーライド。
    virtual void Update() override;     // 更新処理をオーバーライド。
    virtual void Draw() override = 0; // 描画処理は純粋仮想のまま、派生で必須。

    int frame_count();
    void reset_frame_count();
};
