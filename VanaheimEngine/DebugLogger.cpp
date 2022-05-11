#include "VanaheimPCH.h"
#include "DebugLogger.h"

#include <string>
#if defined(DEBUG) || defined(_DEBUG)
#include <dxerr.h>
#endif

void DebugLogger::Initialize()
{
	//CreateConsoleWindow();
}

void DebugLogger::Log(const ErrorLevel& errorLevel, const std::string& message)
{
	const std::string outputMessage{ OutputMessageToConsole(errorLevel, message) };
	OutputMessageBox(errorLevel, outputMessage);

	Locator::GetUIManagerService()->GetUI<ConsoleUI>()->AddLog(message.c_str());
}
void DebugLogger::LogHRESULT(HRESULT hr, const std::string& functionName, const std::string& file, const std::string& line)
{
	if (!FAILED(hr))
		return;

	std::string outputMessage{};
	outputMessage += ">Error name< " + GetErrorName(hr) + '\n';
	outputMessage += ">Error Description< " + GetErrorDescription(hr) + '\n';
	outputMessage += ">File< " + GetErrorFile(file) + '\n';
	outputMessage += ">Line< " + GetErrorLine(line) + '\n';
	outputMessage += ">Function< " + functionName + '\n';

	OutputMessageBox_HRESULT(outputMessage.c_str());
}

void DebugLogger::CreateConsoleWindow()
{
	// ------------------------------------
	// Code from: Overlord Engine 
	// During Graphics Programming 2 - DAE
	// Author: Matthieu Delaere
	// ------------------------------------
	if (AllocConsole())
	{
		// Redirect the CRT standard input, output, and error handles to the console
		FILE* pCout;
		freopen_s(&pCout, "CONIN$", "r", stdin);
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);
		//Clear the error state for each of the C++ standard stream objects. We need to do this, as
		//attempts to access the standard streams before they refer to a valid target will cause the
		//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
		//to always occur during startup regardless of whether anything has been read from or written to
		//the console or not.
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();
		std::cin.clear();

		// Get Console Handle
		m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		//Disable Close-Button
		HWND hwnd = GetConsoleWindow();
		if (hwnd != NULL)
		{
			HMENU hMenu = GetSystemMenu(hwnd, FALSE);
			if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
}

std::string DebugLogger::OutputMessageToConsole(const ErrorLevel& errorLevel, const std::string& message)
{
	std::string outputMessage{};

	// *************************************************************
	// Setup the start of the error logging output && 
	// Set console text attributes 
	// *************************************************************
	/* SetConsoleTextAttribute - Parameters */
	HANDLE hConsoleOutput{ m_ConsoleHandle };
	WORD   wAttributes{};
	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
	switch (errorLevel)
	{
		case ErrorLevel::LOG_INFO:		// White
			outputMessage += ">LOG INFO< ";

			wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			SetConsoleTextAttribute(hConsoleOutput, wAttributes);
			break;
		case ErrorLevel::LOG_WARNING:	// Yellow
			outputMessage += ">LOG WARNING< ";

			wAttributes = FOREGROUND_RED | FOREGROUND_GREEN;
			SetConsoleTextAttribute(hConsoleOutput, wAttributes);
			break;
		case ErrorLevel::LOG_ERROR:		// Red
			outputMessage += ">LOG ERROR< ";

			wAttributes = FOREGROUND_RED;
			SetConsoleTextAttribute(hConsoleOutput, wAttributes);
			break;
		case ErrorLevel::LOG_FATAL:		// Blue_ish
			outputMessage += ">LOG FATAL< ";

			wAttributes = FOREGROUND_GREEN | FOREGROUND_BLUE;
			SetConsoleTextAttribute(hConsoleOutput, wAttributes);
			break;
	}

	// Merge the received message with the output message
	outputMessage += '\t';
	outputMessage += message;

	std::cout << outputMessage << '\n';

	return outputMessage;
}
void DebugLogger::OutputMessageBox(const ErrorLevel& errorLevel, const std::string& outputMessage)
{
	// *************************************************************
	// If the message is an ERROR or a FATAL then output a message box
	// *************************************************************
	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
	if (errorLevel == ErrorLevel::LOG_ERROR)
	{
		/* MessageBoxA - Parameters */
		HWND   hWnd{ NULL };
		LPCSTR lpText{ outputMessage.c_str() };
		LPCSTR lpCaption{ "!!!Error found!!!" };
		UINT   uType{ MB_ICONEXCLAMATION };

		MessageBox(hWnd, lpText, lpCaption, uType);
	}
	else if (errorLevel == ErrorLevel::LOG_FATAL)
	{
		/* MessageBoxA - Parameters */
		HWND   hWnd{ NULL };
		LPCSTR lpText{ outputMessage.c_str() };
		LPCSTR lpCaption{ "!!!FATAL found!!!" };
		UINT   uType{ MB_ICONERROR };

		MessageBox(hWnd, lpText, lpCaption, uType);
	}
}
void DebugLogger::OutputMessageBox_HRESULT(LPCSTR errMsg)
{
	// *************************************************************
	// If the message is an ERROR or a FATAL then output a message box
	// *************************************************************
	/* MessageBoxA - Parameters */
	HWND   hWnd{ NULL };
	LPCSTR lpText{ errMsg };
	LPCSTR lpCaption{ "HRESULT error" };
	UINT   uType{ MB_ICONERROR };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
	MessageBox(hWnd, lpText, lpCaption, uType);
}

std::string DebugLogger::GetErrorName(HRESULT hr)
{ 
	#if defined(DEBUG) || defined(_DEBUG)
	return DXGetErrorString(hr);
	#else
	UNREFERENCED_PARAMETER(hr);
	return std::string();
	#endif	
}
std::string DebugLogger::GetErrorDescription(HRESULT hr)
{
	#if defined(DEBUG) || defined(_DEBUG)
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return std::string(buf);
	#else
	UNREFERENCED_PARAMETER(hr);
	return std::string();
	#endif
}
std::string DebugLogger::GetErrorFile(const std::string& file)
{
	std::string newFile{ file };

	// Extract the file name out of the entire path
	const std::string delimiter = "\\";

	size_t pos = 0;
	while ((pos = newFile.find(delimiter)) != std::string::npos)
		newFile.erase(0, ++pos);

	return newFile;
}
std::string DebugLogger::GetErrorLine(const std::string& line)
{ return line; }