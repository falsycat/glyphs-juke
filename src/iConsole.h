#pragma once

#include "Rasterbuffer.h"


namespace gj {


class iConsole {
 public:
  iConsole(iConsole&&) = delete;
  iConsole(const iConsole&) = delete;

  iConsole& operator=(iConsole&&) = delete;
  iConsole& operator=(const iConsole&) = delete;

  iConsole() = default;
  virtual ~iConsole() = default;

  virtual void Show() = 0;
  virtual void Hide() = 0;

  virtual Colorbuffer& TakeColorbuffer() = 0;
  virtual void SwapColorbuffer() = 0;

  virtual Textbuffer& TakeTextbuffer() = 0;
  virtual void SwapTextbuffer() = 0;

  virtual uint32_t width()  const = 0;
  virtual uint32_t height() const = 0;
};


}