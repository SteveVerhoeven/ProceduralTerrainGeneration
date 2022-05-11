#include "VanaheimPCH.h"
#include "Timer.h"

Timer::Timer()
	  : m_CurrentTime{}
	  , m_PreviousTime{}
	  , m_ElapsedTime{}
	  , m_MsEachUpdate{ MsPerFrame / 1000.f }
	  , m_Lag{}
{
	m_PreviousTime = std::chrono::high_resolution_clock::now();
}
void Timer::Update()
{
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	m_ElapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_CurrentTime - m_PreviousTime).count();
	m_PreviousTime = m_CurrentTime;
	m_Lag += m_ElapsedTime;
}