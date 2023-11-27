#include <iostream>
#include <iomanip>
#include <string>

class Account {
protected:
    std::string acc_number;
    std::string acc_holder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initial)
        : acc_number(number), acc_holder(holder), balance(initial) {}

    virtual void details() const {
        std::cout << "Account Details for Account (ID: " << acc_number << "):\n"
                  << "   Holder: " << acc_holder << "\n"
                  << "   Balance: $" << std::fixed << std::setprecision(2) << get_balance() << "\n";
    }

    double get_balance() const {
        return balance;
    }

    virtual void deposit(double amount) {
        balance += amount;
        std::cout << "Deposited $" << amount << " into the account.\n";
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the account.\n";
        } else {
            std::cout << "Insufficient funds.\n";
        }
    }

    // Protected function to modify balance
    void modify(double amount) {
        balance -= amount;
    }
};

class SavingsAcc : public Account {
private:
    double interest;

public:
    SavingsAcc(const std::string& number, const std::string& holder, double initial, double rate)
        : Account(number, holder, initial), interest(rate) {}

    void details() const override {
        Account::details();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interest * 100 << "%\n";
    }

    void withdraw(double amount) override {
        const double min_balance = 100.0;  // Minimum balance requirement for a savings account
        if (get_balance() - amount >= min_balance) {
            modify(amount);
            std::cout << "Withdrawn $" << amount << " from the savings account.\n";
        } else {
            std::cout << "Withdrawal not allowed. Minimum balance requirement not met.\n";
        }
    }
};

class CurrrentAcc : public Account {
private:
    double overdraft;

public:
    CurrrentAcc(const std::string& number, const std::string& holder, double initial, double limit)
        : Account(number, holder, initial), overdraft(limit) {}

    void details() const override {
        Account::details();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraft << "\n";
    }

    void withdraw(double amount) override {
        if (get_balance() + overdraft >= amount) {
            modify(amount);
            std::cout << "Withdrawn $" << amount << " from the current account.\n";
        } else {
            std::cout << "Withdrawal not allowed. Exceeds overdraft limit.\n";
        }
    }
};

// Overload the + operator for transferring money between accounts
CurrrentAcc operator+(CurrrentAcc& current, SavingsAcc& savings) {
    double transfer = 300.0;
    if (savings.get_balance() >= transfer) {
        current.deposit(transfer);
        savings.modify(transfer);
        std::cout << "Transferred $" << transfer << " from savings to current account.\n";
    } else {
        std::cout << "Transfer failed. Insufficient funds in savings.\n";
    }

    return current;
}

// Overload the << operator to display account details
std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.details();
    return os;
}

int main() {
    SavingsAcc savings("S123", "John Doe", 1000, 0.02);
    CurrrentAcc current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "\nAccount details after deposit and withdrawal:\n";
    std::cout << savings;
    std::cout << current;

    // Transfer money
    current = current + savings;

    std::cout << "\nAccount details after transfer:\n";
    std::cout << savings;
    std::cout << current;

    return 0;
}

