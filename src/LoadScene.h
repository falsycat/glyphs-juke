#pragma once

#include <string>

#include "iAllocator.h"
#include "iClock.h"
#include "iScene.h"
#include "Music.h"
#include "PlayScene.h"
#include "Text.h"

namespace gj {


class LoadScene : public iScene {
 public:
  struct Param {
    iScene::Param super;

    std::string title;
    std::string path;
    
  /*  'orphan' is an instance of Music that couldn't be deleted
   * at previous scene because it was busy. */
    UniqPtr<Music> orphan;
  };

  LoadScene() = delete;
  LoadScene(LoadScene&&) = delete;
  LoadScene(const LoadScene&) = delete;

  LoadScene& operator=(LoadScene&&) = delete;
  LoadScene& operator=(const LoadScene&) = delete;

  LoadScene(Param&& p);

  UniqPtr<iScene> Update(Frame& frame) override;

 private:
  iAllocator* alloc_;
  const iClock* clock_;

  UniqPtr<PlayScene> prod_;
  UniqPtr<Music>     orphan_;

  Text loading_;
};


}