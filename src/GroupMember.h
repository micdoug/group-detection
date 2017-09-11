//
// Created by micha on 8/22/2017.
//

#ifndef GROUP_DETECTION_GROUPMEMBER_H
#define GROUP_DETECTION_GROUPMEMBER_H


class GroupMember final
{
public:
    explicit GroupMember(int nid);
    GroupMember(const GroupMember &other);
    ~GroupMember() = default;

    int id() const;
    bool isActive() const;
    void setIsActive(bool active);
    int responseCounter() const;
    int failureCounter() const;

    void resetCounters();
    void incrementResponseCounter();
    void incrementFailureCounter();

private:
    int m_nid;
    int m_responseCounter;
    int m_failureCounter;
    bool m_isActive;
};


#endif //GROUP_DETECTION_GROUPMEMBER_H
