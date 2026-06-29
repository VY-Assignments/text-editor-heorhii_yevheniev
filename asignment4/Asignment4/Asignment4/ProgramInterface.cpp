#include "ProgramInterface.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include <iostream>
#include <limits>
#include <string>

void ProgramInterface::printMenu() const {
	std::cout << "Choose command\n";
	std::cout << "1. Add text row\n";
	std::cout << "2. Add contact row\n";
	std::cout << "3. Add checklist row\n";
	std::cout << "4. Toggle checklist row\n";
	std::cout << "5. Print document\n";
	std::cout << "6. Print serialized document\n";
	std::cout << "0. Exit\n";
}

void ProgramInterface::clearInput() {
	std::cin.clear();
	std::cin.ignore(10000, '\n');
}

void ProgramInterface::addTextRow() {
	std::string text;
	std::cout << "enter text: ";
	std::getline(std::cin, text);
	document.addLine(new TextLine(text));

	std::cout << "Text row added.\n";
}

void ProgramInterface::addContactRow() {
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
	std::string item;
	std::cout << "enter checklist item: ";
	std::getline(std::cin, item);
	document.addLine(new ChecklistLine(item, false));
	std::cout << "Checklist row added\n";
}

void ProgramInterface::toggleChecklistRow() {
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
		case 1:
			addTextRow(); break;
		case 2:
			addContactRow(); break;
		case 3:
			addChecklistRow(); break;
		case 4:
			toggleChecklistRow();break;
		case 5:
			printDocument();break;
		case 6:
			printSerializedDocument();break;
		case 0:
			running = false;break;
		default:
			std::cout << "wrong command\n";break;
		}
	}
}