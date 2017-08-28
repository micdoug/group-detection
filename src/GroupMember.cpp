//
// Created by micha on 8/22/2017.
//

#include "GroupMember.h"

GroupMember::GroupMember(int nid):
    m_nid{nid}, 
    m_responseCounter{0}, 
    m_failureCounter{0},
    m_isActive{false}
{

}

int GroupMember::id() const {
    return m_nid;
}

bool GroupMember::isActive() const {
    return m_isActive;
}

void GroupMember::setIsActive(bool active) {
    m_isActive = active;
}

int GroupMember::responseCounter() const {
    return m_responseCounter;
}

int GroupMember::failureCounter() const {
    return m_failureCounter;
}

void GroupMember::incrementResponseCounter()
{
    m_failureCounter = 0;
    m_responseCounter++;
}

void GroupMember::incrementFailureCounter()
{
    m_responseCounter = 0;
    m_failureCounter++;
}