#include "PlayScene.h"


gj::PlayScene::PlayScene(gj::PlayScene::Param&& p) :
    alloc_(p.alloc), logger_(p.logger), w_(p.w), h_(p.h),
    clock_(p.clock) {
  lua_ = alloc_->MakeUniq<Lua>(alloc_, Lua::FactoryMap(), "res/score/"+p.score+".lua");

  logger_->Print(L"PlayScene init");
}