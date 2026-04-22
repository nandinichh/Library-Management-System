#pragma once
#include <string>
#include <vector>

struct BorrowRecord {
    std::string isbn;
    std::string title;
    std::string borrowDate;
    std::string dueDate;
    bool returned;

    BorrowRecord(std::string isbn, std::string title, std::string borrowDate, std::string dueDate)
        : isbn(isbn), title(title), borrowDate(borrowDate), dueDate(dueDate), returned(false) {}
};

class Member {
public:
    std::string memberId;
    std::string name;
    std::string email;
    std::string phone;
    double fineBalance;
    std::vector<BorrowRecord> borrowHistory;
    std::vector<std::string> currentlyBorrowed; // ISBNs

    Member() : fineBalance(0.0) {}
    Member(std::string id, std::string name, std::string email, std::string phone)
        : memberId(id), name(name), email(email), phone(phone), fineBalance(0.0) {}

    bool hasBorrowed(const std::string& isbn) const {
        for (const auto& isbn_ : currentlyBorrowed)
            if (isbn_ == isbn) return true;
        return false;
    }

    int borrowCount() const { return currentlyBorrowed.size(); }

    void display() const;
};
