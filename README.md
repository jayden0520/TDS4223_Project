# 🌸 Blossom POS Delivery Management System

A highly dynamic, terminal-based Point of Sale (POS) and Delivery Management System built entirely in C++ from scratch. This project was developed to streamline F&B business operations by handling live order tracking, rider dispatching, VIP priority queueing, and end-of-day analytics.

**Note:** This system relies entirely on custom-built Object-Oriented architectures and raw memory pointers. **Zero Standard Template Libraries (STL) were used** for the data structures.

---

## 🚀 Key Features
* **Role-Based Access Control (RBAC):** Secure login system with distinct interfaces for Admins (Management) and Customers (Ordering/Tracking). Passwords are encrypted using a custom hashing algorithm.
* **Live Dashboard & Order Tracking:** Real-time calculation of daily revenue, active order statuses, and average delivery times.
* **VIP Priority Queueing:** Standard orders join the back of the line, while VIP orders bypass the queue and jump to the front.
* **Fail-Safe "Recycle Bin":** Accidental deletions can be instantly recovered via a Last-In-First-Out (LIFO) "Undo" mechanic.
* **Fair Rider Dispatching:** A strict First-In-First-Out (FIFO) queue ensures customers who order first are served first.
* **Advanced Analytics:** Instantly sort hundreds of end-of-day records to identify the highest revenue sales, and generate individual `.txt` receipts.

---

## 🧠 Data Structures & Algorithms Built
To fulfill advanced memory management requirements, the following structures were engineered manually using dynamic node pointers (`new`/`delete`):

* **Doubly Linked List:** Acts as the primary live database, allowing efficient two-way traversal for editing and VIP Head/Tail insertions.
* **Linked Stack:** Powers the Undo/Recycle Bin feature (LIFO).
* **Linked Queue:** Manages the Pending Dispatch line for riders (FIFO).
* **Merge Sort:** Recursively sorts completed orders by Highest Price or Longest Delivery Time ($O(n \log n)$ complexity).
* **Binary Search:** Provides lightning-fast lookups for user authentication and specific Order IDs ($O(\log n)$ complexity).

---

## ⚙️ How to Compile and Run

1. Ensure you have a C++ compiler installed (e.g., GCC/MinGW).
2. Download the `Project_GR1.cpp` file and the associated `.txt` files to the same folder.
3. Open your terminal or command prompt in that folder.
4. Compile the program:
   `g++ Project_GR1.cpp -o BlossomPOS`
5. Run the executable:
   * **Windows:** `BlossomPOS.exe`
   * **Mac/Linux:** `./BlossomPOS`

### 🔑 Default Testing Credentials
To bypass the registration screen and test the Admin Management System immediately, use the following seeded credentials:
* **Username:** `admin`
* **Password:** `admin123`

*(To register a new Admin account, the internal Staff Code is `MMU2026`).*

---

## 📁 File Input/Output System
The system ensures complete data persistence between sessions using the `<fstream>` library. Do not delete these files, as the program relies on them to load previous states:
* `Orders.txt` - Stores the master Doubly Linked List of live deliveries.
* `Users.txt` - Stores registered Admin credentials and Staff IDs.
* `Customers.txt` - Stores registered Customer profiles.
* `CustomerOrders.txt` - A ledger linking specific Order IDs to Customer usernames.
* `Restaurants.txt` - Stores the dynamic array catalog of available food and floral items.
* `Summary.txt` - Generated at the end of the day by the Analytics module.

---

## 👥 Team Members (Group 1)
* **Member 1:** Low Yong Yi *(Authentication, User Inheritance, Hashing, Binary Search)*
* **Member 2:** Yong Yi *(Doubly Linked List, Linked Stack, File I/O, VIP Memory Logic)*
* **Member 3:** Yam Chee Fai *(Linked Queue Dispatch, Restaurant CRUD, Friend & Overloaded Functions)*
* **Member 4:** Dylan Tan Chun Wei *(Merge Sort Analytics, Reporting, Binary Search for Orders)*