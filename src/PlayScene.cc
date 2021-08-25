#include "PlayScene.h"

#include "GlyphElementFactory.h"


gj::PlayScene::PlayScene(Param&& p) :
    alloc_(p.alloc), logger_(p.logger), w_(p.w), h_(p.h),
    clock_(p.clock), store_(&clock_, 256) {
  GlyphElementFactory glyph(alloc_);

  Lua::FactoryMap map;
  map["Glyph"] = &glyph;

  lua_ = alloc_->MakeUniq<Lua>(
    alloc_, &store_, map, "res/score/" + p.score + ".lua");

  logger_->Print(L"PlayScene init");
}