#pragma once

struct TimingPlot
{
    static constexpr int SampleCount = 128;

    const char* label = "";
    float samples[SampleCount] = {};
    int offset = 0;
    float displayedValue = 0.0f;

    void Push(float valueMs);
    void Render() const;
};

enum PlotType
{
    FrameTime = 0,
    CPU,
    GPU,
    Count
};

class PerformanceViewer
{
public:
    static void Tick();
    static void RenderImGui();

private:
    static constexpr float ourSampleInterval = 1.0f / 40.0f;

    inline static bool myIsVisible = false;
    inline static float mySampleAccumulator = 0.0f;
    inline static float myDisplayedFps = 0.0f;

    inline static TimingPlot myPlots[] = {
        { "FrameTime" },
        { "CPU" },
        { "GPU" },
    };
};