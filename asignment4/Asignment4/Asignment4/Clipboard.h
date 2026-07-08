#pragma once
#include <string>

class Clipboard {
private:
    std::string buffer;
public:
    void copy(const std::string& text);
    bool isEmpty() const;
    const std::string& getText() const;
    void clear();
};
