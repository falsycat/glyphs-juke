#include "TitleScene.h"

#include <cinttypes>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "thirdparty/picojson.h"

#include "common.h"
#include "Font.h"
#include "PlayScene.h"


gj::TitleScene::TitleScene(const Param& p) :
    param_(p),
    score_(L"penguin: you didn't see anything..."),
    next_(L"> L"), prev_(L"H <"),
    guide_(L"H:PREV / SPACE:PLAY / L:NEXT"),
    logo_(Colorbuffer(p.alloc, 1, 1)) {
  /* load score list */
  std::ifstream ifs(kListPath);
  if (!ifs) Abort(std::string(kListPath)+" is missing");

  ::picojson::value root;
  const std::string err = ::picojson::parse(root, ifs);
  ifs.close();

  if (err.size()) Abort(std::string(kListPath)+": "+err);

  std::string line;
  auto& list = root.get<::picojson::array>();
  for (auto& e : list) {
    auto& obj = e.get<::picojson::object>();

    Score s;

    s.displayName = obj["displayName"].get<std::string>();
    s.music       = obj["music"].get<std::string>();
    s.score       = obj["score"].get<std::string>();
    s.playOffset  = obj["playOffset"].get<double>();

    list_.push_back(s);
  }
  if (list_.size() == 0) Abort("no score is registered");

  SelectScore_(0);

  /* render logo */
  Font font(param_.alloc, "res/font/shippori.ttf");
  logo_ = Texture(font.RenderGlyphs(L"GlyphsJuke", 64));
}

gj::UniqPtr<gj::iScene> gj::TitleScene::Update(Frame& frame) {
  /* input handling */
  for (const auto c : frame.input) {
    switch (c) {
    case 'h':
      SelectScore_(select_index_? select_index_-1: list_.size()-1);
      break;
    case 'l':
      SelectScore_((select_index_+1)%list_.size());
      break;
    case ' ':
      if (music_) param_.audio->RemoveEffect(music_.get());
      return param_.alloc->MakeUniq<iScene, PlayScene>(
        param_, list_[select_index_].displayName, list_[select_index_].score);
    }
  }

  /* play music */
  if (trying_play_ && !(music_ && music_->IsBusy())) {
    if (music_) {
      param_.audio->RemoveEffect(music_.get());
    }
    auto au = param_.audio;

    const auto& s = list_[select_index_];
    music_ = param_.alloc->MakeUniq<Music>(s.music, au->ch(), au->sampleRate());
    music_->Seek(s.playOffset);
    music_->SetVolume(.2f);
    music_->SetLpf(.99f);
    music_->Play();

    au->AddEffect(music_.get());
    trying_play_ = false;
  }

  /* graphics calculation */
  const uint64_t now = param_.clock->now();

  const int32_t w = static_cast<int32_t>(frame.w);
  const int32_t h = static_cast<int32_t>(frame.h);

  const int32_t selector_y = static_cast<int32_t>(h*.75);

  const uint64_t period1 = XorShift(now/1000+1)%1000 + 500;
  const uint64_t period2 = XorShift(now/1000+5)%1000 + 500;
  logo_.SetAlpha(static_cast<float>(XorShift(now/period1+1)%100/100.*.4+.4));

  const double shift_x = (XorShift(now/period1+1)%100/100.*2 - 1)*.05;
  const double shift_y = (XorShift(now/period2+3)%100/100.*2 - 1)*.05;
  const double scale_x = (XorShift(now/period1+3)%100/100.*2 - 1)*.1;
  
  const double theta = (XorShift(now/period2+13)%100/100.*2 - 1)*.01*kPi*2;

  const double c = cos(theta), s = sin(theta);
  auto M = mat3{
    {1+scale_x, 0, 0},
    {0, .8, 0},
    {shift_x, shift_y+.4, 1}
  };
  auto Mr = mat3{
    {c, -s, 0},
    {s, c, 0},
    {0, 0, 1},
  };
  M = ::linalg::mul(Mr, M);
  logo_.SetMatrix(M);
  frame.Add(&logo_);

  next_.SetPosition(static_cast<int32_t>(w*.8-next_.width()), selector_y);
  frame.Add(&next_);

  prev_.SetPosition(static_cast<int32_t>(w*.2), selector_y);
  frame.Add(&prev_);
  
  score_.SetPosition((w-score_.width())/2, selector_y);
  frame.Add(&score_);

  guide_.SetPosition((w-guide_.width())/2, selector_y+3);
  frame.Add(&guide_);

  pe_.seed     = XorShift(now/period1+10);
  pe_.maxShift = (XorShift(now/period1+7)%100/100.*2 - 1)*.03;
  frame.Add(&pe_);

  return nullptr;
}

void gj::TitleScene::SelectScore_(size_t index) {
  const auto& s = list_[index];
  score_        = Text(ConvertStrToWstr(s.displayName));
  select_index_ = index;
  
  trying_play_ = true;
}