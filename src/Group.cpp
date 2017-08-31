#include "Group.h"
#include "extensions.h"
#include <stdexcept>
#include <string>
#include <sstream>

GroupMember &Group::operator[](int key)
{
    return *m_members[key];
}

Group::Group():
    m_created{-1L},
    m_destroyed{-1L}
{

}

std::set<int> Group::memberKeys() const
{
    return ext::getKeys(m_members);
}

long Group::created() const
{
    return m_created;
}

void Group::setCreated(const long time)
{
    m_created = time;
}

long Group::destroyed() const 
{
    return m_destroyed;
}

void Group::setDestroyed(const long time)
{
    m_destroyed = time;
}

void Group::addMember(const int id)
{
    if (ext::containsKey(m_members, id))
    {
        throw std::invalid_argument("There is already a member with the id " + std::to_string(id));
    }
    m_members.insert(make_pair(id, std::make_unique<GroupMember>(id)));
}

void Group::removeMember(const int id)
{
    if (!ext::containsKey(m_members, id))
    {
        throw std::invalid_argument("There isn't a member with the id " + std::to_string(id) + " to be removed.");
    }
    m_members.erase(id);
}

int Group::size() const
{
    return m_members.size();
}

std::string Group::to_string() const
{
    std::ostringstream sbuilder;
    sbuilder << "{ ";
    for(const auto &key: memberKeys())
    {
        sbuilder << key << " ";
    }
    sbuilder << "}";
    return sbuilder.str();
}

std::map<int, std::unique_ptr<GroupMember>>::iterator Group::begin()
{
    return m_members.begin();
}

std::map<int, std::unique_ptr<GroupMember>>::iterator Group::end()
{
    return m_members.end();
}

