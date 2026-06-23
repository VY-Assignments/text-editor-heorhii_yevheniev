#include <iostream>
#include <string>
#include <fstream>

class Dog {
private:
    std::string name;
    uint8_t age;

public:
    Dog(std::string n, uint8_t a) {
        name = n;
        age = a;
    }

    std::string get_name() const { return name; }
    uint8_t get_age() const { return age; }

    void serialize(uint8_t* buffer, size_t& length) const {
        length = 0;

        for (int i = 0; i < name.length(); i++) {
            buffer[length] = name[i];
            length++;
        }

        buffer[length] = '\0';
        length++;

        buffer[length] = age;
        length++;
    }

    static Dog deserialize(const uint8_t* buffer, size_t length) {
        std::string restored_name = "";
        size_t pos = 0;

        while (buffer[pos] != '\0') {
            restored_name += buffer[pos];
            pos++;
        }

        pos++;

        uint8_t restored_age = buffer[pos];

        return Dog(restored_name, restored_age);
    }
};

int main() {
    Dog originalDog("Jack", 5);
    std::cout << " Original: " << originalDog.get_name() << " (Age: " << (int)originalDog.get_age() << ")\n";

    uint8_t memoryArray[100];
    size_t dataSize = 0;

    originalDog.serialize(memoryArray, dataSize);
    std::cout << " Packed Total bytes : " << dataSize << "\n";

    Dog restoredDog = Dog::deserialize(memoryArray, dataSize);
    std::cout << " Restored: " << restoredDog.get_name() << " (Age: " << (int)restoredDog.get_age() << ")\n\n";

    std::ofstream outFile("dog_data.bin", std::ios::binary);
    outFile.write((char*)memoryArray, dataSize);
    outFile.close();
    std::cout << "Saved to file 'dog_data.bin'\n";

    uint8_t fileArray[100];
    std::ifstream inFile("dog_data.bin", std::ios::binary);
    inFile.read((char*)fileArray, dataSize);
    inFile.close();
    std::cout << "Read from file.\n";

    Dog fileDog = Dog::deserialize(fileArray, dataSize);
    std::cout << "Restored from file: " << fileDog.get_name() << " (Age: " << (int)fileDog.get_age() << ")\n";

    return 0;
}