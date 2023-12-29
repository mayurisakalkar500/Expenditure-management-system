#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>

using namespace std;

string DATA_FILE = "expenses.dat";

struct Expense
{
    string category;
    float amount;
};

class IUser
{
public:
    virtual string Username() = 0;
    virtual string Password() = 0;
    virtual void addExpense(const string &category, float amount) = 0;
    virtual void viewExpenses() = 0;
    virtual void ExpensesFromFile() = 0;
};

class User : public IUser
{
private:
    string name_;
    string pass_;
    unordered_map<string, float> expenses_;
    float total_ = 0;

public:
    User(const string &username, const string &password) : name_(username), pass_(password) {}

    string Username() override
    {
        return name_;
    }

    string Password() override
    {
        return pass_;
    }

    void addExpense(const string &category, float amount) override
    {
        expenses_[category] += amount;
        total_ += amount;
    }

    void viewExpenses() override
    {
        ofstream output("expenses.txt", ios::app);
        cout << "Expenses:" << endl;
        for (const auto &[category, amount] : expenses_)
        {
            cout << category << ": " << amount << endl;
            output << category << " " << amount << endl;
        }
        cout << endl
             << "Total: " << total_ << endl;
        output << endl
               << "Total:- " << total_;
        output.close(); // Close the file after writing
    }

    void saveExpensesToFile()
    {
        ofstream output("expenses.txt", ios::app);
        for (const auto &[category, amount] : expenses_)
        {
            output << category << "  " << amount << endl;
        }
    }

    void ExpensesFromFile() override
    {
        ifstream input("expenses.txt");
        string category;
        float amount;

        while (input >> category >> amount)
        {
            expenses_[category] += amount;
            total_ += amount;
        }
    }
};

class sample
{
public:
    virtual void signup(const string &username, const string &password) = 0;
    virtual void logout() = 0;
    virtual void addExpense(const string &category, float amount) = 0;
    virtual void viewExpenses() = 0;
};

class finance : public sample
{
public:
    unique_ptr<IUser> user_;

    void signup(const string &username, const string &password) override
    {
        ofstream output(DATA_FILE, ios::app);
        output << "Username:-" << username << endl
               << "Password:-" << password << endl;
        user_ = make_unique<User>(username, password);
        output.close();
    }

    void logout() override
    {
        user_.reset();
    }

    void addExpense(const string &category, float amount) override
    {
        if (user_)
        {
            user_->addExpense(category, amount);
        }
        else
        {
            cout << "Error: Not signed in" << endl;
        }
    }

    void viewExpenses() override
    {
        if (user_)
        {
            user_->viewExpenses();
        }
        else
        {
            cout << "Error: Not signed in" << endl;
        }
    }
};

int main()
{
    cout << endl
         << "\t\t---------Welcome to the Expense Management System!----------" << endl;

    auto expenses = make_unique<finance>();

    while (true)
    {
        cout << endl;
        cout << "Please choose an option:" << endl;
        cout << "1. Signup" << endl;
        cout << "2. Logout" << endl;
        cout << "3. Add expenses" << endl;
        cout << "4. View expenses" << endl;
        cout << "5. Exit" << endl;

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter a new username and password: ";
            string username, password;
            cin >> username >> password;

            expenses->signup(username, password);
            cout << endl
                 << "Successfully signed up" << endl;
        }

        else if (choice == 2)
        {
            expenses->logout();
            cout << "Successfully logged out" << endl;
        }

        else if (choice == 3)
        {
            cout << "Enter a category and amount: ";
            string category;
            float amount;
            cin >> category >> amount;

            expenses->addExpense(category, amount);
            cout << endl
                 << "Expense added" << endl;
        }

        else if (choice == 4)
        {
            expenses->viewExpenses();
        }

        else if (choice == 5)
        {
            break;
        }

        else
        {
            cout << endl
                 << "Invalid choice" << endl;
        }
    }

    return 0;
}
