#include "PlayScene.h"

#include "common.h"
#include "GlyphElementFactory.h"
#include "InputWindowElementFactory.h"
#include "MusicElementFactory.h"
#include "ResultScene.h"


gj::UniqPtr<gj::iScene> gj::PlayScene::Update(Frame& f) {
  if (store_.IsEmpty()) {
    return param_.alloc->MakeUniq<ResultScene>(param_, sb_);
  }

  store_.Update(f, clock_.now());
  return nullptr;
}

gj::PlayScene::PlayScene(const Param& p, const std::string& title, const std::string& path) :
    param_(p), clock_(p.clock), store_(256) {
  sb_.title = ConvertStrToWstr(title);

  /* creates factory instances and pass them to the Lua.
   * actually these factories are used in only Lua's constructor,
   * so it's no problem that they're on stack   */
  GlyphElementFactory       glyph(p.alloc);
  InputWindowElementFactory inputWin(p.alloc, &sb_);
  MusicElementFactory       music(p.alloc, p.audio);

  Lua::FactoryMap map = {
    { "Glyph",    &glyph },
    { "InputWin", &inputWin },
    { "Music",    &music },
  };
  lua_ = p.alloc->MakeUniq<Lua>(p.alloc, &store_, map, path);
}

void gj::PlayScene::Start() {
  clock_ = OffsetClock(param_.clock);
}

bool gj::PlayScene::HasPrepared() const {
  return store_.CountPreparings() == 0;
}