#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
#include "Book.h"
#include "Member.h"

class Library {
private:
    std::map<std::string, Book> books;       // isbn -> Book
    std::map<std::string, Member> members;   // memberId -> Member
    std::set<std::string> memberIds;
    int memberCounter;

    static const int MAX_BORROW_LIMIT = 5;
    static const int LOAN_PERIOD_DAYS = 14;
    static const double FINE_PER_DAY;

    std::string getCurrentDate() const;
    std::string calculateDueDate() const;
    int daysBetween(const std::string& date1, const std::string& date2) const;
    double calculateFine(const std::string& dueDate) const;
    std::string generateMemberId();

public:
    Library() : memberCounter(1000) {}

    // Book management
    void addBook(const std::string& isbn, const std::string& title,
                 const std::string& author, const std::string& genre, int copies);
    void removeBook(const std::string& isbn);
    void displayAllBooks() const;
    void searchByTitle(const std::string& keyword) const;
    void searchByAuthor(const std::string& keyword) const;
    void searchByGenre(const std::string& genre) const;
    void displayBookDetails(const std::string& isbn) const;

    // Member management
    std::string registerMember(const std::string& name, const std::string& email,
                                const std::string& phone);
    void removeMember(const std::string& memberId);
    void displayAllMembers() const;
    void displayMemberDetails(const std::string& memberId) const;

    // Borrowing & returning
    bool borrowBook(const std::string& memberId, const std::string& isbn);
    bool returnBook(const std::string& memberId, const std::string& isbn);
    void joinWaitlist(const std::string& memberId, const std::string& isbn);

    // Fines
    void checkFines(const std::string& memberId);
    void payFine(const std::string& memberId, double amount);
    void displayOverdueBooks() const;

    // File I/O
    void saveBooksToFile(const std::string& filename) const;
    void loadBooksFromFile(const std::string& filename);
    void saveMembersToFile(const std::string& filename) const;
    void loadMembersFromFile(const std::string& filename);
};
