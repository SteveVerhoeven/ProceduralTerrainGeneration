#include "VanaheimPCH.h"
#include "Subject.h"

Subject::~Subject()
{
    m_pObservers.clear();
}

void Subject::Notify(const ObserverEvent& event)
{
    const size_t nbrOfObservers{ m_pObservers.size() };
    for (size_t i{}; i < nbrOfObservers; ++i)
    {
        m_pObservers[i]->onNotify(event);
    }
}