#pragma once

#include <chrono>

class Timer
{
public:
    Timer() : m_startTime(std::chrono::high_resolution_clock::now()) {}

    // Update the time and calculate delta time
    void Update()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        m_deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
        m_lastTime = currentTime;
    }

    // Get the time elapsed since the last update
    float GetDeltaTime() const
    {
        return m_deltaTime;
    }

    // Get the total time elapsed since the Time object was created
    float GetTotalTime() const
    {
        return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_startTime).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;
    float m_deltaTime=0.0f;
};