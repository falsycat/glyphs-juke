#include "ResultScene.h"

#include "TitleScene.h"


gj::ResultScene::ResultScene(const Param& p, const Scoreboard& sb) :
      param_(p), clock_(p.clock), sb_(sb),
      title_(sb.title),
      correct_label_(L"CORRECT TYPES"),
      correct_num_(std::to_wstring(sb.correct)),
      correct_den_(std::to_wstring(sb.input)),
      line_label_(L"COMPLETE LINES"),
      line_num_(std::to_wstring(sb.completeLines)),
      line_den_(std::to_wstring(sb.lines)),
      guide_(L"~ PRESS SPACE ~") {
}

gj::UniqPtr<gj::iScene> gj::ResultScene::Update(Frame& f) {
  const int32_t w = static_cast<int32_t>(f.w);
  const int32_t h = static_cast<int32_t>(f.h);

  const int32_t title_y   = static_cast<int32_t>(h * .2);
  const int32_t correct_y = static_cast<int32_t>(h * .35);
  const int32_t line_y    = static_cast<int32_t>(h * .45);

  title_.SetPosition((w-title_.width())/2, title_y);
  f.Add(&title_);

  correct_label_.SetPosition(
    static_cast<int32_t>(w*.2), correct_y);
  f.Add(&correct_label_);

  correct_num_.SetPosition(
    static_cast<int32_t>(w*.7 - correct_num_.width()), correct_y);
  f.Add(&correct_num_);

  correct_den_.SetPosition(
    static_cast<int32_t>(w*.8 - correct_den_.width()), correct_y);
  f.Add(&correct_den_);

  line_label_.SetPosition(
    static_cast<int32_t>(w*.2), line_y);
  f.Add(&line_label_);

  line_num_.SetPosition(
    static_cast<int32_t>(w*.7 - line_num_.width()), line_y);
  f.Add(&line_num_);

  line_den_.SetPosition(
    static_cast<int32_t>(w*.8 - line_den_.width()), line_y);
  f.Add(&line_den_);

  guide_.SetPosition((w-guide_.width())/2, static_cast<int32_t>(h*.8));
  f.Add(&guide_);

  if (f.input.find(' ') != std::string::npos) {
    return param_.alloc->MakeUniq<TitleScene>(param_);
  }

  return nullptr;
}