#pragma once

#include "common.h"
#include "Rasterbuffer.h"


namespace gj {


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

  void SetMatrix(const mat3& m) {
    SetMatrix(mat3(m));
  }
  void SetMatrix(mat3&& m) {
    mat_    = std::move(m);
    invmat_ = ::linalg::inverse(mat_);
  }

  void SetAlpha(float a) {
    alpha_ = a;
  }

 protected:
  mat3 mat_    = ::linalg::identity;
  mat3 invmat_ = ::linalg::identity;

  float alpha_ = 1;
};


}