#pragma once
#include <chrono>

enum class TimeSizeType
{
	NANOSECONDS,
	MICROSECONDS,
	MILLISECONDS,
	SECONDS,
	MINUTES,
	HOURS
};

class Timer final
{
	public:
		Timer();
		~Timer() = default;

		Timer(const Timer&) = delete;
		Timer(Timer&&) noexcept = delete;
		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) noexcept = delete;

		void Update();

		void StartCodeTimer()
		{
			m_CodeTimer_Start = std::chrono::high_resolution_clock::now();
		}
		long long EndCodeTimer(const TimeSizeType& timeSizeType)
		{
			std::chrono::steady_clock::time_point codeTimer_End = std::chrono::high_resolution_clock::now();
			//long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(codeTimer_End - m_CodeTimer_Start).count();
			long long duration{};
			if (timeSizeType == TimeSizeType::NANOSECONDS)
				duration = std::chrono::duration_cast<std::chrono::nanoseconds>(codeTimer_End - m_CodeTimer_Start).count();
			else if (timeSizeType == TimeSizeType::MICROSECONDS)
				duration = std::chrono::duration_cast<std::chrono::microseconds>(codeTimer_End - m_CodeTimer_Start).count();
			else if (timeSizeType == TimeSizeType::MILLISECONDS)
				duration = std::chrono::duration_cast<std::chrono::milliseconds>(codeTimer_End - m_CodeTimer_Start).count();
			else if (timeSizeType == TimeSizeType::SECONDS)
				duration = std::chrono::duration_cast<std::chrono::nanoseconds>(codeTimer_End - m_CodeTimer_Start).count();
			else if (timeSizeType == TimeSizeType::MINUTES)
				duration = std::chrono::duration_cast<std::chrono::minutes>(codeTimer_End - m_CodeTimer_Start).count();
			else 
				duration = std::chrono::duration_cast<std::chrono::hours>(codeTimer_End - m_CodeTimer_Start).count();

			m_CodeTimer_Start = {};

			return duration;
		}

		const float GetElapsedTime() const { return m_ElapsedTime; }
		const float GetMsEachUpdate() const { return m_MsEachUpdate; }
		const float GetLag() const { return m_Lag; }
		void SubtractFixedUpdateFromLag() { m_Lag -= m_MsEachUpdate; }

		void ResetElapsedTime() { m_ElapsedTime = 0; }

	protected:

	private:
		std::chrono::steady_clock::time_point m_CodeTimer_Start;
		std::chrono::steady_clock::time_point m_CurrentTime;
		std::chrono::steady_clock::time_point m_PreviousTime;
		float m_Lag;
		float m_ElapsedTime;
		float m_MsEachUpdate;
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
};