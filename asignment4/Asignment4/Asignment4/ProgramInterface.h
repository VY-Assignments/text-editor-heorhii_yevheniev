#pragma once
#include "TextLine.h"
#include "TextDocument.h"
#include "Clipboard.h"
#include "History.h"

class ProgramInterface {
private:
    TextDocument document;
    Clipboard clipboard;
    History history;
    void printMenu() const;
    void clearInput();
    void appendText();
    void startNewTextRow();
    void addContactRow();
    void addChecklistRow();
    void toggleChecklistRow();
    void printDocument() const;
    void printSerializedDocument() const;
    void saveDocument();
    void loadDocument();
    void insertText();
    void deleteText();
    void searchText() const;
    void insertWithReplacement();
    void copyText();
    void pasteText();
    void cutText();
    void undo();
    void redo();
    void encryptDocument();
    void decryptDocument();
    void encryptDocumentVigenere();
    void decryptDocumentVigenere();
    TextLine* getTextLineByIndex(int index) const;
public:
    void run();
};