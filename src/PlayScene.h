#pragma once


#include "ElementStore.h"
#include "Frame.h"
#include "iAllocator.h"
#include "iAudioDevice.h"
#include "iLogger.h"
#include "iScene.h"
#include "Lua.h"
#include "OffsetClock.h"
#include "Scoreboard.h"


namespace gj {


class PlayScene : public iScene {
 public:
  struct Param {
    iAllocator*   alloc;
    iAudioDevice* audio;
    const iClock* clock;

    std::string score;
  };

  PlayScene() = delete;
  PlayScene(PlayScene&&) = delete;
  PlayScene(const PlayScene&) = delete;

  PlayScene& operator=(PlayScene&&) = delete;
  PlayScene& operator=(const PlayScene&) = delete;

  PlayScene(Param&& p);

  UniqPtr<iScene> Update(Frame& f) override;

 private:
  iAllocator*   alloc_;
  iAudioDevice* audio_;

  OffsetClock clock_;

  Scoreboard   sb_;
  ElementStore store_;
  UniqPtr<Lua> lua_;
};


}