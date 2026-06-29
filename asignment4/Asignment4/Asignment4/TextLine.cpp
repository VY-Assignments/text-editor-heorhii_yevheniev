#include "TextLine.h"
#include <iostream>

TextLine::TextLine(const std::string& value) {
    text = value;
}

void TextLine::print() const {
    std::cout << "Text: " << text << std::endl;
}

std::string TextLine::serialize() const {
    return "TEXT|" + text;
}