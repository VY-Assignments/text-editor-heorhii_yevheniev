#pragma once
#include <string>
class Line {
public:
    virtual void print() const = 0;
    virtual std::string serialize() const = 0;

    virtual ~Line() {}
};