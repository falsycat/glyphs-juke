#pragma once

#include "Frame.h"
#include "iAllocator.h"
#include "iLogger.h"
#include "iScene.h"
#include "Lua.h"
#include "OffsetClock.h"


namespace gj {


class PlayScene : public iScene {
 public:
  struct Param {
    iAllocator*   alloc;
    iLogger*      logger;
    const iClock* clock;

    uint32_t w, h;

    std::string score;
  };

  PlayScene() = delete;
  PlayScene(PlayScene&&) = delete;
  PlayScene(const PlayScene&) = delete;

  PlayScene& operator=(PlayScene&&) = delete;
  PlayScene& operator=(const PlayScene&) = delete;

  PlayScene(Param&& p);

  UniqPtr<iScene> Update(Frame& f) override {
    return nullptr;
  }

 private:
  iAllocator* alloc_;
  iLogger*    logger_;

  uint32_t w_, h_;

  OffsetClock clock_;

  UniqPtr<Lua> lua_;
};


}