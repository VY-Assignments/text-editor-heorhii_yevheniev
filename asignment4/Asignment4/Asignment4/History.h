#pragma once
#include "TextDocument.h"
#include <string>

class History {
private:
    std::string* undoStack;
    std::string* redoStack;
    int undoTop;
    int redoTop;
    int capacity;
    void push(std::string* stack, int& top, const std::string& state);
public:
    History(int maxStates = 5);
    void saveState(const TextDocument& document);
    bool undo(TextDocument& document);
    bool redo(TextDocument& document);
    void clear();

    ~History();
};