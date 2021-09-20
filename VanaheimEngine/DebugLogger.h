#pragma once

enum class ErrorLevel
{
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL
};
class DebugLogger final
{
	public:		
		DebugLogger() = default;
		~DebugLogger() = default;

		DebugLogger(const DebugLogger&) = delete;
		DebugLogger& operator=(const DebugLogger&) = delete;
		DebugLogger(DebugLogger&&) = delete;
		DebugLogger& operator=(DebugLogger&&) = delete;

		void Initialize();

		// Logging manual errors
		void Log(const ErrorLevel& errorLevel, const std::string& message);
		void LogHRESULT(HRESULT hr, const std::string& functionName, const std::string& file, const std::string& line);

	protected:
	private:
		HANDLE m_ConsoleHandle;

		void CreateConsoleWindow();

		std::string OutputMessageToConsole(const ErrorLevel& errorLevel, const std::string& message);
		void OutputMessageBox(const ErrorLevel& errorLevel, const std::string& outputMessage);
		void OutputMessageBox_HRESULT(LPCSTR errMsg);

		std::string GetErrorName(HRESULT hr);
		std::string GetErrorDescription(HRESULT hr);
		std::string GetErrorFile(const std::string& file);
		std::string GetErrorLine(const std::string& line);
};