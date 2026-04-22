#pragma once
#include <string>
#include <vector>

class Book {
public:
    std::string isbn;
    std::string title;
    std::string author;
    std::string genre;
    int totalCopies;
    int availableCopies;
    std::vector<std::string> waitlist; // member IDs

    Book() : totalCopies(0), availableCopies(0) {}
    Book(std::string isbn, std::string title, std::string author, std::string genre, int copies)
        : isbn(isbn), title(title), author(author), genre(genre),
          totalCopies(copies), availableCopies(copies) {}

    bool isAvailable() const { return availableCopies > 0; }

    void addToWaitlist(const std::string& memberId) {
        waitlist.push_back(memberId);
    }

    std::string nextOnWaitlist() {
        if (waitlist.empty()) return "";
        std::string next = waitlist.front();
        waitlist.erase(waitlist.begin());
        return next;
    }

    void display() const;
};
