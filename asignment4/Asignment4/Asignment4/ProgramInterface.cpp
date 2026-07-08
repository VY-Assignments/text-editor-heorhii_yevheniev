#include "ProgramInterface.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include "FileManager.h"
#include "DllCipher.h"
#include <iostream>
#include <limits>
#include <string>

void ProgramInterface::printMenu() const {
	std::cout << "Choose command\n";
	std::cout << "1. Append text to last text row\n";
	std::cout << "2. Start new empty text row\n";
	std::cout << "3. Add contact row\n";
	std::cout << "4. Add checklist row\n";
	std::cout << "5. Toggle checklist row\n";
	std::cout << "6. Print document\n";
	std::cout << "7. Save document to file\n";
	std::cout << "8. Load document from file\n";
	std::cout << "9. Print serialized document\n";
	std::cout << "10. Insert text by line and index\n";
	std::cout << "11. Delete text by line, index and count\n";
	std::cout << "12. Search text\n";
	std::cout << "13. Insert with replacement\n";
	std::cout << "14. Copy text\n";
	std::cout << "15. Paste text\n";
	std::cout << "16. Cut text\n";
	std::cout << "17. Undo\n";
	std::cout << "18. Redo\n";
	std::cout << "19. Encrypt document\n";
	std::cout << "20. Decrypt document\n";
	std::cout << "21. Encrypt document with Vigenere\n";
	std::cout << "22. Decrypt document with Vigenere\n";
	std::cout << "0. Exit\n";
}

void ProgramInterface::clearInput() {
	std::cin.clear();
	std::cin.ignore(10000, '\n');
}

TextLine* ProgramInterface::getTextLineByIndex(int index) const {
	Line* line = document.getLine(index);
	if (line == nullptr) {
		return nullptr;
	}
	return dynamic_cast<TextLine*>(line);
}

void ProgramInterface::appendText() {
	history.saveState(document);
	std::string text;

	std::cout << "Enter text to append\n";
	std::getline(std::cin, text);

	if (document.getLineCount() == 0) {
		document.addLine(new TextLine(text));
		std::cout << "Document was empty, new text row created\n";
		return;
	}
	int lastIndex = document.getLineCount() - 1;
	Line* line = document.getLine(lastIndex);
	TextLine* textLine = dynamic_cast<TextLine*>(line);

	if (textLine == nullptr) {
		std::cout << "Last row is not a text row. Start a new text row first\n";
		return;
	}
	textLine->append(text);
	std::cout << "Text appended successfully\n";
}

void ProgramInterface::startNewTextRow() {
	history.saveState(document);
	document.addLine(new TextLine(""));
	std::cout << "New empty text row started\n";
}

void ProgramInterface::insertText() {
	history.saveState(document);
	int lineIndex;
	int position;
	std::string text;
	std::cout << "Enter line index: ";
	if (!(std::cin >> lineIndex)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	std::cout << "Enter position: ";
	if (!(std::cin >> position)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	clearInput();
	TextLine* textLine = getTextLineByIndex(lineIndex);
	if (textLine == nullptr) {
		std::cout << "This operation is available only for text rows or index is out of range\n";
		return;
	}
	std::cout << "Enter text to insert: ";
	std::getline(std::cin, text);
	if (!textLine->insertText(position, text)) {
		std::cout << "Insertion index out of range\n";
		return;
	}
	std::cout << "Text inserted successfully\n";
}

void ProgramInterface::deleteText() {
	history.saveState(document);
	int lineIndex;
	int position;
	int count;
	std::cout << "Enter line index, position and number of symbols\n";
	if (!(std::cin >> lineIndex >> position >> count)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	clearInput();
	TextLine* textLine = getTextLineByIndex(lineIndex);

	if (textLine == nullptr) {
		std::cout << "index is out of range or not a text row\n";
		return;
	}
	if (!textLine->deleteText(position, count)) {
		std::cout << "Invalid position or count\n";
		return;
	}
	std::cout << "Text deleted successfully\n";
}

void ProgramInterface::searchText() const {
	std::string searchValue;
	std::cout << "Enter text to search\n";
	std::getline(std::cin, searchValue);

	if (searchValue.empty()) {
		std::cout << "Search text is empty\n";
		return;
	}
	bool found = false;
	for (int i = 0; i < document.getLineCount(); i++) {
		TextLine* textLine = getTextLineByIndex(i);

		if (textLine == nullptr) {
			continue;
		}
		const std::string& currentText = textLine->getText();
		size_t position = currentText.find(searchValue);
		while (position != std::string::npos) {
			std::cout << "Found in line " << i << " at index " << position << "\n";
			found = true;

			position = currentText.find(searchValue, position + 1);
		}
	}
	if (!found) {
		std::cout << "Text not found\n";
	}
}

void ProgramInterface::insertWithReplacement() {
	history.saveState(document);
	int lineIndex;
	int position;
	std::string text;
	std::cout << "Enter line index\n";

	if (!(std::cin >> lineIndex)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	std::cout << "Enter position\n";

	if (!(std::cin >> position)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	clearInput();
	TextLine* textLine = getTextLineByIndex(lineIndex);

	if (textLine == nullptr) {
		std::cout << "index out of range or not text row\n";
		return;
	}
	std::cout << "Enter replacement text\n";
	std::getline(std::cin, text);

	if (!textLine->replaceText(position, text)) {
		std::cout << "Replacement index out of range\n";
		return;
	}
	std::cout << "Text replaced/inserted successfully\n";
}

void ProgramInterface::copyText() {
	int lineIndex;
	int position;
	int count;
	std::cout << "Enter line index, position and number of symbols\n";

	if (!(std::cin >> lineIndex >> position >> count)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	clearInput();
	TextLine* textLine = getTextLineByIndex(lineIndex);

	if (textLine == nullptr) {
		std::cout << "index out of range or not text row\n";
		return;
	}
	std::string copiedText;
	if (!textLine->getSubstring(position, count, copiedText)) {
		std::cout << "Invalid position or count\n";
		return;
	}
	clipboard.copy(copiedText);
	std::cout << "Text copied to clipboard\n" << copiedText << "\n";
}

void ProgramInterface::pasteText() {
	history.saveState(document);
	if (clipboard.isEmpty()) {
		std::cout << "Clipboard is empty\n";
		return;
	}
	int lineIndex;
	int position;
	std::cout << "Enter line index and position\n";

	if (!(std::cin >> lineIndex >> position)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	clearInput();
	TextLine* textLine = getTextLineByIndex(lineIndex);

	if (textLine == nullptr) {
		std::cout << " index out of range or not a text row\n";
		return;
	}
	if (!textLine->insertText(position, clipboard.getText())) {
		std::cout << "Paste index out of range\n";
		return;
	}
	std::cout << "Text pasted successfully\n";
}

void ProgramInterface::cutText() {
	history.saveState(document);
	int lineIndex;
	int position;
	int count;
	std::cout << "Enter line index, position and number of symbols\n";

	if (!(std::cin >> lineIndex >> position >> count)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	clearInput();
	TextLine* textLine = getTextLineByIndex(lineIndex);

	if (textLine == nullptr) {
		std::cout << "or index out of range or not a text row\n";
		return;
	}

	std::string cutValue;

	if (!textLine->getSubstring(position, count, cutValue)) {
		std::cout << "Invalid position or count\n";
		return;
	}
	clipboard.copy(cutValue);
	if (!textLine->deleteText(position, count)) {
		std::cout << "Could not delete selected text.\n";
		return;
	}
	std::cout << "Text cut to clipboard\n" << cutValue << "\n";
}

void ProgramInterface::undo() {
	if (history.undo(document)) {
		std::cout << "Undo completed\n";
	}
	else {
		std::cout << "No actions to undo\n";
	}
}

void ProgramInterface::redo() {
	if (history.redo(document)) {
		std::cout << "Redo completed\n";
	}
	else {
		std::cout << "No actions to redo\n";
	}
}

void ProgramInterface::encryptDocument() {
	int key;
	std::cout << "Enter Caesar key:\n";
	if (!(std::cin >> key)) {
		std::cout << "Invalid key\n";
		clearInput();
		return;
	}
	clearInput();
	DllCipher cipher;
	if (!cipher.load("CipherDll.dll")) {
		std::cout << "Could not load CipherDll.dll\n";
		return;
	}
	history.saveState(document);
	if (!document.encryptAll(cipher, key)) {
		std::cout << "Encryption failed\n";
		return;
	}

	std::cout << "document encrypted successfully.\n";
}

void ProgramInterface::decryptDocument() {
	int key;
	std::cout << "Enter Caesar key:\n";
	if (!(std::cin >> key)) {
		std::cout << "Invalid key\n";
		clearInput();
		return;
	}
	clearInput();
	DllCipher cipher;

	if (!cipher.load("CipherDll.dll")) {
		std::cout << "Could not load CipherDll.dll\n";
		return;
	}
	history.saveState(document);
	if (!document.decryptAll(cipher, key)) {
		std::cout << "Decryption failed\n";
		return;
	}
	std::cout << "document decrypted successfully\n";
}

void ProgramInterface::encryptDocumentVigenere() {
	std::string key;
	std::cout << "Enter Vigenere key:\n";
	std::getline(std::cin, key);
	if (key.empty()) {
		std::cout << "Key cannot be empty\n";
		return;
	}
	DllCipher cipher;
	if (!cipher.load("CipherDll.dll")) {
		std::cout << "Could not load CipherDll.dll\n";
		return;
	}
	history.saveState(document);
	if (!document.encryptAllVigenere(cipher, key)) {
		std::cout << "Vigenere encryption failed\n";
		return;
	}
	std::cout << "document encrypted with Vigenere successfully\n";
}

void ProgramInterface::decryptDocumentVigenere() {
	std::string key;
	std::cout << "Enter Vigenere key:\n";
	std::getline(std::cin, key);
	if (key.empty()) {
		std::cout << "Key cannot be empty\n";
		return;
	}
	DllCipher cipher;
	if (!cipher.load("CipherDll.dll")) {
		std::cout << "Could not load CipherDll.dll\n";
		return;
	}
	history.saveState(document);
	if (!document.decryptAllVigenere(cipher, key)) {
		std::cout << "Vigenere decryption failed\n";
		return;
	}
	std::cout << "document decrypted with Vigenere successfully\n";
}

void ProgramInterface::addContactRow() {
	history.saveState(document);
	std::string name;
	std::string surname;
	std::string email;
	std::cout << "enter name: ";
	std::getline(std::cin, name);
	std::cout << "enter surname: ";
	std::getline(std::cin, surname);
	std::cout << "enter email: ";
	std::getline(std::cin, email);

	document.addLine(new ContactLine(name, surname, email));
	std::cout << "Contact row added\n";
}

void ProgramInterface::addChecklistRow() {
	history.saveState(document);
	std::string item;
	std::cout << "enter checklist item: ";
	std::getline(std::cin, item);
	document.addLine(new ChecklistLine(item, false));
	std::cout << "Checklist row added\n";
}

void ProgramInterface::toggleChecklistRow() {
	history.saveState(document);
	int index;

	std::cout << "enter checklist row index: ";
	if (!(std::cin >> index)) {
		std::cout << "Invalid input\n";
		clearInput();
		return;
	}
	clearInput();
	Line* line = document.getLine(index);
	if (line == nullptr) {
		std::cout << "index out of range\n";
		return;
	}

	ChecklistLine* checklistLine = dynamic_cast<ChecklistLine*>(line);
	if (checklistLine == nullptr) {
		std::cout << "not a checklist row\n";
		return;
	}

	checklistLine->toggle();
	std::cout << "checklist status changed\n";
}

void ProgramInterface::printDocument() const {
	document.print();
}

void ProgramInterface::printSerializedDocument() const {
	std::cout << "serialized document\n";
	std::cout << document.serialize() << std::endl;
}

void ProgramInterface::saveDocument() {
	std::string path;
	std::cout << "enter path to file\n";
	std::getline(std::cin, path);
	std::string serializedData = document.serialize();

	if (FileManager::saveTextToFile(path, serializedData)) {
		std::cout << "document saved\n";
	}
	else {
		std::cout << "Error occured, unable to save document\n";
	}
}

void ProgramInterface::loadDocument() {
	std::string path;
	std::string data;

	std::cout << "Enter path to file\n";
	std::getline(std::cin, path);

	if (!FileManager::loadTextFromFile(path, data)) {
		std::cout << "Error occured, could not open file\n";
		return;
	}

	if (!document.deserialize(data)) {
		std::cout << "Error occured, file format is incorrect\n";
		return;
	}
	history.clear();
	std::cout << "Document loaded\n";
}


void ProgramInterface::run() {
	int choice = -1;
	bool running = true;

	while (running) {
		printMenu();

		if (!(std::cin >> choice)) {
			std::cout << "invalid input\n";
			clearInput();
			continue;
		}
		clearInput();
		switch (choice) {
		case 1:appendText();break;

		case 2:startNewTextRow();break;

		case 3:addContactRow();break;

		case 4:addChecklistRow();break;

		case 5:toggleChecklistRow();break;

		case 6:printDocument();break;

		case 7:saveDocument();break;

		case 8:loadDocument();break;

		case 9:printSerializedDocument();break;

		case 10:insertText(); break;

		case 11:deleteText();break;

		case 12:searchText(); break;

		case 13:insertWithReplacement();break;

		case 14:copyText();break;

		case 15:pasteText();break;

		case 16:cutText();break;

		case 17:undo();break;

		case 18:redo();break;

		case 19:encryptDocument();break;

		case 20:decryptDocument();break;

		case 21:encryptDocumentVigenere();break;

		case 22:decryptDocumentVigenere();break;

		case 0:running = false;break;

		default:
			std::cout << "wrong command\n";
			break;
		}
	}
}