#pragma once

#include <string>

#include "ElementStore.h"
#include "iScene.h"
#include "Lua.h"
#include "OffsetClock.h"
#include "Scoreboard.h"


namespace gj {


class PlayScene : public iScene {
 public:
  friend class LoadScene;

  PlayScene() = delete;
  PlayScene(PlayScene&&) = delete;
  PlayScene(const PlayScene&) = delete;

  PlayScene& operator=(PlayScene&&) = delete;
  PlayScene& operator=(const PlayScene&) = delete;

  PlayScene(const Param& p, const std::string& title, const std::string& path);
  
  UniqPtr<iScene> Update(Frame& f) override;

 private:
  void Start();
  bool HasPrepared() const;

  Param param_;

  OffsetClock clock_;

  Scoreboard   sb_;
  ElementStore store_;
  UniqPtr<Lua> lua_;
};


}