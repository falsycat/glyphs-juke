#pragma once

#include <algorithm>

#include "thirdparty/linalg.h"

#include "iDrawable.h"
#include "Rasterbuffer.h"


namespace gj {


class Texture : public DrawableBase {
 public:
  Texture() = delete;
  Texture(Texture&&) = default;
  Texture(const Texture&) = default;

  Texture& operator=(Texture&&) = default;
  Texture& operator=(const Texture&) = default;

  Texture(Colorbuffer&& src) : src_(std::move(src)) {
  }

  void Draw(Colorbuffer& fb) const override;

  void SetSource(Colorbuffer&& src) {
    src_ = std::move(src);
  }

 private:
  Colorbuffer src_;
};


}