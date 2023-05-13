// Generated with ImRAD 0.1
// visit github.com/tpecholt/imrad

#include "X86DebuggerWindow.h"

X86DebuggerWindow gX86DebuggerWindow;


void X86DebuggerWindow::Draw()
{
    /// @begin TopWindow
    ImGui::SetNextWindowSize({ 640, 720 }, ImGuiCond_Appearing);
    bool tmpOpen = true;
    ImGui::Begin("X86 Debugger", &tmpOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
    {
        /// @separator

        /// @begin Text
        ImRad::AlignedText(ImRad::Align_Left, "REGISTERS");
        /// @end Text

        /// @begin Table
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 1, 1 });
        if (ImGui::BeginTable("table94363122515952", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV, { 0, 0 }))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None, 0);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_None, 0);
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            /// @separator

            /// @begin Text
            ImRad::AlignedText(ImRad::Align_Left, "CS");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "ES");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "SS");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "DS");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "EIP");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "EAX");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "EBX");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "ECX");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "EDX");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "ESP");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "EBP");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "ESI");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "EDI");
            /// @end Text

            /// @begin Text
            ImGui::TableNextColumn();
            ImRad::AlignedText(ImRad::Align_Left, "[insert value here]");
            /// @end Text


            /// @separator
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
        /// @end Table

        /// @separator
        ImGui::End();
    }
    /// @end TopWindow
}
