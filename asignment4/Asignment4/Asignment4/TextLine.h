#pragma once
#include "Line.h"
#include <string>

class TextLine : public Line {
private:
    std::string text;
public:
    TextLine(const std::string& value);
    void print() const override;
    std::string serialize() const override;
};