#include "History.h"

History::History(int maxStates) {
    capacity = maxStates;
    undoTop = 0;
    redoTop = 0;
    undoStack = new std::string[capacity];
    redoStack = new std::string[capacity];
}

void History::push(std::string* stack, int& top, const std::string& state) {
    if (top >= capacity) {
        for (int i = 1; i < capacity; i++) {
            stack[i - 1] = stack[i];
        }
        top = capacity - 1;
    }
    stack[top] = state;
    top++;
}

void History::saveState(const TextDocument& document) {
    push(undoStack, undoTop, document.serialize());
    redoTop = 0;
}

bool History::undo(TextDocument& document) {
    if (undoTop <= 0) {
        return false;
    }
    push(redoStack, redoTop, document.serialize());
    undoTop--;
    std::string previousState = undoStack[undoTop];
    return document.deserialize(previousState);
}

bool History::redo(TextDocument& document) {
    if (redoTop <= 0) {
        return false;
    }
    push(undoStack, undoTop, document.serialize());
    redoTop--;
    std::string nextState = redoStack[redoTop];
    return document.deserialize(nextState);
}

void History::clear() {
    undoTop = 0;
    redoTop = 0;
}

History::~History() {
    delete[] undoStack;
    delete[] redoStack;
    undoStack = nullptr;
    redoStack = nullptr;
}