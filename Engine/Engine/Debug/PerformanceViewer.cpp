#include "EnginePch.h"
#include "PerformanceViewer.h"

#include "Engine/Engine.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"

void TimingPlot::Push(float valueMs)
{
    displayedValue = valueMs;
    samples[offset] = valueMs;
    offset = (offset + 1) % SampleCount;
}

void TimingPlot::Render() const
{
    float maxVal = 0.0f;
    for (float s : samples)
        maxVal = std::max(maxVal, s);

    char overlay[32];
    (void)snprintf(overlay, sizeof(overlay), "%.2f ms", displayedValue);

    ImGui::Text("%s:", label);
    ImGui::PlotLines("##plot", samples, SampleCount, offset, overlay, 0.0f, std::max(maxVal * 1.2f, 33.3f), ImVec2(0, 60));
}

void PerformanceViewer::Tick()
{
#if !EDITOR
    if (Input::IsKeyDown(KeyCode::F4))
        myIsVisible = !myIsVisible;

    if (!myIsVisible)
        return;

    ImGui::Begin("PerformanceViewer");
    RenderImGui();
    ImGui::End();
#endif
}

void PerformanceViewer::RenderImGui()
{
    const float frameTime = Time::GetDeltaTime();

    mySampleAccumulator += frameTime;
    if (mySampleAccumulator >= ourSampleInterval)
    {
        mySampleAccumulator -= ourSampleInterval;
        myDisplayedFps = 1.0f / frameTime;
        myPlots[PlotType::FrameTime].Push(frameTime * 1000.0f);
        myPlots[PlotType::CPU].Push(Engine::GetCpuFrameTime() * 1000.0f);
    }

    ImGui::Text("FPS: %.0f", myDisplayedFps);
    for (const TimingPlot& plot : myPlots)
        plot.Render();
}