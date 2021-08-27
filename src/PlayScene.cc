#include "PlayScene.h"

#include "common.h"
#include "GlyphElementFactory.h"
#include "InputWindowElementFactory.h"
#include "MusicElementFactory.h"
#include "ResultScene.h"


gj::PlayScene::PlayScene(const Param& p, const std::string& title, const std::string& path) :
    param_(p), clock_(p.clock), store_(&clock_, 256) {

  GlyphElementFactory       glyph(p.alloc);
  InputWindowElementFactory inputWin(p.alloc, &sb_);
  MusicElementFactory       music(p.alloc, p.audio);

  sb_.title = ConvertStrToWstr(title);

  Lua::FactoryMap map = {
    { "Glyph",    &glyph },
    { "InputWin", &inputWin },
    { "Music",    &music },
  };
  lua_ = p.alloc->MakeUniq<Lua>(
    p.alloc, &store_, map, path);
}


gj::UniqPtr<gj::iScene> gj::PlayScene::Update(Frame& f) {
  if (store_.IsEmpty()) {
    return param_.alloc->MakeUniq<iScene, ResultScene>(param_, sb_);
  }

  store_.Update(f);
  return nullptr;
}