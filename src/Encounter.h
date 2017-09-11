#ifndef ENCOUNTER_H
#define ENCOUNTER_H

class Encounter
{
public:
    explicit Encounter(long start, long end);
    ~Encounter() = default;

    long duration() const;
    long start() const;
    long end() const;
    bool hasIntersection(const Encounter &other) const;
private:
    long m_start;
    long m_end;
};

#endif