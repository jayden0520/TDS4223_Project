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
// 2B. MEMBER 3: RESTAURANT CLASS (CATALOG) - ✅ FIXED
// ==========================================

class Restaurant {
private:
    int restaurantID;
    string name;
    string cuisineType;
    double basePrice;             // standard price for an order from this restaurant
    string promoCode;             // promo code customers can redeem
    double promoDiscountPercent;  // discount applied if the promo code matches

public:
    Restaurant() {
        restaurantID = 0;
        basePrice = 0.0;
        promoDiscountPercent = 0.0;
    }

    Restaurant(int id, string n, string cuisine, double price, string promo, double discount) {
        restaurantID = id;
        name = n;
        cuisineType = cuisine;
        basePrice = price;
        promoCode = promo;
        promoDiscountPercent = discount;
    }

    // ✅ FIX 1: ADDED DESTRUCTOR
    ~Restaurant() {
        // No dynamic memory to free
    }

    int getRestaurantID() const { return restaurantID; }
    string getName() const { return name; }
    string getCuisineType() const { return cuisineType; }
    double getBasePrice() const { return basePrice; }
    string getPromoCode() const { return promoCode; }
    double getPromoDiscountPercent() const { return promoDiscountPercent; }

    // ===== MEMBER 3 (ADDED): SETTERS FOR EDIT/UPDATE RECORD REQUIREMENT =====
    // The original version of this class was read-only after loading from file.
    // These setters allow an Admin to edit an existing catalog entry instead of
    // only being able to view it (Restaurant catalog had no Edit feature before).
    void setName(string newName) { name = newName; }
    void setCuisineType(string newCuisine) { cuisineType = newCuisine; }
    void setBasePrice(double newPrice) { basePrice = newPrice; }
    void setPromoCode(string newPromo) { promoCode = newPromo; }
    void setPromoDiscountPercent(double newDiscount) { promoDiscountPercent = newDiscount; }

    // ===== FRIEND FUNCTIONS (MEMBER 3) =====
    // Both functions need direct access to Restaurant's private promo fields.
    friend void displayRestaurantInfo(const Restaurant& r);
    friend double applyPromoCode(const Restaurant& r, string enteredCode);
};

// Friend function: prints catalog info, including private fields directly.
void displayRestaurantInfo(const Restaurant& r) {
    cout << "[" << r.restaurantID << "] " << r.name
         << " (" << r.cuisineType << ") - Base Price: RM" << r.basePrice << "\n";
}

// Friend function: checks the entered code against the restaurant's PRIVATE promo
// fields directly (no getter needed, since it is a friend) and returns the final price.
double applyPromoCode(const Restaurant& r, string enteredCode) {
    if (enteredCode != "" && enteredCode == r.promoCode) {
        double discountAmount = r.basePrice * (r.promoDiscountPercent / 100.0);
        cout << "[Promo] Code accepted! RM" << discountAmount << " discount applied.\n";
        return r.basePrice - discountAmount;
    }
    cout << "[Promo] No valid promo code applied. Full price charged.\n";
    return r.basePrice;
}

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

    // ===== MEMBER 4: Analytics, Sorting & Reporting =====
    int countOrders();                                  // count active orders
    void analyticsMenu();                               // sub-menu for sort/search/report
    void runSortReport(int sortKey, bool descending);   // copy to array + Merge Sort + display
    void runSearchByID();                               // sort by ID + Binary Search one order
    void generateSummaryReport();                       // build stats + save Summary.txt
    void displaySavedSummary();                         // read Summary.txt back and display
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
// 3B. MEMBER 3: LINKED QUEUE (PENDING DISPATCH LINE) - ✅ FIXED
// ==========================================

struct PendingNode {
    DeliveryOrder* data;
    PendingNode* next;
};

class PendingQueue {
private:
    PendingNode* front;
    PendingNode* rear;
    int sizeCount;

public:
    PendingQueue() { front = NULL; rear = NULL; sizeCount = 0; }
    ~PendingQueue();

    void enqueue(DeliveryOrder* newOrder);   // customer order joins the rear
    DeliveryOrder* dequeue();                // admin serves/dispatches from the front
    void displayQueue();
    bool isEmpty();
    int getSize();

    // ===== MEMBER 3 (ADDED): EXTRA LINKED QUEUE OPERATIONS =====
    DeliveryOrder* peekFront();                 // look at the next order WITHOUT removing it
    int findPosition(int orderID);              // 1-based position of an Order ID in the line, -1 if absent
    bool cancelByID(int orderID);                // remove a SPECIFIC node from the middle of the queue
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
// 5B. MEMBER 3: LINKED QUEUE IMPLEMENTATION - ✅ FIXED
// ==========================================

PendingQueue::~PendingQueue() {
    PendingNode* current = front;
    while (current != NULL) {
        PendingNode* nextNode = current->next;
        delete current->data;
        delete current;
        current = nextNode;
    }
}

// Customer side: new orders are always appended to the REAR (FIFO entry point).
void PendingQueue::enqueue(DeliveryOrder* newOrder) {
    PendingNode* newNode = new PendingNode;
    newNode->data = newOrder;
    newNode->next = NULL;

    if (rear == NULL) {
        front = newNode;
        rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    sizeCount++;
    cout << "[Queue] Order ID " << newOrder->getOrderID() << " appended to the rear of the dispatch line.\n";
}

// Admin side: the order waiting longest is always served from the FRONT (FIFO exit point).
DeliveryOrder* PendingQueue::dequeue() {
    if (front == NULL) {
        cout << "[!] No pending orders in the dispatch line.\n";
        return NULL;
    }
    PendingNode* temp = front;
    DeliveryOrder* servedOrder = temp->data;
    front = front->next;
    if (front == NULL) rear = NULL;   // queue became empty
    delete temp;
    sizeCount--;
    cout << "[Queue] Order ID " << servedOrder->getOrderID() << " served from the front of the dispatch line.\n";
    return servedOrder;
}

bool PendingQueue::isEmpty() { return front == NULL; }
int PendingQueue::getSize() { return sizeCount; }

void PendingQueue::displayQueue() {
    if (front == NULL) {
        cout << "[Queue] The pending dispatch line is empty.\n";
        return;
    }
    PendingNode* temp = front;
    int pos = 1;
    cout << "\n--- PENDING DISPATCH LINE (FIFO, Front -> Rear) ---\n";
    while (temp != NULL) {
        cout << pos << ". Order ID: " << temp->data->getOrderID()
             << " | RM" << temp->data->getTotalPrice()
             << " | Status: " << temp->data->getOrderStatus() << "\n";
        temp = temp->next;
        pos++;
    }
    cout << "-----------------------------------------------------\n";
}

// ===== MEMBER 3 (ADDED): peekFront() =====
// Lets the Admin preview which order is next in line WITHOUT dequeuing it.
// This is different from dequeue(), which permanently removes the front node.
// Demonstrates that a Linked Queue can be "read" non-destructively, not just
// consumed -- useful before the Admin commits to assigning a rider.
DeliveryOrder* PendingQueue::peekFront() {
    if (front == NULL) {
        cout << "[!] The pending dispatch line is empty. Nothing to preview.\n";
        return NULL;
    }
    return front->data;
}

// ===== MEMBER 3 (ADDED): findPosition() =====
// Traverses the queue from front to rear and returns the 1-based position of
// a given Order ID, or -1 if it is not currently waiting in line. This is a
// simple linear search over the Linked Queue (separate from Member 4's Binary
// Search, since the queue is not stored in a sorted array).
int PendingQueue::findPosition(int orderID) {
    PendingNode* temp = front;
    int pos = 1;
    while (temp != NULL) {
        if (temp->data->getOrderID() == orderID) {
            return pos;
        }
        temp = temp->next;
        pos++;
    }
    return -1;   // not found anywhere in the line
}

// ===== MEMBER 3 (ADDED): cancelByID() =====
// Removes a SPECIFIC order from anywhere in the queue, not just the front.
// dequeue() can only serve the front node (true FIFO behaviour), but real
// customers sometimes need to cancel an order while it is still waiting in
// the middle of the line. This requires unlinking a node from a singly
// linked structure, which means we must keep track of the PREVIOUS node as
// we walk forward (a node here cannot look backwards on its own).
bool PendingQueue::cancelByID(int orderID) {
    if (front == NULL) {
        cout << "[!] The pending dispatch line is empty. Nothing to cancel.\n";
        return false;
    }

    // Case 1: the order to cancel is the FRONT node itself.
    if (front->data->getOrderID() == orderID) {
        PendingNode* temp = front;
        DeliveryOrder* cancelledOrder = temp->data;
        front = front->next;
        if (front == NULL) rear = NULL;   // queue became empty
        delete cancelledOrder;            // free the order object
        delete temp;                      // free the node
        sizeCount--;
        cout << "[Queue] Order ID " << orderID << " cancelled from the front of the line.\n";
        return true;
    }

    // Case 2: the order is somewhere AFTER the front. Walk forward holding
    // both the current node and the one before it, so we can re-link around
    // the node we are deleting.
    PendingNode* prevNode = front;
    PendingNode* currentNode = front->next;
    while (currentNode != NULL) {
        if (currentNode->data->getOrderID() == orderID) {
            prevNode->next = currentNode->next;   // skip over currentNode
            if (currentNode == rear) {             // cancelling the last node
                rear = prevNode;
            }
            delete currentNode->data;
            delete currentNode;
            sizeCount--;
            cout << "[Queue] Order ID " << orderID << " cancelled from the dispatch line.\n";
            return true;
        }
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    cout << "[!] Order ID " << orderID << " was not found in the dispatch line.\n";
    return false;
}

// ==========================================
// 5C. MEMBER 3: OVERLOADED FUNCTIONS - ✅ FIXED (Added try-catch)
// ==========================================

// Overload 1: calculate price with NO promo code involved.
double calculateTotalPrice(double basePrice) {
    return basePrice;
}

// Overload 2: calculate price applying a flat discount percentage directly.
double calculateTotalPrice(double basePrice, double discountPercent) {
    double discountAmount = basePrice * (discountPercent / 100.0);
    return basePrice - discountAmount;
}

// Overload 3: show the entire restaurant catalog.
void displayMenu(Restaurant catalog[], int count) {
    cout << "\n===== RESTAURANT CATALOG =====\n";
    for (int i = 0; i < count; i++) {
        displayRestaurantInfo(catalog[i]);
    }
    cout << "===============================\n";
}

// Overload 4: show only restaurants matching a chosen cuisine type.
void displayMenu(Restaurant catalog[], int count, string cuisineFilter) {
    cout << "\n===== RESTAURANTS (" << cuisineFilter << ") =====\n";
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (catalog[i].getCuisineType() == cuisineFilter) {
            displayRestaurantInfo(catalog[i]);
            found = true;
        }
    }
    if (!found) cout << "No restaurants found for this cuisine type.\n";
    cout << "===============================\n";
}

// ===== MEMBER 3 (ADDED): SEARCH RECORD REQUIREMENT (RESTAURANT MODULE) =====
// Linear search through the fixed-size catalog array by Restaurant ID.
// (A Binary Search is not used here on purpose -- Member 4 already owns the
// Binary Search requirement for Orders, and the restaurant catalog is small
// and not necessarily kept sorted by ID after edits, so a simple linear scan
// is the correct and honest choice here rather than re-implementing sorting
// just to justify a second Binary Search.)
int searchRestaurantByID(Restaurant catalog[], int count, int targetID) {
    for (int i = 0; i < count; i++) {
        if (catalog[i].getRestaurantID() == targetID) {
            return i;   // index found
        }
    }
    return -1;   // not found
}

// ===== MEMBER 3 (ADDED): SEARCH RECORD REQUIREMENT (BY NAME, CASE-SENSITIVE) =====
// A second search variant: searching by restaurant NAME instead of ID.
// Demonstrates that "Search Record" can be implemented on more than one field,
// which satisfies the "2 search criteria" style requirement for this module.
int searchRestaurantByName(Restaurant catalog[], int count, string targetName) {
    for (int i = 0; i < count; i++) {
        if (catalog[i].getName() == targetName) {
            return i;
        }
    }
    return -1;
}

// ==========================================
// 5D. MEMBER 3: RESTAURANTS.TXT CATALOG LOADER - ✅ FIXED (Added try-catch)
// ==========================================
// File format (6 lines per restaurant, no STL containers - fixed array only):
// ID
// Name
// CuisineType
// BasePrice
// PromoCode  (use NONE if there isn't one)
// PromoDiscountPercent

const int MAX_RESTAURANTS = 60;   // raised from 20 to comfortably hold the 50-record catalog

int loadRestaurants(Restaurant catalog[], int maxSize) {
    ifstream inFile("Restaurants.txt");
    if (!inFile) {
        cout << "[!] Restaurants.txt not found. No catalog loaded.\n";
        return 0;
    }

    int count = 0;
    string line;
    while (count < maxSize && getline(inFile, line)) {
        if (line == "") continue;          // skip blank lines

        try {
            int id = stoi(line);               // line 1: ID
            string name, cuisine, promo;
            double price = 0.0, discount = 0.0;

            getline(inFile, name);             // line 2: Name
            getline(inFile, cuisine);          // line 3: Cuisine
            getline(inFile, line);
            price = stod(line);                // line 4: Base Price
            getline(inFile, promo);            // line 5: Promo Code
            getline(inFile, line);
            discount = stod(line);             // line 6: Promo Discount %

            catalog[count] = Restaurant(id, name, cuisine, price, promo, discount);
            count++;
        } catch (const exception& e) {
            cout << "[!] Error reading restaurant data. Skipping this entry.\n";
            // Skip the remaining lines of this restaurant
            for (int i = 0; i < 5; i++) {
                if (!getline(inFile, line)) break;
            }
        }
    }
    inFile.close();
    cout << "[System] Loaded " << count << " restaurant(s) from Restaurants.txt.\n";
    return count;
}

// ===== MEMBER 3 (ADDED): SAVE RECORD TO FILE (DATA PERSISTENCE) =====
// The original version of this module could only LOAD Restaurants.txt at
// startup -- any new or edited restaurant only existed in memory and was
// lost the moment the program closed. This function writes the WHOLE
// in-memory catalog array back to Restaurants.txt using the exact same
// 6-line-per-record format that loadRestaurants() expects, so the file
// stays compatible with the loader on the next run.
void saveRestaurants(Restaurant catalog[], int count) {
    ofstream outFile("Restaurants.txt");
    if (!outFile) {
        cout << "[!] Could not open Restaurants.txt for writing.\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        outFile << catalog[i].getRestaurantID() << "\n";
        outFile << catalog[i].getName() << "\n";
        outFile << catalog[i].getCuisineType() << "\n";
        outFile << catalog[i].getBasePrice() << "\n";
        outFile << catalog[i].getPromoCode() << "\n";
        outFile << catalog[i].getPromoDiscountPercent() << "\n";
    }
    outFile.close();
    cout << "[System] " << count << " restaurant(s) saved to Restaurants.txt.\n";
}

// ===== MEMBER 3 (ADDED): ADD NEW RECORD REQUIREMENT (RESTAURANT MODULE) =====
// Allows the Admin to add a brand new restaurant to the catalog at runtime,
// instead of only ever being able to view restaurants that existed in the
// .txt file at startup. Demonstrates array bounds checking (MAX_RESTAURANTS)
// and duplicate-ID prevention, wrapped in try/catch per the project rubric.
void addRestaurant(Restaurant catalog[], int &count, int maxSize) {
    try {
        if (count >= maxSize) throw "Restaurant catalog is full. Cannot add more.";

        int newID;
        cout << "Enter new Restaurant ID: ";
        if (!(cin >> newID)) throw "Invalid Restaurant ID. Must be numeric.";

        if (searchRestaurantByID(catalog, count, newID) != -1) {
            throw "A restaurant with this ID already exists.";
        }

        string newName, newCuisine, newPromo;
        double newPrice, newDiscount;

        cin.ignore(10000, '\n');
        cout << "Enter Restaurant Name: ";
        getline(cin, newName);
        cout << "Enter Cuisine Type: ";
        getline(cin, newCuisine);
        cout << "Enter Base Price: ";
        cin >> newPrice;
        cin.ignore(10000, '\n');
        cout << "Enter Promo Code (or NONE): ";
        getline(cin, newPromo);
        cout << "Enter Promo Discount Percent (0 if NONE): ";
        cin >> newDiscount;

        catalog[count] = Restaurant(newID, newName, newCuisine, newPrice, newPromo, newDiscount);
        count++;
        cout << "[System] Restaurant \"" << newName << "\" added to the catalog.\n";
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// ===== MEMBER 3 (ADDED): EDIT/UPDATE RECORD REQUIREMENT (RESTAURANT MODULE) =====
// Lets the Admin update an existing restaurant's details (name, cuisine,
// price, promo code, promo discount) by ID. Before this, the Restaurant
// class only had getters, so the catalog was effectively read-only after
// loading -- this closes that gap and satisfies the "Edit/Update Record"
// functional requirement for the Restaurant module specifically.
void editRestaurant(Restaurant catalog[], int count) {
    try {
        int targetID;
        cout << "Enter Restaurant ID to edit: ";
        if (!(cin >> targetID)) throw "Invalid Restaurant ID. Must be numeric.";

        int idx = searchRestaurantByID(catalog, count, targetID);
        if (idx == -1) throw "Restaurant ID not found in the catalog.";

        cout << "Editing: ";
        displayRestaurantInfo(catalog[idx]);

        int field;
        cout << "What do you want to edit?\n";
        cout << "  1. Name\n";
        cout << "  2. Cuisine Type\n";
        cout << "  3. Base Price\n";
        cout << "  4. Promo Code\n";
        cout << "  5. Promo Discount Percent\n";
        cout << "Enter choice: ";
        if (!(cin >> field)) throw "Invalid menu choice.";

        cin.ignore(10000, '\n');
        if (field == 1) {
            string newName;
            cout << "Enter new Name: ";
            getline(cin, newName);
            catalog[idx].setName(newName);
        } else if (field == 2) {
            string newCuisine;
            cout << "Enter new Cuisine Type: ";
            getline(cin, newCuisine);
            catalog[idx].setCuisineType(newCuisine);
        } else if (field == 3) {
            double newPrice;
            cout << "Enter new Base Price: ";
            cin >> newPrice;
            catalog[idx].setBasePrice(newPrice);
        } else if (field == 4) {
            string newPromo;
            cout << "Enter new Promo Code: ";
            getline(cin, newPromo);
            catalog[idx].setPromoCode(newPromo);
        } else if (field == 5) {
            double newDiscount;
            cout << "Enter new Promo Discount Percent: ";
            cin >> newDiscount;
            catalog[idx].setPromoDiscountPercent(newDiscount);
        } else {
            throw "Invalid field choice.";
        }

        cout << "[System] Restaurant ID " << targetID << " updated successfully.\n";
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// ==========================================
// 5E. MEMBER 3: PLACE ORDER / DISPATCH (LINKED QUEUE WORKFLOW) - ✅ FIXED (Added try-catch)
// ==========================================

// Forward declaration: readInt() is fully defined later in Member 4's section,
// but Member 3's functions below need to call it before that point in the file.
bool readInt(int &out);

// Customer side: builds a DeliveryOrder from the chosen restaurant and ENQUEUES it.
void placeNewOrder(PendingQueue &queue, ActiveOrderList &activeList, Restaurant catalog[], int restaurantCount) {
    try {
        if (restaurantCount == 0) throw "No restaurants available. Cannot place an order.";

        displayMenu(catalog, restaurantCount);   // overload with no filter

        int restChoice;
        cout << "Select Restaurant ID to order from: ";
        if (!readInt(restChoice)) throw "Invalid input. Please enter a numeric Restaurant ID.";

        int idx = -1;
        for (int i = 0; i < restaurantCount; i++) {
            if (catalog[i].getRestaurantID() == restChoice) { idx = i; break; }
        }
        if (idx == -1) throw "Restaurant ID not found.";

        int orderID, time, d, m, y;
        cout << "Enter Order ID (e.g. 2001): ";
        if (!readInt(orderID)) throw "Invalid Order ID.";
        if (activeList.isIDExists(orderID)) throw "Order ID already exists in the active list.";

        cout << "Enter Date (DD MM YYYY): ";
        cin >> d >> m >> y;
        cout << "Enter Est. Delivery Time (mins): ";
        if (!readInt(time)) throw "Invalid delivery time.";

        cin.ignore(10000, '\n');
        string street, city, promoInput;
        cout << "Enter Delivery Street: ";
        getline(cin, street);
        cout << "Enter Delivery City: ";
        getline(cin, city);
        cout << "Enter Promo Code (press Enter to skip): ";
        getline(cin, promoInput);

        double finalPrice;
        if (promoInput == "") {
            finalPrice = calculateTotalPrice(catalog[idx].getBasePrice());   // overload: no promo
        } else {
            finalPrice = applyPromoCode(catalog[idx], promoInput);          // friend function
        }

        char vipAns;
        cout << "Is this a VIP order? (y/n): ";
        cin >> vipAns;
        bool isVip = (vipAns == 'y' || vipAns == 'Y');

        Date dateStruct = {d, m, y};
        Address addrStruct = {street, city};
        DeliveryOrder* newOrder = new DeliveryOrder(orderID, finalPrice, dateStruct, time,
                                                      "Unassigned", addrStruct, "Pending Dispatch", isVip);

        queue.enqueue(newOrder);   // joins the rear of the Linked Queue
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// Admin side: DEQUEUES the longest-waiting order, assigns a rider, then hands it off
// to the existing Doubly Linked List (ActiveOrderList) for live tracking.
void dispatchNextOrder(PendingQueue &queue, ActiveOrderList &activeList) {
    try {
        if (queue.isEmpty()) throw "Pending dispatch line is empty. Nothing to dispatch.";

        DeliveryOrder* nextOrder = queue.dequeue();   // served from the front

        string riderName;
        cout << "Assign Rider Name for Order ID " << nextOrder->getOrderID() << ": ";
        cin.ignore(10000, '\n');
        getline(cin, riderName);
        nextOrder->setRiderName(riderName);
        nextOrder->setOrderStatus("Out for Delivery");

        if (nextOrder->getIsVIP()) {
            activeList.addPriorityOrder(nextOrder);   // existing Member 2 function
        } else {
            activeList.addOrder(nextOrder);           // existing Member 2 function
        }
        cout << "[System] Order dispatched and moved to the Live Active Orders list.\n";
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// ==========================================
// 6. MAIN FUNCTION (INTERACTIVE MENU)
// ==========================================

// MEMBER 1: forward declaration so main() can launch the login portal, which is
// fully defined later in Section 8 (Authentication & User Management). It returns
// the admin's username if an admin logged in (so the admin menu below should run),
// or an empty string if the user only ran customer sessions / chose to exit.
string runAuthenticationSystem(PendingQueue &queue, ActiveOrderList &activeList, Restaurant catalog[], int catalogCount);

int main() {
    ActiveOrderList activeList;
    UndoStack undoHistory;

    cout << "\n============================================\n";
    cout << "   BLOSSOM POS: ORDER MANAGEMENT SYSTEM     \n";
    cout << "============================================\n";
    activeList.loadFromFile(); 

    // ===== MEMBER 3: Load restaurant catalog + set up the dispatch queue =====
    Restaurant restaurantCatalog[MAX_RESTAURANTS];
    int restaurantCount = loadRestaurants(restaurantCatalog, MAX_RESTAURANTS);
    PendingQueue pendingLine;

    // ===== MEMBER 1: Login portal runs first. Customers are fully handled inside
    // the portal (their own restricted menu). If an Admin logs in, the portal hands
    // control back here so the admin order-management menu below can run. =====
    string adminUser = runAuthenticationSystem(pendingLine, activeList, restaurantCatalog, restaurantCount);
    if (adminUser == "") {
        activeList.saveToFile();   // persist any customer changes before quitting
        cout << "Exiting system. Goodbye!\n";
        return 0;
    }
    cout << "\n[System] Welcome, Admin " << adminUser << ". Loading the management system...\n";

    int choice = 0;

    while (choice != 24) {
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
        cout << "11. Analytics & Reports (Sort / Search / Summary)\n";
        cout << "12. View Restaurant Catalog\n";
        cout << "13. Place New Order (Join Pending Dispatch Line)\n";
        cout << "14. Dispatch Next Order (Serve Queue Front)\n";
        cout << "15. View Pending Dispatch Line\n";
        cout << "16. Add a New Restaurant\n";
        cout << "17. Edit a Restaurant\n";
        cout << "18. Search Restaurant by ID\n";
        cout << "19. Search Restaurant by Name\n";
        cout << "20. Save Restaurant Catalog to File\n";
        cout << "21. Preview Next Order in Dispatch Line\n";
        cout << "22. Check an Order's Position in Dispatch Line\n";
        cout << "23. Cancel a Pending Order (Before Dispatch)\n";
        cout << "24. Save and Exit\n";
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
            activeList.analyticsMenu();

        // ===== MEMBER 3: Restaurant Catalog + Linked Queue branches =====
        } else if (choice == 12) {
            displayMenu(restaurantCatalog, restaurantCount);   // overload: show all

        } else if (choice == 13) {
            placeNewOrder(pendingLine, activeList, restaurantCatalog, restaurantCount);

        } else if (choice == 14) {
            dispatchNextOrder(pendingLine, activeList);

        } else if (choice == 15) {
            pendingLine.displayQueue();

        // ===== MEMBER 3 (ADDED): RESTAURANT CRUD + EXTRA QUEUE OPERATIONS =====
        } else if (choice == 16) {
            addRestaurant(restaurantCatalog, restaurantCount, MAX_RESTAURANTS);

        } else if (choice == 17) {
            editRestaurant(restaurantCatalog, restaurantCount);

        } else if (choice == 18) {
            int searchID;
            cout << "Enter Restaurant ID to search: ";
            cin >> searchID;
            int idx = searchRestaurantByID(restaurantCatalog, restaurantCount, searchID);
            if (idx == -1) {
                cout << "[!] Restaurant ID " << searchID << " not found.\n";
            } else {
                displayRestaurantInfo(restaurantCatalog[idx]);
            }

        } else if (choice == 19) {
            string searchName;
            cin.ignore(10000, '\n');
            cout << "Enter Restaurant Name to search: ";
            getline(cin, searchName);
            int idx = searchRestaurantByName(restaurantCatalog, restaurantCount, searchName);
            if (idx == -1) {
                cout << "[!] Restaurant \"" << searchName << "\" not found.\n";
            } else {
                displayRestaurantInfo(restaurantCatalog[idx]);
            }

        } else if (choice == 20) {
            saveRestaurants(restaurantCatalog, restaurantCount);

        } else if (choice == 21) {
            DeliveryOrder* nextUp = pendingLine.peekFront();
            if (nextUp != NULL) {
                cout << "\n[Preview] Next order to be dispatched:\n";
                nextUp->displayOrder();
            }

        } else if (choice == 22) {
            int checkID;
            cout << "Enter Order ID to check its position in the dispatch line: ";
            cin >> checkID;
            int pos = pendingLine.findPosition(checkID);
            if (pos == -1) {
                cout << "[!] Order ID " << checkID << " is not currently in the dispatch line.\n";
            } else {
                cout << "[Queue] Order ID " << checkID << " is currently at position " << pos
                     << " of " << pendingLine.getSize() << " in the dispatch line.\n";
            }

        } else if (choice == 23) {
            int cancelID;
            cout << "Enter Order ID to cancel from the dispatch line: ";
            cin >> cancelID;
            pendingLine.cancelByID(cancelID);

        } else if (choice == 24) {
            activeList.saveToFile();
            saveRestaurants(restaurantCatalog, restaurantCount);
            cout << "Exiting system. Data successfully saved. Goodbye!\n";
        } else {
            cout << "[!] Invalid choice.\n";
        }
    }

    return 0;
}

// ==========================================
// 7. MEMBER 4: ANALYTICS, SORTING & REPORTING
//    (Merge Sort + Binary Search + Summary.txt)
// ==========================================

// Safely read an integer from the user. If they type non-numeric input,
// clear the error and discard the bad characters so the program does not loop.
bool readInt(int &out) {
    cin >> out;                    // try to read an integer
    if (cin.fail()) {              // the user typed something that is not a number
        cin.clear();               // reset cin's error state so it works again
        cin.ignore(10000, '\n');   // throw away the leftover bad characters in the buffer
        return false;              // tell the caller the input was invalid
    }
    return true;                   // the input was a valid integer
}

// Decide the order of two orders for the chosen key and direction.
// Returns true if 'a' should be placed BEFORE 'b'.
bool compareOrders(DeliveryOrder* a, DeliveryOrder* b, int sortKey, bool descending) {
    double valA, valB;
    if (sortKey == 1) {            // 1 = compare by total price
        valA = a->getTotalPrice();
        valB = b->getTotalPrice();
    } else if (sortKey == 2) {     // 2 = compare by delivery time
        valA = (double)a->getDeliveryTime();
        valB = (double)b->getDeliveryTime();
    } else {                       // 3 = compare by order ID
        valA = (double)a->getOrderID();
        valB = (double)b->getOrderID();
    }

    if (descending) {
        return valA >= valB;       // larger value comes first (highest / longest)
    } else {
        return valA <= valB;       // smaller value comes first (lowest / earliest)
    }
}

// Merge two already-sorted halves of the array back into sorted order.
void merge(DeliveryOrder** arr, int left, int mid, int right, int sortKey, bool descending) {
    int n1 = mid - left + 1;       // number of items in the left half
    int n2 = right - mid;          // number of items in the right half

    DeliveryOrder** leftArr = new DeliveryOrder*[n1];   // temp storage for the left half
    DeliveryOrder** rightArr = new DeliveryOrder*[n2];  // temp storage for the right half

    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];      // fill the left temp array
    for (int j = 0; j < n2; j++) rightArr[j] = arr[mid + 1 + j];  // fill the right temp array

    int i = 0, j = 0, k = left;    // i scans left, j scans right, k writes back to arr
    while (i < n1 && j < n2) {     // while both halves still have items
        if (compareOrders(leftArr[i], rightArr[j], sortKey, descending)) {
            arr[k] = leftArr[i];   // the left item wins this position
            i++;
        } else {
            arr[k] = rightArr[j];  // the right item wins this position
            j++;
        }
        k++;
    }

    while (i < n1) { arr[k] = leftArr[i]; i++; k++; }    // copy any leftovers from the left half
    while (j < n2) { arr[k] = rightArr[j]; j++; k++; }   // copy any leftovers from the right half

    delete[] leftArr;              // free the temporary left array
    delete[] rightArr;             // free the temporary right array
}

// Merge Sort: recursively split the array, then merge the sorted pieces.
void mergeSort(DeliveryOrder** arr, int left, int right, int sortKey, bool descending) {
    if (left < right) {                            // stop when the piece has 0 or 1 item
        int mid = left + (right - left) / 2;       // safe midpoint (avoids overflow)
        mergeSort(arr, left, mid, sortKey, descending);        // sort the left half
        mergeSort(arr, mid + 1, right, sortKey, descending);   // sort the right half
        merge(arr, left, mid, right, sortKey, descending);     // merge the two sorted halves
    }
}

// Binary Search on an array already sorted ascending by Order ID.
// Returns the index of the match, or -1 if the ID is not present.
int binarySearchByID(DeliveryOrder** arr, int n, int targetID) {
    int low = 0;                   // lower bound of the search window
    int high = n - 1;              // upper bound of the search window

    while (low <= high) {          // keep going while the window is valid
        int mid = low + (high - low) / 2;     // middle index of the window
        int midID = arr[mid]->getOrderID();   // the ID sitting at the middle

        if (midID == targetID) {
            return mid;            // exact match found
        } else if (midID < targetID) {
            low = mid + 1;         // search the right half
        } else {
            high = mid - 1;        // search the left half
        }
    }
    return -1;                     // target ID was never found
}

// Count how many orders are currently in the active list.
int ActiveOrderList::countOrders() {
    int count = 0;                 // running counter
    OrderNode* temp = head;        // begin at the head of the list
    while (temp != NULL) {         // walk to the end of the list
        count++;                   // one more order counted
        temp = temp->next;         // step to the next node
    }
    return count;                  // give back the total
}

// Copy all orders into an array, run Merge Sort, then print them in order.
void ActiveOrderList::runSortReport(int sortKey, bool descending) {
    try {
        int n = countOrders();                 // find out how many orders we have
        if (n == 0) {                          // guard against an empty list
            throw "No active orders available to sort.";
        }

        DeliveryOrder** arr = new DeliveryOrder*[n];   // dynamic array of order pointers
        OrderNode* temp = head;                // start copying from the head
        int i = 0;                             // array write index
        while (temp != NULL) {                 // copy every node's pointer into the array
            arr[i] = temp->data;               // store the pointer (shallow copy, shared object)
            i++;
            temp = temp->next;
        }

        mergeSort(arr, 0, n - 1, sortKey, descending);   // sort the whole array

        string keyName;                        // human-readable name of the sort field
        if (sortKey == 1) keyName = "Total Price";
        else if (sortKey == 2) keyName = "Delivery Time";
        else keyName = "Order ID";
        string dir = descending ? "Highest to Lowest" : "Lowest to Highest";   // direction label

        cout << "\n========== SORTED ORDERS (" << keyName << " : " << dir << ") ==========\n";
        for (int j = 0; j < n; j++) {          // walk the sorted array
            arr[j]->displayOrder();            // reuse the existing display method
        }
        cout << "=====================================================================\n";

        delete[] arr;                          // free ONLY the pointer array, not the orders
    } catch (const char* msg) {                // catch our thrown text message
        cout << "[!] " << msg << "\n";
    }
}

// Sort by Order ID, then use Binary Search to fetch one order instantly.
void ActiveOrderList::runSearchByID() {
    try {
        int n = countOrders();                 // how many orders exist
        if (n == 0) {                          // nothing to search
            throw "No active orders available to search.";
        }

        DeliveryOrder** arr = new DeliveryOrder*[n];   // dynamic pointer array
        OrderNode* temp = head;
        int i = 0;
        while (temp != NULL) {                 // copy every order pointer into the array
            arr[i] = temp->data;
            i++;
            temp = temp->next;
        }

        mergeSort(arr, 0, n - 1, 3, false);    // Binary Search needs the data sorted by ID first

        int targetID;                          // the ID the user is looking for
        cout << "Enter the Order ID you want to find: ";
        if (!readInt(targetID)) {              // reject letters / symbols
            throw "Invalid input. Please enter a numeric Order ID.";
        }

        int foundIndex = binarySearchByID(arr, n, targetID);   // run the search

        if (foundIndex == -1) {                // -1 means not found
            cout << "[!] Order ID " << targetID << " was not found.\n";
        } else {
            cout << "\n[System] Order located instantly with Binary Search:\n";
            arr[foundIndex]->displayOrder();   // show the matching order
        }

        delete[] arr;                          // free the pointer array only
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// Read Summary.txt from disk and print it to the screen.
void ActiveOrderList::displaySavedSummary() {
    try {
        ifstream inFile("Summary.txt");        // open the saved report for reading
        if (!inFile) {                         // file missing or cannot be opened
            throw "Summary.txt not found. Please generate the report first.";
        }

        cout << "\n----- RETRIEVED FROM Summary.txt -----\n";
        string line;                           // holds one line at a time
        while (getline(inFile, line)) {        // read the file line by line
            cout << line << "\n";              // echo each saved line to the screen
        }
        inFile.close();                        // close the file handle
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// Build the end-of-day statistics, save them to Summary.txt, then show them.
void ActiveOrderList::generateSummaryReport() {
    try {
        int n = countOrders();                 // total number of active orders
        if (n == 0) {                          // cannot summarise an empty list
            throw "No active orders to summarise.";
        }

        double totalRevenue = 0.0;             // sum of every order price
        int totalTime = 0;                     // sum of every delivery time
        int vipCount = 0;                      // how many VIP orders
        int pending = 0, outForDelivery = 0, delivered = 0, cancelled = 0;   // status tallies

        OrderNode* temp = head;                // start at the head
        while (temp != NULL) {                 // visit every order once
            totalRevenue += temp->data->getTotalPrice();    // add this price to the total
            totalTime += temp->data->getDeliveryTime();     // add this time to the total
            if (temp->data->getIsVIP()) vipCount++;         // count VIP orders

            string st = temp->data->getOrderStatus();       // read the status text
            if (st == "Pending") pending++;                 // bucket the order by status
            else if (st == "Out for Delivery") outForDelivery++;
            else if (st == "Delivered") delivered++;
            else if (st == "Cancelled") cancelled++;

            temp = temp->next;                 // move to the next order
        }

        double avgValue = totalRevenue / n;     // average money per order
        double avgTime = (double)totalTime / n; // average delivery time per order

        DeliveryOrder** arr = new DeliveryOrder*[n];   // pointer array for the "top" lookups
        temp = head;
        int i = 0;
        while (temp != NULL) { arr[i] = temp->data; i++; temp = temp->next; }   // fill the array

        mergeSort(arr, 0, n - 1, 1, true);     // sort by price, highest first
        int topID = arr[0]->getOrderID();      // the biggest sale is now at the front
        double topValue = arr[0]->getTotalPrice();

        mergeSort(arr, 0, n - 1, 2, true);     // sort by delivery time, longest first
        int longID = arr[0]->getOrderID();     // the longest delivery is now at the front
        int longTime = arr[0]->getDeliveryTime();

        delete[] arr;                          // free the pointer array

        ofstream outFile("Summary.txt");       // open Summary.txt for writing
        if (!outFile) {                        // creation failed
            throw "Could not open Summary.txt for writing.";
        }

        outFile << "=============================================\n";
        outFile << "        BLOSSOM POS - END OF DAY SUMMARY      \n";
        outFile << "=============================================\n";
        outFile << "Total Active Orders : " << n << "\n";
        outFile << "Total VIP Orders    : " << vipCount << "\n";
        outFile << "Total Revenue       : RM" << totalRevenue << "\n";
        outFile << "Average Order Value : RM" << avgValue << "\n";
        outFile << "Average Delivery    : " << avgTime << " mins\n";
        outFile << "---------------------------------------------\n";
        outFile << "STATUS BREAKDOWN\n";
        outFile << "  Pending          : " << pending << "\n";
        outFile << "  Out for Delivery : " << outForDelivery << "\n";
        outFile << "  Delivered        : " << delivered << "\n";
        outFile << "  Cancelled        : " << cancelled << "\n";
        outFile << "---------------------------------------------\n";
        outFile << "Highest Value Order : #" << topID << " (RM" << topValue << ")\n";
        outFile << "Longest Delivery    : #" << longID << " (" << longTime << " mins)\n";
        outFile << "=============================================\n";
        outFile.close();                       // finish writing and close the file

        cout << "[System] Summary report generated and saved to Summary.txt.\n";
        displaySavedSummary();                 // immediately read it back and show it
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// Sub-menu that lets the admin choose a sort, a search, or the summary report.
void ActiveOrderList::analyticsMenu() {
    int subChoice = 0;             // the admin's choice inside this sub-menu
    while (subChoice != 6) {       // keep looping until they pick "Back"
        cout << "\n--- ANALYTICS & REPORTS ---\n";
        cout << "1. Sort Orders by Price (Highest Sales)\n";
        cout << "2. Sort Orders by Delivery Time (Longest)\n";
        cout << "3. Sort Orders by Order ID\n";
        cout << "4. Search an Order by ID (Binary Search)\n";
        cout << "5. Generate End-of-Day Summary Report\n";
        cout << "6. Back to Main Menu\n";
        cout << "Select an option: ";
        if (!readInt(subChoice)) {             // user typed a letter instead of a number
            cout << "[!] Please enter a number (1-6).\n";
            continue;                          // reprint the menu and ask again
        }

        if (subChoice == 1) {
            runSortReport(1, true);    // price, highest to lowest
        } else if (subChoice == 2) {
            runSortReport(2, true);    // delivery time, longest to shortest
        } else if (subChoice == 3) {
            runSortReport(3, false);   // order ID, ascending
        } else if (subChoice == 4) {
            runSearchByID();           // binary search by ID
        } else if (subChoice == 5) {
            generateSummaryReport();   // build + save + show the report
        } else if (subChoice != 6) {
            cout << "[!] Invalid option.\n";   // anything else is invalid
        }
    }
}


// ==========================================================================
// 8. MEMBER 1: AUTHENTICATION & USER MANAGEMENT
//    (Register / Login / Logout  +  Users.txt & Customers.txt)
// ==========================================================================

const int MAX_USERS = 200;


// 8.1 EARLY FORWARD DECLARATIONS
string hashPassword(string plain);                        // djb2 password hash
void   printAuthHeader(string title);                     // overload 1
void   printAuthHeader(string title, string subtitle);    // overload 2
string readNonEmptyField(string prompt);

// --------------------------------------------------------------------------
// 8.2  PASSWORD HASH FUNCTION  (djb2)
// --------------------------------------------------------------------------

string hashPassword(string plain) {
    unsigned long hash = 5381;                    
    for (int i = 0; i < (int)plain.length(); i++) {
        hash = ((hash << 5) + hash) + (unsigned char)plain[i];
    }
    return to_string(hash);                       
}

// --------------------------------------------------------------------------
// 8.3  OOP CLASSES  (Base + two Derived classes)
// --------------------------------------------------------------------------

class User {
protected:
    string username;   
    string password;   
    string role;       

public:
    User() {
        username = "";
        password = "";
        role = "";
    }

    User(string u, string p, string r) {
        username = u;
        password = p;
        role = r;
    }

    virtual ~User() {
    }

    string getUsername() const { return username; }
    string getPassword() const { return password; }   
    string getRole() const { return role; }

    // Hashes the typed password and compares it to the stored hash.
    bool checkPassword(string enteredPlain) const {
        return password == hashPassword(enteredPlain);
    }

    // Virtual so Admin and Customer can show their own extra fields.
    virtual void displayProfile() const {
        cout << "Username : " << username << "\n";
        cout << "Role     : " << role << "\n";
    }
};

// DERIVED CLASS: Admin  -  a staff / management account.
class Admin : public User {
private:
    string staffID; 

public:
    Admin() : User() {
        staffID = "";
        role = "Admin";
    }

    Admin(string u, string p, string id) : User(u, p, "Admin") {
        staffID = id;
    }

    ~Admin() {

    }

    string getStaffID() const { return staffID; }

    void displayProfile() const override {
        cout << "----- ADMIN PROFILE -----\n";
        cout << "Username : " << username << "\n";
        cout << "Role     : " << role << "\n";
        cout << "Staff ID : " << staffID << "\n";
        cout << "-------------------------\n";
    }
};

// DERIVED CLASS: Customer  -  a regular user account.
class Customer : public User {
private:
    string phone;   
    string address;    

public:
    Customer() : User() {
        phone = "";
        address = "";
        role = "Customer";
    }

    Customer(string u, string p, string ph, string addr) : User(u, p, "Customer") {
        phone = ph;
        address = addr;
    }

    ~Customer() {
    }

    string getPhone() const { return phone; }
    string getAddress() const { return address; }

    void displayProfile() const override {
        cout << "----- CUSTOMER PROFILE -----\n";
        cout << "Username : " << username << "\n";
        cout << "Role     : " << role << "\n";
        cout << "Phone    : " << phone << "\n";
        cout << "Address  : " << address << "\n";
        cout << "----------------------------\n";
    }
};


void   bubbleSortUsers(User** arr, int n);                          
int    binarySearchUser(User** arr, int n, string targetUsername); 

int    loadUsersFromFile(string filename, string role, User** arr, int startIndex, int maxSize);
int    loadAllUsers(User** arr, int maxSize);
void   freeUsers(User** arr, int n);
bool   usernameExists(string uname);
void   seedDefaultAdmin();

void   registerCustomer();
void   registerAdmin();
bool   loginUser(string &loggedUser, string &loggedRole);

void   recordCustomerOrder(int orderID, string username, double price, string street, string city);
string findOrderOwner(int targetID);
void   removeOrderOwner(int targetID);
string getOrderStatusFromFile(int targetID);
bool   removeFromQueueByID(PendingQueue &queue, int targetID);

void   customerPlaceOrder(string username, PendingQueue &queue, ActiveOrderList &activeList, Restaurant catalog[], int catalogCount);
void   trackMyOrders(string username, ActiveOrderList &activeList);
void   cancelMyOrder(string username, ActiveOrderList &activeList, PendingQueue &queue);
void   runCustomerMenu(string username, PendingQueue &queue, ActiveOrderList &activeList, Restaurant catalog[], int catalogCount);

string runAuthenticationSystem(PendingQueue &queue, ActiveOrderList &activeList, Restaurant catalog[], int catalogCount);

// --------------------------------------------------------------------------
// 8.4  SMALL DISPLAY / INPUT HELPERS
// --------------------------------------------------------------------------

void printAuthHeader(string title) {
    cout << "\n==================================================\n";
    cout << "   " << title << "\n";
    cout << "==================================================\n";
}

void printAuthHeader(string title, string subtitle) {
    cout << "\n==================================================\n";
    cout << "   " << title << " : " << subtitle << "\n";
    cout << "==================================================\n";
}


string readNonEmptyField(string prompt) {
    string value = "";
    while (value == "") {
        cout << prompt;
        getline(cin, value);
        if (value == "") {
            cout << "  [!] This field cannot be empty. Please try again.\n";
        }
    }
    return value;
}

// --------------------------------------------------------------------------
// 8.5  SORTING ALGORITHM  -  BUBBLE SORT  (Chapter 8)
// --------------------------------------------------------------------------

void bubbleSortUsers(User** arr, int n) {
    int i = 0;
    bool sorted = false;            

    while (i < n && sorted == false) {
        sorted = true;              
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j]->getUsername() > arr[j + 1]->getUsername()) {
                User* temp = arr[j];       
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                sorted = false;            
            }
        }
        i++;
    }
}

// --------------------------------------------------------------------------
// 8.6  SEARCHING ALGORITHM  -  BINARY SEARCH  
// --------------------------------------------------------------------------

int binarySearchUser(User** arr, int n, string targetUsername) {
    int first = 0;                 
    int last = n - 1;             
    int found = -1;                 
    while (found == -1 && first <= last) {
        int mid = (first + last) / 2;               
        string midName = arr[mid]->getUsername();  
        if (midName == targetUsername) {
            found = mid;                            
        } else if (targetUsername < midName) {
            last = mid - 1;                        
        } else {
            first = mid + 1;                       
        }
    }
    return found;
}

// --------------------------------------------------------------------------
// 8.7  ACCOUNT FILE HANDLING  -  Users.txt and Customers.txt
// --------------------------------------------------------------------------

int loadUsersFromFile(string filename, string role, User** arr, int startIndex, int maxSize) {
    ifstream inFile(filename);
    if (!inFile) {
        return startIndex;          
    }

    int count = startIndex;
    string uname, phash, field3, field4;

    while (count < maxSize && getline(inFile, uname)) {
        if (uname == "") continue;              
        if (!getline(inFile, phash)) break;     
        if (!getline(inFile, field3)) break;    

        if (role == "Admin") {
            arr[count] = new Admin(uname, phash, field3);         
        } else {
            if (!getline(inFile, field4)) break;                  
            arr[count] = new Customer(uname, phash, field3, field4);
        }
        count++;
    }

    inFile.close();
    return count;
}

int loadAllUsers(User** arr, int maxSize) {
    int count = 0;
    count = loadUsersFromFile("Users.txt", "Admin", arr, count, maxSize);
    count = loadUsersFromFile("Customers.txt", "Customer", arr, count, maxSize);
    return count;
}

void freeUsers(User** arr, int n) {
    for (int i = 0; i < n; i++) {
        delete arr[i];      
    }
}

bool usernameExists(string uname) {
    User** users = new User*[MAX_USERS];        
    int n = loadAllUsers(users, MAX_USERS);

    bool exists = false;
    for (int i = 0; i < n; i++) {
        if (users[i]->getUsername() == uname) {
            exists = true;
            break;
        }
    }

    freeUsers(users, n);
    delete[] users;
    return exists;
}


void seedDefaultAdmin() {
    User** users = new User*[MAX_USERS];
    int n = loadAllUsers(users, MAX_USERS);
    freeUsers(users, n);
    delete[] users;

    if (n > 0) {
        return;         
    }

    ofstream outFile("Users.txt", ios::app);
    if (!outFile) {
        return;
    }
    outFile << "admin" << "\n"
            << hashPassword("admin123") << "\n"
            << "S001" << "\n";
    outFile.close();

    cout << "[System] No accounts found yet, so a default admin was created.\n";
    cout << "         Username: admin  |  Password: admin123\n";
}

// --------------------------------------------------------------------------
// 8.8  REGISTRATION
// --------------------------------------------------------------------------

// Registers a brand-new Customer and appends it to Customers.txt.
void registerCustomer() {
    printAuthHeader("REGISTER NEW CUSTOMER");

    try {
        cin.ignore(10000, '\n');   

        string uname = readNonEmptyField("Choose a Username: ");
        if (usernameExists(uname)) {
            throw "That username is already taken. Please pick another one.";
        }

        string pwd   = readNonEmptyField("Choose a Password: ");
        string phone = readNonEmptyField("Enter Phone Number: ");
        string addr  = readNonEmptyField("Enter Delivery Address: ");

        ofstream outFile("Customers.txt", ios::app);   
        if (!outFile) {
            throw "Could not open Customers.txt for writing.";
        }
        
        outFile << uname << "\n" << hashPassword(pwd) << "\n"
                << phone << "\n" << addr << "\n";
        outFile.close();

        printAuthHeader("CUSTOMER REGISTERED", uname);
        cout << "[System] New customer account saved to Customers.txt.\n";
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}


void registerAdmin() {
    printAuthHeader("REGISTER NEW ADMIN / STAFF");

    try {
        cin.ignore(10000, '\n');  

        string code = readNonEmptyField("Enter Staff Registration Code: ");
        if (code != "MMU2026") {
            throw "Incorrect staff code. Admin registration cancelled.";
        }

        string uname = readNonEmptyField("Choose a Username: ");
        if (usernameExists(uname)) {
            throw "That username is already taken. Please pick another one.";
        }

        string pwd     = readNonEmptyField("Choose a Password: ");
        string staffID = readNonEmptyField("Enter Staff ID (e.g. S002): ");

        ofstream outFile("Users.txt", ios::app);       
        if (!outFile) {
            throw "Could not open Users.txt for writing.";
        }
        
        outFile << uname << "\n" << hashPassword(pwd) << "\n" << staffID << "\n";
        outFile.close();

        printAuthHeader("ADMIN REGISTERED", uname);
        cout << "[System] New admin account saved to Users.txt.\n";
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}

// --------------------------------------------------------------------------
// 8.9  LOGIN  (Sort, then Binary Search)
// --------------------------------------------------------------------------

bool loginUser(string &loggedUser, string &loggedRole) {
    printAuthHeader("LOGIN");

    User** users = new User*[MAX_USERS];           
    int n = loadAllUsers(users, MAX_USERS);      

    bool success = false;

    try {
        if (n == 0) {
            throw "No accounts exist yet. Please register first.";
        }

        bubbleSortUsers(users, n);                 

        cin.ignore(10000, '\n');                   
        string uname = readNonEmptyField("Username: ");
        string pwd   = readNonEmptyField("Password: ");

        int index = binarySearchUser(users, n, uname);   

        if (index == -1) {
            cout << "[!] Account '" << uname << "' does not exist.\n";
        } else if (!users[index]->checkPassword(pwd)) {   
            cout << "[!] Incorrect password. Login failed.\n";
        } else {
            loggedUser = users[index]->getUsername();
            loggedRole = users[index]->getRole();
            printAuthHeader("LOGIN SUCCESSFUL", loggedUser);
            users[index]->displayProfile();   
            success = true;
        }
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }

    freeUsers(users, n);     
    delete[] users;          
    return success;
}

// --------------------------------------------------------------------------
// 8.10  CUSTOMER ORDER LEDGER  -  CustomerOrders.txt
// --------------------------------------------------------------------------

void recordCustomerOrder(int orderID, string username, double price,
                         string street, string city) {
    ofstream out("CustomerOrders.txt", ios::app);
    if (!out) return;
    out << orderID << "\n" << username << "\n" << price << "\n"
        << street << "\n" << city << "\n";
    out.close();
}

string findOrderOwner(int targetID) {
    ifstream in("CustomerOrders.txt");
    if (!in) return "";

    int id;
    double price;
    string owner = "";
    string nameLine, streetLine, cityLine;
    while (in >> id) {
        in.ignore(10000, '\n');     
        getline(in, nameLine);      
        in >> price; in.ignore(10000, '\n');
        getline(in, streetLine);   
        getline(in, cityLine);      
        if (id == targetID) {
            owner = nameLine;    
        }
    }
    in.close();
    return owner;
}


void removeOrderOwner(int targetID) {
    ifstream in("CustomerOrders.txt");
    if (!in) return;

    int    keepIDs[MAX_USERS];
    string keepNames[MAX_USERS];
    double keepPrices[MAX_USERS];
    string keepStreets[MAX_USERS];
    string keepCities[MAX_USERS];
    int    keepCount = 0;

    int id;
    double price;
    string nameLine, streetLine, cityLine;
    while (keepCount < MAX_USERS && in >> id) {
        in.ignore(10000, '\n');
        getline(in, nameLine);
        in >> price; in.ignore(10000, '\n');
        getline(in, streetLine);
        getline(in, cityLine);
        if (id != targetID) {                 
            keepIDs[keepCount]     = id;
            keepNames[keepCount]   = nameLine;
            keepPrices[keepCount]  = price;
            keepStreets[keepCount] = streetLine;
            keepCities[keepCount]  = cityLine;
            keepCount++;
        }
    }
    in.close();

    ofstream out("CustomerOrders.txt");       
    for (int i = 0; i < keepCount; i++) {
        out << keepIDs[i] << "\n" << keepNames[i] << "\n" << keepPrices[i] << "\n"
            << keepStreets[i] << "\n" << keepCities[i] << "\n";
    }
    out.close();
}

bool removeFromQueueByID(PendingQueue &queue, int targetID) {
    int n = queue.getSize();
    bool removed = false;
    for (int i = 0; i < n; i++) {
        DeliveryOrder* o = queue.dequeue();        
        if (o == NULL) break;
        if (!removed && o->getOrderID() == targetID) {
            delete o;                              
            removed = true;
        } else {
            queue.enqueue(o);                     
        }
    }
    return removed;
}


string getOrderStatusFromFile(int targetID) {
    ifstream in("Orders.txt");
    if (!in) return "";

    int id, time, day, month, year;
    double price;
    bool vip;
    string status, rider, street, city;

    while (in >> id) {
        in >> vip >> price >> day >> month >> year >> time >> ws;
        getline(in, status);
        getline(in, rider);
        getline(in, street);
        getline(in, city);
        if (id == targetID) {
            in.close();
            return status;
        }
    }
    in.close();
    return "";
}


// --------------------------------------------------------------------------
// 8.11  CUSTOMER FEATURES 
// --------------------------------------------------------------------------

void customerPlaceOrder(string username, PendingQueue &queue, ActiveOrderList &activeList,
                        Restaurant catalog[], int catalogCount) {
    printAuthHeader("PLACE A NEW ORDER", username);

    try {
        if (catalogCount == 0) {
            throw "No restaurants are available right now.";
        }

        displayMenu(catalog, catalogCount);  

        cout << "Select a Restaurant ID: ";
        int restID;
        if (!readInt(restID)) throw "Invalid Restaurant ID.";

        int idx = -1;
        for (int i = 0; i < catalogCount; i++) {
            if (catalog[i].getRestaurantID() == restID) { idx = i; break; }
        }
        if (idx == -1) throw "Restaurant ID not found.";

        cout << "Enter a new Order ID (e.g. 3001): ";
        int orderID;
        if (!readInt(orderID)) throw "Invalid Order ID.";
  
        if (activeList.isIDExists(orderID)) throw "That Order ID already exists.";
        if (findOrderOwner(orderID) != "")  throw "That Order ID already exists.";

        int d, m, y, time;
        cout << "Enter Order Date (DD MM YYYY): ";
        if (!(cin >> d >> m >> y)) {
            cin.clear();
            cin.ignore(10000, '\n');
            throw "Invalid date entered.";
        }
        cout << "Enter Est. Delivery Time (mins): ";
        if (!readInt(time)) throw "Invalid delivery time.";

        cin.ignore(10000, '\n');               
        string street = readNonEmptyField("Enter Delivery Street: ");
        string city   = readNonEmptyField("Enter Delivery City: ");

        double price = catalog[idx].getBasePrice();   

        Date dateStruct = {d, m, y};
        Address addrStruct = {street, city};


        DeliveryOrder* newOrder = new DeliveryOrder(orderID, price, dateStruct, time,
                                                    "Unassigned", addrStruct,
                                                    "Pending Dispatch", false);

        queue.enqueue(newOrder);                                   
        recordCustomerOrder(orderID, username, price, street, city);   

        cout << "[System] Order " << orderID
             << " placed and added to the dispatch queue.\n";
        cout << "[System] Status: Pending Dispatch - an admin will assign a rider.\n";
    } catch (const char* msg) {
        cout << "[!] " << msg << "\n";
    }
}


void trackMyOrders(string username, ActiveOrderList &activeList) {
    printAuthHeader("TRACK MY ORDERS", username);

    activeList.saveToFile();          

    ifstream in("CustomerOrders.txt");
    if (!in) {
        cout << "[System] You have not placed any orders yet.\n";
        return;
    }

    int id;
    double price;
    string owner, street, city;
    bool anyFound = false;

    while (in >> id) {
        in.ignore(10000, '\n');
        getline(in, owner);                       
        in >> price; in.ignore(10000, '\n');      
        getline(in, street);                      
        getline(in, city);                        

        if (owner != username) continue;          

     
        string liveStatus = getOrderStatusFromFile(id);
        if (liveStatus == "") {
            liveStatus = "Pending Dispatch (waiting for a rider)";
        }

        cout << "Order ID : " << id << "\n";
        cout << "Status   : " << liveStatus << "\n";
        cout << "Price    : RM" << price << "\n";
        cout << "Deliver  : " << street << ", " << city << "\n";
        cout << "--------------------------------------------------\n";
        anyFound = true;
    }
    in.close();

    if (!anyFound) {
        cout << "[System] You have not placed any orders yet.\n";
    }
}

void cancelMyOrder(string username, ActiveOrderList &activeList, PendingQueue &queue) {
    printAuthHeader("CANCEL MY ORDER", username);

    cout << "Enter the Order ID you want to cancel: ";
    int targetID;
    if (!readInt(targetID)) {
        cout << "[!] Invalid Order ID.\n";
        return;
    }

    if (findOrderOwner(targetID) != username) {
        cout << "[!] Error: Order " << targetID
             << " is not yours, or it does not exist.\n";
        return;
    }

    activeList.saveToFile();
    string liveStatus = getOrderStatusFromFile(targetID);
    if (liveStatus != "") {
        cout << "[!] Error: Order is already on the way and cannot be cancelled.\n";
        return;
    }

    if (removeFromQueueByID(queue, targetID)) {
        removeOrderOwner(targetID);
        cout << "[System] Order " << targetID << " has been cancelled successfully.\n";
    } else {
        cout << "[!] Order " << targetID << " could not be found in the dispatch queue.\n";
    }
}

void runCustomerMenu(string username, PendingQueue &queue, ActiveOrderList &activeList,
                     Restaurant catalog[], int catalogCount) {
    int choice = 0;
    while (choice != 5) {
        printAuthHeader("CUSTOMER MENU", username);
        cout << "1. View Menu Catalog\n";
        cout << "2. Place a New Order\n";
        cout << "3. Track My Orders\n";
        cout << "4. Cancel My Order\n";
        cout << "5. Logout\n";
        cout << "Enter choice: ";

        if (!readInt(choice)) {
            cout << "[!] Please enter a number between 1 and 5.\n";
            continue;
        }

        if (choice == 1) {
            displayMenu(catalog, catalogCount);   
        } else if (choice == 2) {
            customerPlaceOrder(username, queue, activeList, catalog, catalogCount);
        } else if (choice == 3) {
            trackMyOrders(username, activeList);
        } else if (choice == 4) {
            cancelMyOrder(username, activeList, queue);
        } else if (choice == 5) {
            cout << "[System] Logging out " << username << ". See you again!\n";
        } else {
            cout << "[!] Invalid choice. Please try again.\n";
        }
    }
}

// --------------------------------------------------------------------------
// 8.12  AUTHENTICATION PORTAL (DRIVER)
// --------------------------------------------------------------------------
string runAuthenticationSystem(PendingQueue &queue, ActiveOrderList &activeList,
                               Restaurant catalog[], int catalogCount) {
    seedDefaultAdmin();   

    int choice = 0;
    while (true) {
        printAuthHeader("BLOSSOM POS - LOGIN PORTAL");
        cout << "1. Register as Customer\n";
        cout << "2. Register as Admin / Staff\n";
        cout << "3. Login\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        if (!readInt(choice)) {
            cout << "[!] Please enter a number between 1 and 4.\n";
            continue;
        }

        if (choice == 1) {
            registerCustomer();
        } else if (choice == 2) {
            registerAdmin();
        } else if (choice == 3) {
            string user = "";
            string role = "";
            if (loginUser(user, role)) {
                if (role == "Admin") {
                    return user;                 
                } else {
                    runCustomerMenu(user, queue, activeList, catalog, catalogCount);
                }
            }
        } else if (choice == 4) {
            cout << "[System] Closing the login portal.\n";
            return "";                          
        } else {
            cout << "[!] Invalid choice. Please try again.\n";
        }
    }
}