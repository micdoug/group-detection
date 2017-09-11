#include "UnifiedGroup.h"
#include "extensions.h"
#include <stdexcept>


using namespace std;

UnifiedGroup::UnifiedGroup(set<int> members)
{
    m_members = members;
}

void UnifiedGroup::addMember(const int member)
{
    m_members.insert(member);
}

void UnifiedGroup::removeMember(const int member)
{
    m_members.erase(member);
}

void UnifiedGroup::addEncounter(long start, long end)
{
    m_encounters.push_back(Encounter(start, end));
}

const set<int> &UnifiedGroup::members() const
{
    return m_members;
}

const vector<Encounter> &UnifiedGroup::encounters() const
{
    return m_encounters;
}

bool UnifiedGroup::hasIntersection(long start, long end) const 
{
    Encounter enc(start, end);
    for(const auto &e: m_encounters)
    {
        if (e.hasIntersection(enc))
        {
            return true;
        }
    }
    return false;
}