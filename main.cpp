#include <iostream>
#include <filesystem>

int main() {
    for (const auto& entry : std::filesystem::directory_iterator("books_xml")) {
        std::cout << entry.path().filename() << std::endl;
    }
    return 0;
}
