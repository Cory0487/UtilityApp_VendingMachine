#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

using namespace std;

struct Item {
    string code;
    string name;
    int pricePence;
    int stock;
};

struct BasketLine {
    int itemIndex;
    int quantity;
};

string formatGBP(int pence) {
    int pounds = pence / 100;
    int pennies = abs(pence % 100);
    ostringstream out;
    out << "£" << pounds << "." << setw(2) << setfill('0') << pennies;
    return out.str();
}

int readInt(string message) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value) return value;
        cout << "Invalid input. Enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readString(string message) {
    cout << message;
    string input;
    cin >> input;
    return input;
}

int findItem(vector<Item>& items, string code) {
    for (int i = 0; i < items.size(); i++) {
        if (items[i].code == code) return i;
    }
    return -1;
}

void showItems(vector<Item>& items) {
    cout << "\n--- ITEMS ---\n";
    for (auto& item : items) {
        cout << item.code << " | "
             << item.name << " | "
             << formatGBP(item.pricePence)
             << " | Stock: " << item.stock << "\n";
    }
}

void showBasket(vector<Item>& items, vector<BasketLine>& basket) {
    int total = 0;
    cout << "\n--- BASKET ---\n";
    for (auto& line : basket) {
        int cost = items[line.itemIndex].pricePence * line.quantity;
        total += cost;
        cout << items[line.itemIndex].name
             << " x" << line.quantity
             << " = " << formatGBP(cost) << "\n";
    }
    cout << "TOTAL: " << formatGBP(total) << "\n";
}

void addItem(vector<Item>& items, vector<BasketLine>& basket) {
    showItems(items);
    string code = readString("Enter item code: ");
    int index = findItem(items, code);

    if (index == -1 || items[index].stock <= 0) {
        cout << "Invalid or out of stock.\n";
        return;
    }

    int qty = readInt("Enter quantity: ");
    if (qty <= 0 || qty > items[index].stock) {
        cout << "Invalid quantity.\n";
        return;
    }

    basket.push_back({index, qty});
    cout << "Added to basket.\n";
}

void checkout(vector<Item>& items, vector<BasketLine>& basket) {
    int total = 0;
    for (auto& line : basket)
        total += items[line.itemIndex].pricePence * line.quantity;

    int paid = 2000; // £20
    cout << "Paying with £20\n";

    if (paid < total) {
        cout << "Not enough money.\n";
        return;
    }

    int change = paid - total;

    cout << "\n--- DISPENSING ---\n";
    for (auto& line : basket) {
        items[line.itemIndex].stock -= line.quantity;
        cout << "Dispensed: "
             << items[line.itemIndex].name
             << " x" << line.quantity << "\n";
    }

    cout << "Change returned: " << formatGBP(change) << "\n";
    basket.clear();
}

int main() {
    vector<Item> items = {
        {"A1", "Chips", 175, 10},
        {"A2", "Coffee Espresso", 200, 10},
        {"A3", "Chocolate Bueno", 202, 10},
        {"A4", "Coca Cola Zero Sugar", 120, 10},
        {"A5", "Water", 100, 10},
        {"A6", "M&S", 130, 10}
    };

    vector<BasketLine> basket;
    int choice;

    do {
        cout << "\n1. Show items\n2. Add item\n3. View basket\n4. Checkout (£20)\n5. Exit\n";
        choice = readInt("Choose: ");

        if (choice == 1) showItems(items);
        else if (choice == 2) addItem(items, basket);
        else if (choice == 3) showBasket(items, basket);
        else if (choice == 4) checkout(items, basket);

    } while (choice != 5);

    return 0;
}
