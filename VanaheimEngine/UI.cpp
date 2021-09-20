#include "pch.h"
#include "UI.h"

// Vanaheim Includes

UI::UI(const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size)
   : m_Name(windowName)
   , m_Position(position)
   , m_Size(size)
{}