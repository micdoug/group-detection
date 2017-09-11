#include "Encounter.h"
#include <stdexcept>

using namespace std;

Encounter::Encounter(long start, long end)
{
    m_start = start;
    m_end = end;
    if (m_start < 0 || m_end < 0)
    {
        throw std::invalid_argument("The start and end times of the encounter must be an integer greater or equal to zero.");
    }
    if (m_end <= m_start)
    {
        throw std::invalid_argument("The end time must be greater than the start time of the encounter.");
    }
}

long Encounter::duration() const
{
    return m_end - m_start;
}

long Encounter::start() const
{
    return m_start;
}

long Encounter::end() const
{
    return m_end;
}

bool Encounter::hasIntersection(const Encounter &other) const
{
    return !(other.end() < start() || other.start() > end());
}