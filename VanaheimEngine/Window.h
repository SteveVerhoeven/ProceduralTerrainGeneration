#pragma once
#include <string>

class Window final
{
	public:
		Window(const std::string& pName, HINSTANCE instance);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		const HWND& GetWindowHandle() const { return m_Window; }
		float GetAscpectRatio() const { return float(m_Width) / float(m_Height); }
		int GetWindowWidth() const { return m_Width; }
		int GetWindowHeight() const { return m_Height; }

	private:
		const char* m_pName;
		int m_Height;
		int m_Width;
		HINSTANCE m_Instance;
		HWND m_Window;

		void CreateClass();
		void CreateShowWindow();

		static LRESULT CALLBACK WindowProcedureStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		void GetWidthHeight();
};