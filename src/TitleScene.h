#pragma once

#include "common.h"
#include "GlitchPosteffect.h"
#include "iScene.h"
#include "Text.h"
#include "Texture.h"

namespace gj {


class TitleScene : public iScene {
 public:
  static constexpr auto kListPath = "res/list.json";

  TitleScene() = delete;
  TitleScene(TitleScene&&) = delete;
  TitleScene(const TitleScene&) = delete;

  TitleScene& operator=(TitleScene&&) = delete;
  TitleScene& operator=(const TitleScene&) = delete;

  TitleScene(const Param& p);

  UniqPtr<iScene> Update(Frame& frame) override;

 private:
  struct Score {
    std::string displayName;
    std::string score;
    std::string music;
    double      playOffset;
  };

  Param param_;

  Text score_;
  Text next_;
  Text prev_;
  Text guide_;

  Texture logo_;
  
  size_t select_index_;
  std::vector<Score> list_;

  GlitchPosteffect pe_;

  void SelectScore_(size_t index);
};


}