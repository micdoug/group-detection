#ifndef GROUP_H
#define GROUP_H

#include "GroupMember.h"
#include <map>
#include <set>
#include <memory>
#include <string>

class Group final 
{
public:
    Group();
    ~Group() = default;

    std::set<int> memberKeys() const;
    long created() const;
    void setCreated(const long time);
    long destroyed() const;
    void setDestroyed(const long time);
    void addMember(const int id);
    void removeMember(const int id);
    int size() const;
    std::string to_string() const;

    std::map<int, std::unique_ptr<GroupMember>>::iterator begin();
    std::map<int, std::unique_ptr<GroupMember>>::iterator end();

    GroupMember &operator[](int key);
private:
    long m_created;
    long m_destroyed;
    std::map<int, std::unique_ptr<GroupMember>> m_members;
};



#endif