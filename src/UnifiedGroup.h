#ifndef UNIFIED_GROUP_H
#define UNIFIED_GROUP_H

#include <set>
#include <vector>
#include "Encounter.h"

class UnifiedGroup final
{
public:
    UnifiedGroup(std::set<int> members = {});
    ~UnifiedGroup() = default;
    void addMember(const int member);
    void removeMember(const int member);
    void addEncounter(long start, long end);
    const std::set<int> &members() const;
    const std::vector<Encounter> &encounters() const;
    bool hasIntersection(long start, long end) const;
private:
    std::set<int> m_members;
    std::vector<Encounter> m_encounters;
};

#endif