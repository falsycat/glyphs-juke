#include "Game.h"
#include "PlayScene.h"


gj::Game::Game(gj::Game::Param&& p) :
    alloc_(p.alloc),
    clock_(p.clock),
    logger_(p.h),
    w_(p.w), h_(p.h),
    frame_(p.w, p.h, kReserveDrawable, kReserveWritable) {
  gj::PlayScene::Param param;
  param.alloc = alloc_;
  param.clock = &clock_;
  param.audio = p.audio;
  param.score = "test";  /* TODO test */
  scene_ = alloc_->MakeUniq<gj::iScene, gj::PlayScene>(std::move(param));
}