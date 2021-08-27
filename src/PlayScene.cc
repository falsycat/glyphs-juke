#include "PlayScene.h"

#include "common.h"
#include "GlyphElementFactory.h"
#include "InputWindowElementFactory.h"
#include "MusicElementFactory.h"
#include "ResultScene.h"


gj::PlayScene::PlayScene(Param&& p) :
    alloc_(p.alloc), audio_(p.audio),
    clock_(p.clock), store_(&clock_, 256) {

  GlyphElementFactory       glyph(alloc_);
  InputWindowElementFactory inputWin(alloc_, &sb_);
  MusicElementFactory       music(alloc_, audio_);

  sb_.title = ConvertStrToWstr(p.score);

  Lua::FactoryMap map = {
    { "Glyph",    &glyph },
    { "InputWin", &inputWin },
    { "Music",    &music },
  };
  lua_ = alloc_->MakeUniq<Lua>(
    alloc_, &store_, map, "res/score/" + p.score + ".lua");
}


gj::UniqPtr<gj::iScene> gj::PlayScene::Update(Frame& f) {
  if (store_.IsEmpty()) {
    return alloc_->MakeUniq<iScene, ResultScene>(alloc_, clock_.parent(), sb_);
  }

  store_.Update(f);
  return nullptr;
}