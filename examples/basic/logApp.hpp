#pragma once

#include <aw/util/log/log.hpp>
#include <aw/util/log/module.hpp>

namespace aw::log
{
inline Module engineLogModule("App");
}

#define LOG_APP(level, ...)                                                                        \
  aw::log::getDefaultLogger()->log(aw::log::engineLogModule, __FILE__, __LINE__, __func__, level,  \
                                   __VA_ARGS__);