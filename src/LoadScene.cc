#include "LoadScene.h"

#include "common.h"


gj::LoadScene::LoadScene(Param&& p) : 
    alloc_(p.super.alloc), clock_(p.super.clock), loading_(L"Loading...") {
  prod_   = p.super.alloc->MakeUniq<PlayScene>(p.super, p.title, p.path);
  orphan_ = std::move(p.orphan);
}

gj::UniqPtr<gj::iScene> gj::LoadScene::Update(Frame& frame) {
  if (prod_->HasPrepared() && !(orphan_ && orphan_->IsBusy())) {
    prod_->Start();
    return std::move(prod_);
  }

  const uint64_t now = clock_->now();

  if (XorShift(now+1)%10) {
    loading_.SetPosition((frame.w - loading_.width())/2, frame.h/2);
    frame.Add(&loading_);
  } 

  return nullptr;
}