#pragma once

#include <cmath>

#include "iElement.h"
#include "iElementDriver.h"
#include "Texture.h"

namespace gj {


class TextureElement : public iElement {
public:
  TextureElement() = delete;
  TextureElement(TextureElement&&) = delete;
  TextureElement(const TextureElement&) = delete;

  TextureElement& operator=(TextureElement&&) = delete;
  TextureElement& operator=(const TextureElement&) = delete;

  TextureElement(const Period& p, Texture&& tex, UniqPtr<iElementDriver>&& drv) :
      iElement(p), tex_(std::move(tex)), drv_(std::move(drv)) {
    param_["posX"]   = 0.;
    param_["posY"]   = 0.;
    param_["scaleX"] = 1.;
    param_["scaleY"] = 1.;
    param_["rota"]   = 0.;
    param_["alpha"]  = 1.;
  }

  void Update(Frame& frame, double t) override {
    drv_->Update(param_, t);

    const double posX   = std::get<double>(param_["posX"]);
    const double posY   = std::get<double>(param_["posY"]);
    const double scaleX = std::get<double>(param_["scaleX"]);
    const double scaleY = std::get<double>(param_["scaleY"]);
    const double rota   = std::get<double>(param_["rota"]);
    const double alpha  = std::get<double>(param_["alpha"]);

    const double c = std::cos(rota);
    const double s = std::sin(rota);
    
    auto Ms = mat3{
      { scaleX, 0, 0 },
      { 0, scaleY, 0 },
      { 0, 0, 1},
    };
    auto Mr = mat3{
      { c, -s, 0, },
      { s,  c, 0, },
      { 0,  0, 1, },
    };
    auto M = mat3{
      { 1, 0, 0 },
      { 0, 1, 0 },
      { posX, posY, 1},
    };
    M = ::linalg::mul(M, Mr);
    M = ::linalg::mul(M, Ms);

    tex_.SetMatrix(M);
    tex_.SetAlpha(static_cast<float>(alpha));

    frame.Add(&tex_);
  }

  void Finalize() override {
  }

  bool HasPrepared() const override {
    return true;
  }

 private:
  Texture tex_;
  UniqPtr<iElementDriver> drv_;

  iElementDriver::Param param_;
};


}