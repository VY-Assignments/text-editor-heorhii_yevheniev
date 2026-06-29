#pragma once
#include"Line.h"
#include<string>

class ChecklistLine : public Line {
private:
	std::string item;
	bool checked;
public:
	ChecklistLine(const std::string& item, bool checked);
	void print() const override;
	std::string serialize() const override;
	void toggle();
};