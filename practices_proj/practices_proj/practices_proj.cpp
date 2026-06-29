#include<iostream>
#include<string>
class Computer {
private:
	std::string model;
	int ram;
public:
	Computer(std::string model, int ram) {
		this->model = model;
		this->ram = ram;
	}

	void describe() const {
		std::cout << "Computer:" << model << ", RAM:" << ram << "\n";
	}

};
class Printer {
private:
	std::string model;
	bool color;
public:
	Printer(std::string model, bool color) {
		this->model = model;
		this->color = color;
	}

	void describe()const{
		std::cout << "Printer model:" << model;
		if (color) {
			std::cout << ", IS color printor\n";
		}
		else {
			std::cout << "is NOT color printor\n";
		}
		}

};
class Teacher {
private:
	std::string name;
	std::string disceplyne;
public:
	Teacher(std::string Name, std::string Disceplyne) {
		this->disceplyne = Disceplyne;
		this->name = Name;
	}

	void describe() const {
		std::cout << "Teachers name: " << name << ", Teachers subject: " << disceplyne << "\n";
	}
};

class Student {
private:
	std::string name;
	int year;
public:
	Student(std::string name, int year) {
		this->name = name;
		this->year = year;
	}

	void describe() const {
		std::cout << "Students name: " << name << " student year of class: " << year << "\n";

	}
};
class Clasroom {
private:
	std::string roomNumber;
	Computer computer;
	Printer printer;
	Teacher* teacher;
	static const int max_students = 30;
	Student* students[max_students];
	int student_count;
public:
	Clasroom(std::string roomNumber,
		std::string computer_model,
		int computer_ram,
		std::string printer_model,
		bool printer_color)
		: computer(computer_model, computer_ram),
		printer(printer_model, printer_color) {
		this->roomNumber = roomNumber;
		this->teacher = nullptr;
		this->student_count = 0;
		for (int i = 0; i < max_students; i++) {
			students[i] = nullptr;
		}
	}
	void setTeacher(Teacher* teacher) {
		this->teacher = teacher;
	}

	void addStudent(Student* student) {
		if (student_count < max_students) {
			students[student_count] = student;
			student_count++;
		}
		else {
			std::cout << "clasroom is full, can not add student\n";
		}
	}

	void describe() {
		std::cout << "Clasroom number: " << roomNumber << "\n\n";
		computer.describe();
		printer.describe();
		if (teacher != nullptr) {
			teacher->describe();
		}
		else {
			std::cout << "no teacher in clasroom\n";
		}

		if (student_count == 0) {
			std::cout << "no students in clasroom\n";
		}
		else {
			for (int i = 0; i < student_count; i++) {
				students[i]->describe();
			}
		}
	}
};

int main() {
	Teacher teacher("Yevhenii Kubiuk", "CS560");

	Student student1("Anna", 11);
	Student student2("george", 10);
	Student student3("Sofia", 10);

	Clasroom clasroom(
		"F1",
		"chotatammodel228",
		16,
		"super printer",
		true
	);

	clasroom.setTeacher(&teacher);

	clasroom.addStudent(&student1);
	clasroom.addStudent(&student2);
	clasroom.addStudent(&student3);

	clasroom.describe();

	return 0;
}