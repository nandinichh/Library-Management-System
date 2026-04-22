#include <iostream>
#include <string>
#include <limits>
#include "Library.h"

void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printHeader(const std::string& title) {
    std::cout << "\n╔══════════════════════════════════╗\n"
              << "║  " << title << std::string(33 - title.size(), ' ') << "║\n"
              << "╚══════════════════════════════════╝\n";
}

void bookMenu(Library& lib) {
    while (true) {
        printHeader("BOOK MANAGEMENT");
        std::cout << "  1. Add Book\n"
                  << "  2. Remove Book\n"
                  << "  3. View All Books\n"
                  << "  4. Search by Title\n"
                  << "  5. Search by Author\n"
                  << "  6. Search by Genre\n"
                  << "  7. View Book Details\n"
                  << "  0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();

        std::string input;
        int copies;

        switch (ch) {
            case 1: {
                std::string isbn, title, author, genre;
                std::cout << "ISBN: "; std::getline(std::cin, isbn);
                std::cout << "Title: "; std::getline(std::cin, title);
                std::cout << "Author: "; std::getline(std::cin, author);
                std::cout << "Genre: "; std::getline(std::cin, genre);
                std::cout << "Copies: "; std::cin >> copies; clearInput();
                lib.addBook(isbn, title, author, genre, copies);
                break;
            }
            case 2:
                std::cout << "ISBN: "; std::getline(std::cin, input);
                lib.removeBook(input);
                break;
            case 3: lib.displayAllBooks(); break;
            case 4:
                std::cout << "Keyword: "; std::getline(std::cin, input);
                lib.searchByTitle(input);
                break;
            case 5:
                std::cout << "Author: "; std::getline(std::cin, input);
                lib.searchByAuthor(input);
                break;
            case 6:
                std::cout << "Genre: "; std::getline(std::cin, input);
                lib.searchByGenre(input);
                break;
            case 7:
                std::cout << "ISBN: "; std::getline(std::cin, input);
                lib.displayBookDetails(input);
                break;
            case 0: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void memberMenu(Library& lib) {
    while (true) {
        printHeader("MEMBER MANAGEMENT");
        std::cout << "  1. Register Member\n"
                  << "  2. Remove Member\n"
                  << "  3. View All Members\n"
                  << "  4. View Member Details\n"
                  << "  0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();
        std::string input;

        switch (ch) {
            case 1: {
                std::string name, email, phone;
                std::cout << "Name: "; std::getline(std::cin, name);
                std::cout << "Email: "; std::getline(std::cin, email);
                std::cout << "Phone: "; std::getline(std::cin, phone);
                lib.registerMember(name, email, phone);
                break;
            }
            case 2:
                std::cout << "Member ID: "; std::getline(std::cin, input);
                lib.removeMember(input);
                break;
            case 3: lib.displayAllMembers(); break;
            case 4:
                std::cout << "Member ID: "; std::getline(std::cin, input);
                lib.displayMemberDetails(input);
                break;
            case 0: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void borrowMenu(Library& lib) {
    while (true) {
        printHeader("BORROW & RETURN");
        std::cout << "  1. Borrow Book\n"
                  << "  2. Return Book\n"
                  << "  3. Join Waitlist\n"
                  << "  0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();
        std::string memberId, isbn;

        switch (ch) {
            case 1:
                std::cout << "Member ID: "; std::getline(std::cin, memberId);
                std::cout << "ISBN: "; std::getline(std::cin, isbn);
                lib.borrowBook(memberId, isbn);
                break;
            case 2:
                std::cout << "Member ID: "; std::getline(std::cin, memberId);
                std::cout << "ISBN: "; std::getline(std::cin, isbn);
                lib.returnBook(memberId, isbn);
                break;
            case 3:
                std::cout << "Member ID: "; std::getline(std::cin, memberId);
                std::cout << "ISBN: "; std::getline(std::cin, isbn);
                lib.joinWaitlist(memberId, isbn);
                break;
            case 0: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void fineMenu(Library& lib) {
    while (true) {
        printHeader("FINES");
        std::cout << "  1. Check Fines\n"
                  << "  2. Pay Fine\n"
                  << "  3. View All Overdue Books\n"
                  << "  0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();
        std::string memberId;

        switch (ch) {
            case 1:
                std::cout << "Member ID: "; std::getline(std::cin, memberId);
                lib.checkFines(memberId);
                break;
            case 2: {
                std::cout << "Member ID: "; std::getline(std::cin, memberId);
                double amount;
                std::cout << "Amount: $"; std::cin >> amount; clearInput();
                lib.payFine(memberId, amount);
                break;
            }
            case 3: lib.displayOverdueBooks(); break;
            case 0: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void fileMenu(Library& lib) {
    while (true) {
        printHeader("SAVE / LOAD");
        std::cout << "  1. Save Books\n"
                  << "  2. Load Books\n"
                  << "  3. Save Members\n"
                  << "  4. Load Members\n"
                  << "  0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();

        switch (ch) {
            case 1: lib.saveBooksToFile("books.txt"); break;
            case 2: lib.loadBooksFromFile("books.txt"); break;
            case 3: lib.saveMembersToFile("members.txt"); break;
            case 4: lib.loadMembersFromFile("members.txt"); break;
            case 0: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

int main() {
    Library lib;

    // Load saved data on startup if files exist
    lib.loadBooksFromFile("books.txt");
    lib.loadMembersFromFile("members.txt");

    while (true) {
        printHeader("LIBRARY MANAGEMENT");
        std::cout << "  1. Book Management\n"
                  << "  2. Member Management\n"
                  << "  3. Borrow & Return\n"
                  << "  4. Fines\n"
                  << "  5. Save / Load Data\n"
                  << "  0. Exit\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();

        switch (ch) {
            case 1: bookMenu(lib); break;
            case 2: memberMenu(lib); break;
            case 3: borrowMenu(lib); break;
            case 4: fineMenu(lib); break;
            case 5: fileMenu(lib); break;
            case 0:
                lib.saveBooksToFile("books.txt");
                lib.saveMembersToFile("members.txt");
                std::cout << "Data saved. Goodbye!\n";
                return 0;
            default: std::cout << "Invalid choice.\n";
        }
    }
}
