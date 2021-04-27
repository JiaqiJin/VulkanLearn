#include "Timer.h"

namespace Rendering
{
    inline float getDeltaTime(Timer::TimePointT startTime, Timer::TimePointT stopTime)
    {
        return std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }

    Timer::Timer()
    {
        start();
    }

    void Timer::start()
    {
        m_startTime = ClockT::now();
    }

    float Timer::loop()
    {
        TimePointT loopTime = ClockT::now();
        float dt = getDeltaTime(m_startTime, loopTime);
        m_startTime = loopTime;
        return dt;
    }

    float Timer::getTime() const
    {
        TimePointT currentTime = ClockT::now();
        return getDeltaTime(m_startTime, currentTime);
    }
}