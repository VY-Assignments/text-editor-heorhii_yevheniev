#pragma once

#include "TextDocument.h"

class ProgramInterface {
private:
    TextDocument document;
    void printMenu() const;
    void addTextRow();
    void addContactRow();
    void addChecklistRow();
    void toggleChecklistRow();
    void printDocument() const;
    void printSerializedDocument() const;
    void saveDocument();
    void loadDocument();
    void clearInput();
public:
    void run();
};