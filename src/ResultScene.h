#pragma once

#include "iScene.h"
#include "OffsetClock.h"
#include "Scoreboard.h"
#include "Text.h"


namespace gj {


class ResultScene : public iScene {
 public:
  ResultScene() = delete;
  ResultScene(ResultScene&&) = delete;
  ResultScene(const ResultScene&) = delete;

  ResultScene& operator=(ResultScene&&) = delete;
  ResultScene& operator=(const ResultScene&) = delete;

  ResultScene(const Param& p, const Scoreboard& sb);

  UniqPtr<iScene> Update(Frame& f) override;

 private:
  Param param_;

  OffsetClock clock_;
  Scoreboard  sb_;

  Text title_;

  Text correct_label_;
  Text correct_num_;
  Text correct_den_;

  Text line_label_;
  Text line_num_;
  Text line_den_;

  Text guide_;
};


}