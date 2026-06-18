#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

// ==========================================
// 1. DATA STRUCTURE STRUCTS
// ==========================================

struct Date {
    int day;
    int month;
    int year;
};

struct Address {
    string street;
    string city;
};

// ==========================================
// 2. OOP CLASSES (INHERITANCE)
// ==========================================

// BASE CLASS: Order
class Order {
protected:
    int orderID;
    double totalPrice;
    Date orderDate;

public:
    Order(int id, double price, Date d) {
        orderID = id;
        totalPrice = price;
        orderDate = d;
    }
    
    virtual ~Order() {}

    virtual void displayOrder() {
        cout << "Order ID: " << orderID << " | Price: RM" << totalPrice << endl;
    }
    
    int getOrderID() { return orderID; }
    double getTotalPrice() { return totalPrice; }
    Date getOrderDate() { return orderDate; }
};

// DERIVED CLASS: DeliveryOrder
class DeliveryOrder : public Order {
private:
    int deliveryTimeMinutes;
    string riderName;
    Address deliveryAddress;
    string orderStatus; 
    bool isVIP; // NEW VIP FEATURE: Tracks if the order is priority

public:
    // NEW VIP FEATURE: Added 'bool vipStat' to the constructor
    DeliveryOrder(int id, double price, Date d, int time, string rider, Address addr, string status, bool vipStat) 
        : Order(id, price, d) {
        deliveryTimeMinutes = time;
        riderName = rider;
        deliveryAddress = addr;
        orderStatus = status; 
        isVIP = vipStat; 
    }

    // Overridden Function 
    void displayOrder() override {
        cout << "------------------------------------------------------\n";
        
        // NEW VIP FEATURE: Dynamic Display Tag
        if (isVIP) {
            cout << "Order ID: " << orderID << " *** [VIP PRIORITY] *** [" << orderStatus << "]\n";
        } else {
            cout << "Order ID: " << orderID << " [" << orderStatus << "]\n";
        }

        cout << "Price   : RM" << totalPrice << " | Est. Time: " << deliveryTimeMinutes << " mins\n";
        cout << "Rider   : " << riderName << "\n";
        cout << "Address : " << deliveryAddress.street << ", " << deliveryAddress.city << "\n";
        cout << "Date    : " << orderDate.day << "/" << orderDate.month << "/" << orderDate.year << "\n";
        cout << "------------------------------------------------------\n";
    }

    int getDeliveryTime() { return deliveryTimeMinutes; }
    string getRiderName() { return riderName; }
    Address getDeliveryAddress() { return deliveryAddress; }
    string getOrderStatus() { return orderStatus; }
    bool getIsVIP() { return isVIP; } // NEW VIP FEATURE: Getter

    void setOrderStatus(string newStatus) { orderStatus = newStatus; }
    void setRiderName(string newRider) { riderName = newRider; }
    void setAddress(string newStreet, string newCity) { 
        deliveryAddress.street = newStreet; 
        deliveryAddress.city = newCity; 
    }
};

// ==========================================
// 3. CUSTOM NODES & DATA STRUCTURE HEADERS
// ==========================================

struct OrderNode {
    DeliveryOrder* data; 
    OrderNode* next;     
    OrderNode* prev;     
};

class ActiveOrderList {
private:
    OrderNode* head;
    OrderNode* tail;

public:
    ActiveOrderList() { head = NULL; tail = NULL; }
    ~ActiveOrderList(); 

    bool isIDExists(int id); 
    void addOrder(DeliveryOrder* newOrder);
    void addPriorityOrder(DeliveryOrder* newOrder); 
    void displayAllOrders();
    DeliveryOrder* deleteOrder(int id); 
    
    void editOrder(int id); 
    void filterByStatus(string targetStatus); 
    void printReceipt(int id); 
    void generateDashboard(); 
    
    void saveToFile();
    void loadFromFile();
};

class UndoStack {
private:
    OrderNode* top;

public:
    UndoStack() { top = NULL; }
    ~UndoStack();

    void push(DeliveryOrder* deletedOrder);
    DeliveryOrder* pop(); 
    void viewHistory(); 
};

// ==========================================
// 4. DOUBLY LINKED LIST IMPLEMENTATION
// ==========================================

ActiveOrderList::~ActiveOrderList() {
    OrderNode* current = head;
    while (current != NULL) {
        OrderNode* nextNode = current->next;
        delete current->data; 
        delete current;       
        current = nextNode;
    }
}

bool ActiveOrderList::isIDExists(int id) {
    OrderNode* temp = head;
    while (temp != NULL) {
        if (temp->data->getOrderID() == id) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void ActiveOrderList::addOrder(DeliveryOrder* newOrder) {
    if (isIDExists(newOrder->getOrderID())) {
        cout << "[!] Error: Order ID " << newOrder->getOrderID() << " already exists in the system!\n";
        delete newOrder; 
        return;
    }

    OrderNode* newNode = new OrderNode;
    newNode->data = newOrder;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode; 
    }
    cout << "[System] Standard Order added to active list successfully.\n";
}

void ActiveOrderList::addPriorityOrder(DeliveryOrder* newOrder) {
    if (isIDExists(newOrder->getOrderID())) {
        cout << "[!] Error: Order ID " << newOrder->getOrderID() << " already exists in the system!\n";
        delete newOrder; 
        return;
    }

    OrderNode* newNode = new OrderNode;
    newNode->data = newOrder;
    newNode->prev = NULL;

    if (head == NULL) {
        newNode->next = NULL;
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode; 
    }
    cout << "[System] VIP Priority Order added to the front of the queue!\n";
}

void ActiveOrderList::displayAllOrders() {
    if (head == NULL) {
        cout << "[System] There are no active orders right now.\n";
        return;
    }

    OrderNode* temp = head;
    cout << "\n========== LIVE ACTIVE ORDERS ==========\n";
    while (temp != NULL) {
        temp->data->displayOrder(); 
        temp = temp->next;
    }
    cout << "==========================================\n";
}

void ActiveOrderList::editOrder(int id) {
    OrderNode* temp = head;
    while (temp != NULL && temp->data->getOrderID() != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "[!] Error: Order ID " << id << " not found for editing.\n";
        return;
    }

    int editChoice = 0;
    while (editChoice != 4) {
        cout << "\n--- EDITING ORDER ID: " << id << " ---\n";
        cout << "1. Update Status (Currently: " << temp->data->getOrderStatus() << ")\n";
        cout << "2. Reassign Rider (Currently: " << temp->data->getRiderName() << ")\n";
        cout << "3. Update Delivery Address\n";
        cout << "4. Done Editing\n";
        cout << "Select field to edit: ";
        cin >> editChoice;

        if (editChoice == 1) {
            string newStat;
            cout << "Enter new status (e.g., Pending, Out for Delivery, Delivered): ";
            cin.ignore(10000, '\n'); 
            getline(cin, newStat);
            temp->data->setOrderStatus(newStat);
            cout << "[System] Status updated!\n";
        } else if (editChoice == 2) {
            string newRider;
            cout << "Enter new Rider Name: ";
            cin.ignore(10000, '\n'); 
            getline(cin, newRider);
            temp->data->setRiderName(newRider);
            cout << "[System] Rider updated!\n";
        } else if (editChoice == 3) {
            string newSt, newCity;
            cout << "Enter new Street: ";
            cin.ignore(10000, '\n'); 
            getline(cin, newSt);
            cout << "Enter new City: ";
            getline(cin, newCity);
            temp->data->setAddress(newSt, newCity);
            cout << "[System] Address updated!\n";
        }
    }
}

void ActiveOrderList::filterByStatus(string targetStatus) {
    OrderNode* temp = head;
    bool found = false;
    cout << "\n--- FILTERING BY STATUS: " << targetStatus << " ---\n";
    while (temp != NULL) {
        if (temp->data->getOrderStatus() == targetStatus) {
            temp->data->displayOrder();
            found = true;
        }
        temp = temp->next;
    }
    if (!found) cout << "[System] No orders found with that status.\n";
}

void ActiveOrderList::generateDashboard() {
    if (head == NULL) {
        cout << "[System] Dashboard unavailable. No active orders.\n";
        return;
    }

    OrderNode* temp = head;
    int totalOrders = 0;
    int pendingCount = 0;
    int deliveredCount = 0;
    int vipCount = 0; // NEW VIP FEATURE: Dashboard tracking
    double totalRevenue = 0.0;
    int totalDeliveryTime = 0;

    while (temp != NULL) {
        totalOrders++;
        totalRevenue += temp->data->getTotalPrice();
        totalDeliveryTime += temp->data->getDeliveryTime();

        if (temp->data->getIsVIP()) vipCount++; // Track VIPs

        string status = temp->data->getOrderStatus();
        if (status == "Pending") pendingCount++;
        else if (status == "Delivered") deliveredCount++;

        temp = temp->next;
    }

    double avgTime = (double)totalDeliveryTime / totalOrders;

    cout << "\n============================================\n";
    cout << "       BLOSSOM POS LIVE DASHBOARD           \n";
    cout << "============================================\n";
    cout << "Total Active Orders : " << totalOrders << "\n";
    cout << "Total VIP Orders    : " << vipCount << "\n"; // NEW VIP FEATURE
    cout << "Projected Revenue   : RM " << totalRevenue << "\n";
    cout << "Avg. Delivery Time  : " << avgTime << " mins\n";
    cout << "--------------------------------------------\n";
    cout << "STATUS BREAKDOWN:\n";
    cout << " [ Pending ]        : " << pendingCount << "\n";
    cout << " [ Delivered ]      : " << deliveredCount << "\n";
    cout << "============================================\n";
}

void ActiveOrderList::printReceipt(int id) {
    OrderNode* temp = head;
    while (temp != NULL && temp->data->getOrderID() != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "[!] Error: Order ID " << id << " not found.\n";
        return;
    }

    string filename = "Receipt_" + to_string(id) + ".txt";
    ofstream receiptFile(filename);

    if (receiptFile.is_open()) {
        receiptFile << "***********************************\n";
        receiptFile << "      OFFICIAL ORDER RECEIPT       \n";
        receiptFile << "***********************************\n";
        
        // NEW VIP FEATURE: Custom receipt header
        if (temp->data->getIsVIP()) {
            receiptFile << "      *** VIP PRIORITY *** \n";
            receiptFile << "-----------------------------------\n";
        }
        
        receiptFile << "Order ID : " << temp->data->getOrderID() << "\n";
        receiptFile << "Date     : " << temp->data->getOrderDate().day << "/" 
                    << temp->data->getOrderDate().month << "/" << temp->data->getOrderDate().year << "\n";
        receiptFile << "Status   : " << temp->data->getOrderStatus() << "\n";
        receiptFile << "-----------------------------------\n";
        receiptFile << "Total Due: RM" << temp->data->getTotalPrice() << "\n";
        receiptFile << "Est. Time: " << temp->data->getDeliveryTime() << " mins\n";
        receiptFile << "Rider    : " << temp->data->getRiderName() << "\n";
        receiptFile << "Deliver To:\n";
        receiptFile << temp->data->getDeliveryAddress().street << "\n";
        receiptFile << temp->data->getDeliveryAddress().city << "\n";
        receiptFile << "***********************************\n";
        receiptFile << "    Thank you for your order!      \n";
        receiptFile.close();
        cout << "[System] Receipt generated successfully: " << filename << "\n";
    }
}

DeliveryOrder* ActiveOrderList::deleteOrder(int id) {
    if (head == NULL) return NULL; 

    OrderNode* temp = head;

    while (temp != NULL && temp->data->getOrderID() != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "[!] Error: Order ID " << id << " not found.\n";
        return NULL;
    }

    DeliveryOrder* savedOrder = temp->data;

    if (temp == head) {
        head = temp->next;
        if (head != NULL) head->prev = NULL;
    } else if (temp == tail) {
        tail = temp->prev;
        if (tail != NULL) tail->next = NULL;
    } else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    delete temp; 
    cout << "[System] Order " << id << " temporarily removed.\n";
    return savedOrder; 
}

// NEW VIP FEATURE: Saving the boolean status to the file
void ActiveOrderList::saveToFile() {
    ofstream outFile("Orders.txt"); 
    if (!outFile) return;

    OrderNode* temp = head;
    int count = 0;
    while (temp != NULL) {
        DeliveryOrder* order = temp->data;
        outFile << order->getOrderID() << endl;
        outFile << order->getIsVIP() << endl; // Saved as 1 (True) or 0 (False)
        outFile << order->getTotalPrice() << endl;
        outFile << order->getOrderDate().day << " " << order->getOrderDate().month << " " << order->getOrderDate().year << endl;
        outFile << order->getDeliveryTime() << endl;
        outFile << order->getOrderStatus() << endl; 
        outFile << order->getRiderName() << endl;
        outFile << order->getDeliveryAddress().street << endl;
        outFile << order->getDeliveryAddress().city << endl;
        
        temp = temp->next;
        count++;
    }
    outFile.close(); 
    cout << "[System] Successfully saved " << count << " active orders to Orders.txt.\n";
}

// NEW VIP FEATURE: Loading the boolean status from the file
void ActiveOrderList::loadFromFile() {
    ifstream inFile("Orders.txt"); 
    if (!inFile) return;

    int id, time, day, month, year;
    double price;
    bool vipStat;
    string status, rider, street, city;
    int count = 0;

    while (inFile >> id) {
        inFile >> vipStat; // Read VIP status first
        inFile >> price;
        inFile >> day >> month >> year;
        inFile >> time;
        inFile >> ws; 
        getline(inFile, status);
        getline(inFile, rider);
        getline(inFile, street);
        getline(inFile, city);

        Date d = {day, month, year};
        Address a = {street, city};

        DeliveryOrder* recoveredOrder = new DeliveryOrder(id, price, d, time, rider, a, status, vipStat);
        
        OrderNode* newNode = new OrderNode;
        newNode->data = recoveredOrder;
        newNode->next = NULL;
        newNode->prev = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode; 
        }
        count++;
    }
    inFile.close();
    cout << "[System] Successfully loaded " << count << " orders from Orders.txt.\n";
}

// ==========================================
// 5. LINKED STACK IMPLEMENTATION
// ==========================================

UndoStack::~UndoStack() {
    OrderNode* current = top;
    while (current != NULL) {
        OrderNode* nextNode = current->next;
        delete current->data; 
        delete current;
        current = nextNode;
    }
}

void UndoStack::push(DeliveryOrder* deletedOrder) {
    OrderNode* newNode = new OrderNode;
    newNode->data = deletedOrder;
    newNode->next = top; 
    newNode->prev = NULL; 
    top = newNode; 
    cout << "[Stack] Order moved to Undo history (Recycle Bin).\n";
}

DeliveryOrder* UndoStack::pop() {
    if (top == NULL) {
        cout << "[!] No deleted orders to undo.\n";
        return NULL;
    }
    OrderNode* temp = top;
    DeliveryOrder* recoveredOrder = temp->data; 
    top = top->next; 
    delete temp; 
    cout << "[Stack] Order successfully recovered!\n";
    return recoveredOrder;
}

void UndoStack::viewHistory() {
    if (top == NULL) {
        cout << "[Stack] The Recycle Bin is empty.\n";
        return;
    }
    OrderNode* temp = top;
    cout << "\n--- RECYCLE BIN HISTORY (LIFO) ---\n";
    int count = 1;
    while (temp != NULL) {
        cout << count << ". Order ID: " << temp->data->getOrderID() 
             << " | Deleted Status: " << temp->data->getOrderStatus();
        
        if (temp->data->getIsVIP()) cout << " [VIP]"; // NEW VIP FEATURE
        cout << "\n";
             
        temp = temp->next;
        count++;
    }
    cout << "----------------------------------\n";
}

// ==========================================
// 6. MAIN FUNCTION (INTERACTIVE MENU)
// ==========================================

int main() {
    ActiveOrderList activeList;
    UndoStack undoHistory;

    cout << "\n============================================\n";
    cout << "   BLOSSOM POS: ORDER MANAGEMENT SYSTEM     \n";
    cout << "============================================\n";
    activeList.loadFromFile(); 

    int choice = 0;

    while (choice != 11) {
        cout << "\n[ MAIN MENU ]\n";
        cout << "1. Add a Standard Order\n";
        cout << "2. Add a VIP Priority Order\n";
        cout << "3. Display All Active Orders\n";
        cout << "4. Live Business Dashboard\n";
        cout << "5. Edit an Order\n";
        cout << "6. Filter Orders by Status\n";
        cout << "7. Print Individual Receipt\n";
        cout << "8. Delete an Order\n";
        cout << "9. View Undo History (Recycle Bin)\n";
        cout << "10. Undo Last Deletion\n";
        cout << "11. Save and Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            int id, time, d, m, y;
            double price;
            string rider, street, city;

            cout << "Enter Order ID (e.g. 1001): ";
            cin >> id;
            if (activeList.isIDExists(id)) {
                cout << "[!] Error: Order ID already exists. Try again.\n";
                continue;
            }

            cout << "Enter Total Price (e.g. 25.50): ";
            cin >> price;
            cout << "Enter Date (DD MM YYYY): ";
            cin >> d >> m >> y;
            cout << "Enter Est. Delivery Time (mins): ";
            cin >> time;
            
            cin.ignore(10000, '\n'); 
            
            cout << "Enter Rider Name: ";
            getline(cin, rider);
            cout << "Enter Street Address: ";
            getline(cin, street);
            cout << "Enter City: ";
            getline(cin, city);

            Date dateStruct = {d, m, y};
            Address addrStruct = {street, city};

            // NEW VIP FEATURE: Logic to assign True/False based on user choice
            bool isVipOrder = (choice == 2); 

            DeliveryOrder* newOrder = new DeliveryOrder(id, price, dateStruct, time, rider, addrStruct, "Pending", isVipOrder);
            
            if (choice == 1) {
                activeList.addOrder(newOrder);
            } else {
                activeList.addPriorityOrder(newOrder);
            }

        } else if (choice == 3) {
            activeList.displayAllOrders();

        } else if (choice == 4) {
            activeList.generateDashboard();

        } else if (choice == 5) {
            int editID;
            cout << "Enter Order ID to edit: ";
            cin >> editID;
            activeList.editOrder(editID);

        } else if (choice == 6) {
            string input;
            cout << "Enter status (p=Pending, d=Delivered, o=Out for Delivery, c=Cancelled): ";
            cin.ignore(10000, '\n'); 
            getline(cin, input);
            
            string finalStatus = input;
            if (input == "p" || input == "P") finalStatus = "Pending";
            else if (input == "d" || input == "D") finalStatus = "Delivered";
            else if (input == "o" || input == "O") finalStatus = "Out for Delivery";
            else if (input == "c" || input == "C") finalStatus = "Cancelled";

            activeList.filterByStatus(finalStatus);

        } else if (choice == 7) {
            int receiptID;
            cout << "Enter Order ID to generate receipt: ";
            cin >> receiptID;
            activeList.printReceipt(receiptID);

        } else if (choice == 8) {
            int deleteID;
            cout << "Enter Order ID to delete: ";
            cin >> deleteID;
            DeliveryOrder* removed = activeList.deleteOrder(deleteID);
            if (removed != NULL) {
                undoHistory.push(removed);
            }

        } else if (choice == 9) {
            undoHistory.viewHistory();

        } else if (choice == 10) {
            DeliveryOrder* recovered = undoHistory.pop();
            if (recovered != NULL) {
                // If it is a VIP, we make sure we restore it to the FRONT of the line
                if (recovered->getIsVIP()) {
                    activeList.addPriorityOrder(recovered);
                } else {
                    activeList.addOrder(recovered);
                }
            }

        } else if (choice == 11) {
            activeList.saveToFile();
            cout << "Exiting system. Data successfully saved. Goodbye!\n";
        } else {
            cout << "[!] Invalid choice.\n";
        }
    }

    return 0;
}
