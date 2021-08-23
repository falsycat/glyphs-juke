#pragma once

#include "thirdparty/linalg.h"

#include "Rasterbuffer.h"


namespace gj {


using mat3 = ::linalg::mat<double, 3, 3>;
using vec3 = ::linalg::vec<double, 3>;

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

 protected:
  mat3 mat_    = ::linalg::identity;
  mat3 invmat_ = ::linalg::identity;
};


}