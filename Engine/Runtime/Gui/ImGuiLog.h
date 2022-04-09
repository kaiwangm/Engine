#pragma once

#include <imgui.h>

#include "Core.h"

namespace ImGui {
struct ExampleAppLog {
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int> LineOffsets;  // Index to lines offset. We maintain this with
                                // AddLog() calls.
    bool AutoScroll;            // Keep scrolling if already at the bottom.

    ExampleAppLog();

    void Clear();

    void AddLog(const char* logLine){
        int old_size = Buf.size();
        Buf.append(logLine);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n') LineOffsets.push_back(old_size + 1);
    }

    void Draw(const char* title, bool* p_open = NULL);
};

// Demonstrate creating a simple log window with basic filtering.
void ShowExampleAppLog(bool* p_open);
}  // namespace ImGui