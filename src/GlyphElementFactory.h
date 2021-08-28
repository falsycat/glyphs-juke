#pragma once

#include "common.h"
#include "Font.h"
#include "iAllocator.h"
#include "iClock.h"
#include "iElementFactory.h"
#include "Texture.h"
#include "TextureElement.h"


namespace gj {

class GlyphElementFactory : public iElementFactory {
 public:
  GlyphElementFactory(GlyphElementFactory&&) = delete;
  GlyphElementFactory(const GlyphElementFactory&) = delete;

  GlyphElementFactory& operator=(GlyphElementFactory&&) = delete;
  GlyphElementFactory& operator=(const GlyphElementFactory&) = delete;

  GlyphElementFactory(iAllocator* alloc) : alloc_(alloc) {
  }

  UniqPtr<iElement> Create(Param&& param) override {
    if (param.custom.size() != 3) return nullptr;

    const std::string text = std::get<std::string>(param.custom[0]);
    const std::string name = std::get<std::string>(param.custom[1]);
    const uint32_t    size = static_cast<uint32_t>(std::get<double>(param.custom[2]));

    auto& font = FindOrCreateFont(name);
    auto  tex  = std::move(font.RenderGlyphs(ConvertStrToWstr(text), size));  /* TODO */

    return alloc_->MakeUniq<iElement, TextureElement>(
      param.period, std::move(tex), std::move(param.driver));
  }

 private:
  Font& FindOrCreateFont(const std::string& name) {
    auto found = fonts_.find(name);
    if (found != fonts_.end()) {
      return *found->second;
    }
    auto f   = alloc_->MakeUniq<Font>(alloc_, name);
    auto ptr = f.get();
    fonts_[name] = std::move(f);
    return *ptr;
  }

  iAllocator* alloc_;

  std::map<std::string, UniqPtr<Font>> fonts_;
};


}