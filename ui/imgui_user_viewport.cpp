// Generated with ImRAD 0.1
// visit github.com/tpecholt/imrad

#include "imgui_user_viewport.h"

#include "imgui_memory_editor.h"
#include <cstddef>

VGAOutputWindow gVGAOutputWindow;


void VGAOutputWindow::Draw()
{
    /// @begin TopWindow
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
    ImGui::SetNextWindowSize({ 640, 512 }, ImGuiCond_Appearing);
    bool tmpOpen = true;
    ImGui::Begin("VGA Output", &tmpOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    {
        /// @separator

        /// @begin Child
        ImGui::BeginChild("child94363081101824", { 640, 480 }, false);
        {
            /// @separator

            /// @separator
            ImGui::EndChild();
        }
        /// @end Child

        /// @separator
        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    /// @end TopWindow
}
