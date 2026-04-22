#include "Library.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <stdexcept>

const double Library::FINE_PER_DAY = 0.50;

// ─── Date Utilities ───────────────────────────────────────────────────────────

std::string Library::getCurrentDate() const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::ostringstream oss;
    oss << (1900 + ltm->tm_year) << "-"
        << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << "-"
        << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    return oss.str();
}

std::string Library::calculateDueDate() const {
    time_t now = time(0);
    now += LOAN_PERIOD_DAYS * 24 * 3600;
    tm* ltm = localtime(&now);
    std::ostringstream oss;
    oss << (1900 + ltm->tm_year) << "-"
        << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << "-"
        << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    return oss.str();
}

int Library::daysBetween(const std::string& d1, const std::string& d2) const {
    auto parseDate = [](const std::string& d) -> time_t {
        tm t = {};
        sscanf(d.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
        t.tm_year -= 1900;
        t.tm_mon -= 1;
        return mktime(&t);
    };
    double diff = difftime(parseDate(d2), parseDate(d1));
    return (int)(diff / (60 * 60 * 24));
}

double Library::calculateFine(const std::string& dueDate) const {
    int days = daysBetween(dueDate, getCurrentDate());
    return days > 0 ? days * FINE_PER_DAY : 0.0;
}

std::string Library::generateMemberId() {
    return "M" + std::to_string(memberCounter++);
}

// ─── Book Management ──────────────────────────────────────────────────────────

void Library::addBook(const std::string& isbn, const std::string& title,
                      const std::string& author, const std::string& genre, int copies) {
    if (books.count(isbn)) {
        books[isbn].totalCopies += copies;
        books[isbn].availableCopies += copies;
        std::cout << "Updated existing book. Total copies: " << books[isbn].totalCopies << "\n";
    } else {
        books[isbn] = Book(isbn, title, author, genre, copies);
        std::cout << "Book added: " << title << "\n";
    }
}

void Library::removeBook(const std::string& isbn) {
    if (!books.count(isbn)) { std::cout << "Book not found.\n"; return; }
    books.erase(isbn);
    std::cout << "Book removed.\n";
}

void Library::displayAllBooks() const {
    if (books.empty()) { std::cout << "No books in library.\n"; return; }
    std::cout << "\n" << std::string(90, '-') << "\n";
    std::cout << std::left << std::setw(15) << "ISBN" << std::setw(30) << "Title"
              << std::setw(20) << "Author" << std::setw(15) << "Genre"
              << "Available\n";
    std::cout << std::string(90, '-') << "\n";
    for (const auto& [isbn, book] : books) book.display();
    std::cout << std::string(90, '-') << "\n";
}

void Library::searchByTitle(const std::string& keyword) const {
    std::string kw = keyword;
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
    bool found = false;
    for (const auto& [isbn, book] : books) {
        std::string t = book.title;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        if (t.find(kw) != std::string::npos) { book.display(); found = true; }
    }
    if (!found) std::cout << "No books found matching \"" << keyword << "\"\n";
}

void Library::searchByAuthor(const std::string& keyword) const {
    std::string kw = keyword;
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
    bool found = false;
    for (const auto& [isbn, book] : books) {
        std::string a = book.author;
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        if (a.find(kw) != std::string::npos) { book.display(); found = true; }
    }
    if (!found) std::cout << "No books found by \"" << keyword << "\"\n";
}

void Library::searchByGenre(const std::string& genre) const {
    std::string kw = genre;
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
    bool found = false;
    for (const auto& [isbn, book] : books) {
        std::string g = book.genre;
        std::transform(g.begin(), g.end(), g.begin(), ::tolower);
        if (g.find(kw) != std::string::npos) { book.display(); found = true; }
    }
    if (!found) std::cout << "No books found in genre \"" << genre << "\"\n";
}

void Library::displayBookDetails(const std::string& isbn) const {
    if (!books.count(isbn)) { std::cout << "Book not found.\n"; return; }
    const Book& b = books.at(isbn);
    std::cout << "\n=== Book Details ===\n"
              << "ISBN    : " << b.isbn << "\n"
              << "Title   : " << b.title << "\n"
              << "Author  : " << b.author << "\n"
              << "Genre   : " << b.genre << "\n"
              << "Copies  : " << b.availableCopies << " available / " << b.totalCopies << " total\n"
              << "Waitlist: " << b.waitlist.size() << " people\n";
}

// ─── Member Management ────────────────────────────────────────────────────────

std::string Library::registerMember(const std::string& name, const std::string& email,
                                     const std::string& phone) {
    std::string id = generateMemberId();
    members[id] = Member(id, name, email, phone);
    memberIds.insert(id);
    std::cout << "Member registered! ID: " << id << "\n";
    return id;
}

void Library::removeMember(const std::string& memberId) {
    if (!members.count(memberId)) { std::cout << "Member not found.\n"; return; }
    if (!members[memberId].currentlyBorrowed.empty()) {
        std::cout << "Cannot remove member with borrowed books.\n"; return;
    }
    members.erase(memberId);
    memberIds.erase(memberId);
    std::cout << "Member removed.\n";
}

void Library::displayAllMembers() const {
    if (members.empty()) { std::cout << "No members registered.\n"; return; }
    std::cout << "\n" << std::string(85, '-') << "\n";
    std::cout << std::left << std::setw(10) << "ID" << std::setw(20) << "Name"
              << std::setw(25) << "Email" << std::setw(15) << "Phone"
              << "Status\n";
    std::cout << std::string(85, '-') << "\n";
    for (const auto& [id, member] : members) member.display();
    std::cout << std::string(85, '-') << "\n";
}

void Library::displayMemberDetails(const std::string& memberId) const {
    if (!members.count(memberId)) { std::cout << "Member not found.\n"; return; }
    const Member& m = members.at(memberId);
    std::cout << "\n=== Member Details ===\n"
              << "ID      : " << m.memberId << "\n"
              << "Name    : " << m.name << "\n"
              << "Email   : " << m.email << "\n"
              << "Phone   : " << m.phone << "\n"
              << "Fine    : $" << std::fixed << std::setprecision(2) << m.fineBalance << "\n"
              << "Currently Borrowed (" << m.currentlyBorrowed.size() << "):\n";
    for (const auto& isbn : m.currentlyBorrowed) {
        if (books.count(isbn))
            std::cout << "  - [" << isbn << "] " << books.at(isbn).title << "\n";
    }
    std::cout << "Borrow History (" << m.borrowHistory.size() << " total)\n";
}

// ─── Borrow & Return ──────────────────────────────────────────────────────────

bool Library::borrowBook(const std::string& memberId, const std::string& isbn) {
    if (!members.count(memberId)) { std::cout << "Member not found.\n"; return false; }
    if (!books.count(isbn)) { std::cout << "Book not found.\n"; return false; }

    Member& m = members[memberId];
    Book& b = books[isbn];

    if (m.fineBalance > 0) {
        std::cout << "Cannot borrow. Outstanding fine: $"
                  << std::fixed << std::setprecision(2) << m.fineBalance << "\n";
        return false;
    }
    if (m.borrowCount() >= MAX_BORROW_LIMIT) {
        std::cout << "Borrow limit reached (" << MAX_BORROW_LIMIT << " books max).\n";
        return false;
    }
    if (m.hasBorrowed(isbn)) {
        std::cout << "Member already has this book.\n"; return false;
    }
    if (!b.isAvailable()) {
        std::cout << "No copies available. Join the waitlist? (y/n): ";
        char ch; std::cin >> ch;
        if (ch == 'y' || ch == 'Y') joinWaitlist(memberId, isbn);
        return false;
    }

    std::string borrowDate = getCurrentDate();
    std::string dueDate = calculateDueDate();

    b.availableCopies--;
    m.currentlyBorrowed.push_back(isbn);
    m.borrowHistory.push_back(BorrowRecord(isbn, b.title, borrowDate, dueDate));

    std::cout << "Book borrowed successfully!\n"
              << "  Due Date: " << dueDate << "\n";
    return true;
}

bool Library::returnBook(const std::string& memberId, const std::string& isbn) {
    if (!members.count(memberId)) { std::cout << "Member not found.\n"; return false; }
    if (!books.count(isbn)) { std::cout << "Book not found.\n"; return false; }

    Member& m = members[memberId];
    Book& b = books[isbn];

    if (!m.hasBorrowed(isbn)) {
        std::cout << "This member did not borrow this book.\n"; return false;
    }

    // Calculate fine
    for (auto& record : m.borrowHistory) {
        if (record.isbn == isbn && !record.returned) {
            double fine = calculateFine(record.dueDate);
            if (fine > 0) {
                m.fineBalance += fine;
                std::cout << "Late return! Fine added: $"
                          << std::fixed << std::setprecision(2) << fine << "\n";
            }
            record.returned = true;
            break;
        }
    }

    // Remove from currently borrowed
    m.currentlyBorrowed.erase(
        std::remove(m.currentlyBorrowed.begin(), m.currentlyBorrowed.end(), isbn),
        m.currentlyBorrowed.end());

    b.availableCopies++;

    // Notify waitlist
    std::string nextMember = b.nextOnWaitlist();
    if (!nextMember.empty() && members.count(nextMember)) {
        std::cout << "Notifying waitlisted member: "
                  << members[nextMember].name << " (" << nextMember << ")\n";
    }

    std::cout << "Book returned successfully.\n";
    return true;
}

void Library::joinWaitlist(const std::string& memberId, const std::string& isbn) {
    if (!books.count(isbn)) { std::cout << "Book not found.\n"; return; }
    Book& b = books[isbn];
    for (const auto& id : b.waitlist) {
        if (id == memberId) { std::cout << "Already on waitlist.\n"; return; }
    }
    b.addToWaitlist(memberId);
    std::cout << "Added to waitlist. Position: " << b.waitlist.size() << "\n";
}

// ─── Fines ────────────────────────────────────────────────────────────────────

void Library::checkFines(const std::string& memberId) {
    if (!members.count(memberId)) { std::cout << "Member not found.\n"; return; }
    Member& m = members[memberId];

    // Recalculate fines for currently borrowed books
    double newFines = 0.0;
    for (const auto& isbn : m.currentlyBorrowed) {
        for (auto& record : m.borrowHistory) {
            if (record.isbn == isbn && !record.returned) {
                double fine = calculateFine(record.dueDate);
                if (fine > 0) newFines += fine;
            }
        }
    }

    std::cout << "=== Fine Summary for " << m.name << " ===\n"
              << "Outstanding balance : $" << std::fixed << std::setprecision(2) << m.fineBalance << "\n"
              << "Accruing fines      : $" << newFines << "\n"
              << "Total               : $" << (m.fineBalance + newFines) << "\n";
}

void Library::payFine(const std::string& memberId, double amount) {
    if (!members.count(memberId)) { std::cout << "Member not found.\n"; return; }
    Member& m = members[memberId];
    if (amount >= m.fineBalance) {
        std::cout << "Fine fully paid. Change: $"
                  << std::fixed << std::setprecision(2) << (amount - m.fineBalance) << "\n";
        m.fineBalance = 0.0;
    } else {
        m.fineBalance -= amount;
        std::cout << "Payment received. Remaining fine: $"
                  << std::fixed << std::setprecision(2) << m.fineBalance << "\n";
    }
}

void Library::displayOverdueBooks() const {
    std::cout << "\n=== Overdue Books ===\n";
    bool found = false;
    for (const auto& [id, member] : members) {
        for (const auto& record : member.borrowHistory) {
            if (!record.returned) {
                int days = daysBetween(record.dueDate, getCurrentDate());
                if (days > 0) {
                    std::cout << "Member: " << member.name << " (" << id << ")"
                              << "  Book: " << record.title
                              << "  Overdue by: " << days << " day(s)"
                              << "  Fine: $" << std::fixed << std::setprecision(2)
                              << (days * FINE_PER_DAY) << "\n";
                    found = true;
                }
            }
        }
    }
    if (!found) std::cout << "No overdue books.\n";
}

// ─── File I/O ─────────────────────────────────────────────────────────────────

void Library::saveBooksToFile(const std::string& filename) const {
    std::ofstream f(filename);
    for (const auto& [isbn, b] : books) {
        f << b.isbn << "|" << b.title << "|" << b.author << "|"
          << b.genre << "|" << b.totalCopies << "|" << b.availableCopies << "\n";
    }
    std::cout << "Books saved to " << filename << "\n";
}

void Library::loadBooksFromFile(const std::string& filename) {
    std::ifstream f(filename);
    if (!f) { std::cout << "Could not open " << filename << "\n"; return; }
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream ss(line);
        std::string isbn, title, author, genre, total, available;
        std::getline(ss, isbn, '|'); std::getline(ss, title, '|');
        std::getline(ss, author, '|'); std::getline(ss, genre, '|');
        std::getline(ss, total, '|'); std::getline(ss, available, '|');
        Book b(isbn, title, author, genre, std::stoi(total));
        b.availableCopies = std::stoi(available);
        books[isbn] = b;
    }
    std::cout << "Books loaded from " << filename << "\n";
}

void Library::saveMembersToFile(const std::string& filename) const {
    std::ofstream f(filename);
    for (const auto& [id, m] : members) {
        f << m.memberId << "|" << m.name << "|" << m.email << "|"
          << m.phone << "|" << m.fineBalance << "\n";
    }
    std::cout << "Members saved to " << filename << "\n";
}

void Library::loadMembersFromFile(const std::string& filename) {
    std::ifstream f(filename);
    if (!f) { std::cout << "Could not open " << filename << "\n"; return; }
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream ss(line);
        std::string id, name, email, phone, fine;
        std::getline(ss, id, '|'); std::getline(ss, name, '|');
        std::getline(ss, email, '|'); std::getline(ss, phone, '|');
        std::getline(ss, fine, '|');
        Member m(id, name, email, phone);
        m.fineBalance = std::stod(fine);
        members[id] = m;
        memberIds.insert(id);
        // Update counter so new IDs don't clash
        int num = std::stoi(id.substr(1));
        if (num >= memberCounter) memberCounter = num + 1;
    }
    std::cout << "Members loaded from " << filename << "\n";
}
