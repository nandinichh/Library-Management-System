#include "Book.h"
#include "Member.h"
#include <iostream>
#include <iomanip>

void Book::display() const {
    std::cout << std::left
              << std::setw(15) << isbn
              << std::setw(30) << title
              << std::setw(20) << author
              << std::setw(15) << genre
              << std::setw(8)  << availableCopies << "/" << totalCopies
              << (waitlist.empty() ? "" : " [Waitlist: " + std::to_string(waitlist.size()) + "]")
              << "\n";
}

void Member::display() const {
    std::cout << std::left
              << std::setw(10) << memberId
              << std::setw(20) << name
              << std::setw(25) << email
              << std::setw(15) << phone
              << "Books: " << currentlyBorrowed.size()
              << "  Fine: $" << std::fixed << std::setprecision(2) << fineBalance
              << "\n";
}
