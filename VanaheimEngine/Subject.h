#pragma once
#include "Observer.h"
#include <vector>

class Subject
{
	public:
		Subject() = default;
		virtual ~Subject();

		Subject(const Subject&) = delete;
		Subject& operator=(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator=(Subject&&) = delete;

		void AddObserver(Observer* pObserver) { m_pObservers.push_back(pObserver); }
		void RemoveObserver(Observer* pObserver) { m_pObservers.erase(std::find(m_pObservers.begin(), m_pObservers.end(), pObserver)); }

	protected:
		void Notify(const ObserverEvent& event);

	private:
		std::vector<Observer*> m_pObservers;
};