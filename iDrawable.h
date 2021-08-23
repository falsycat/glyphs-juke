#pragma once

#include "thirdparty/linalg.h"

#include "Rasterbuffer.h"


namespace gj {


using mat4 = ::linalg::mat<double, 4, 4>;
using vec4 = ::linalg::vec<double, 4>;

class iDrawable {
 public:
  iDrawable(iDrawable&&) = default;
  iDrawable(const iDrawable&) = default;

  iDrawable& operator=(iDrawable&&) = default;
  iDrawable& operator=(const iDrawable&) = default;

  iDrawable() = default;
  virtual ~iDrawable() = default;

  virtual void Draw(Colorbuffer& buf) const = 0;
};

class DrawableBase : public iDrawable {
 public:
  DrawableBase() = default;
  ~DrawableBase() = default;

  DrawableBase(DrawableBase&&) = default;
  DrawableBase(const DrawableBase&) = default;

  DrawableBase& operator=(DrawableBase&&) = default;
  DrawableBase& operator=(const DrawableBase&) = default;

  void setMatrix(const mat4& m) {
    setMatrix(mat4(m));
  }
  void setMatrix(mat4&& m) {
    mat_ = std::move(m);
  }

 protected:
  mat4 mat_ = ::linalg::identity;
};


}