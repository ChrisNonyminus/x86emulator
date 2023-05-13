// Generated with ImRAD 0.1
// visit github.com/tpecholt/imrad

#pragma once
#include <string>
#include <functional>
#include <imgui.h>
#include "imrad.h"

class X86DebuggerWindow
{
public:
    /// @interface
    void Draw();

    size_t num_regs;

private:
    /// @impl
};

extern X86DebuggerWindow gX86DebuggerWindow;
