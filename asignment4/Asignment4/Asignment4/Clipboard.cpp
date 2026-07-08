#include "Clipboard.h"

void Clipboard::copy(const std::string& text) {
    buffer = text;
}

bool Clipboard::isEmpty() const {
    return buffer.empty();
}

const std::string& Clipboard::getText() const {
    return buffer;
}

void Clipboard::clear() {
    buffer.clear();
}