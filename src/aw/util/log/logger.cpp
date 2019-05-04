#include <aw/util/log/logger.hpp>

#include <type_traits>

namespace aw::log
{
Logger::Logger(unsigned queueSize) : mBufferCount(queueSize) {}

Logger::Logger(unsigned queueSize, std::chrono::microseconds flushInterval) :
    mBufferCount(queueSize),
    mFlushInterval(flushInterval)
{
}

Logger::~Logger()
{
  mRunning = false;
  if (mSinkThread.joinable())
    mSinkThread.join();
}

void Logger::enableLevel(Level level)
{
  using T = std::underlying_type_t<Level>;
  mLevelEnabled |= 1 << static_cast<T>(level);
}

void Logger::disableLevel(Level level)
{
  using T = std::underlying_type_t<Level>;
  mLevelEnabled &= ~(1 << static_cast<T>(level));
}

void Logger::addSink(std::shared_ptr<log::Sink> sink)
{
  mSinks.push_back(sink);
}

void Logger::sinkThread()
{
  do
  {
    while (!mBufferController.empty())
    {
      auto loc = mBufferController.reader();
      // Now print
      for (auto& sink : mSinks)
        sink->log(mMessageBuffer[loc]);

      mMessageBuffer[loc].message.clear();
      mBufferController.increaseReader();
    }
    std::this_thread::sleep_for(mFlushInterval);

  } while (!mBufferController.empty() || mRunning);

  if (!mBufferController.empty())
    fmt::print("Lost log messages :(\n");
}
} // namespace aw::log
