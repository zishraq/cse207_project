#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <iomanip>

using namespace std;

class ElectronicToolsDetails {
public:
    string name;
    int serial;
    string category;
    double price;

    ElectronicToolsDetails() {

    }

    ElectronicToolsDetails(string name, int serial, string category, double price) {
        this->name = name;
        this->serial = serial;
        this->category = category;
        this->price = price;
    }

    ElectronicToolsDetails(string name, int serial, double price) {
        this->name = name;
        this->serial = serial;
        this->category = "";
        this->price = price;
    }
};

class ElectronicToolsNode {
public:
    ElectronicToolsDetails *item;
    ElectronicToolsNode *next;

    ElectronicToolsNode(ElectronicToolsDetails *item) {
        this->item = item;
        this->next = NULL;
    }
};

class ElectronicToolsLinkedList {
public:
    ElectronicToolsNode *head;
    int last_serial;

    ElectronicToolsLinkedList() {
        this->head = NULL;
    }

    void addItem(ElectronicToolsNode *item) {
        ElectronicToolsNode *temp = new ElectronicToolsNode(*item);

        if (!head) {
            this->head = temp;
        } else {
            ElectronicToolsNode *iter = this->head;
            while (iter->next != NULL) {
                iter = iter->next;
            }

            iter->next = temp;
        }

        this->last_serial = temp->item->serial + 1;
    }

    void removeItem(int serial) {
        ElectronicToolsNode *current = NULL;
        ElectronicToolsNode *previous = NULL;
        current = this->head;

        if (serial == 1) {
            this->head = this->head->next;
        } else {
            for (int i = 1; i < serial; i++) {
                previous = current;
                current = current->next;
            }

            previous->next = current->next;

        }

        this->last_serial--;

        serialUpdate();
    }

    void serialUpdate() {
        ElectronicToolsNode *iter = this->head;

        int serial = 1;
        while (iter) {
            iter->item->serial = serial;
            iter = iter->next;
            serial++;
        }
    }

    void display() {
        ElectronicToolsNode *iter = this->head;

        string border;
        border.assign(100, '-');

        cout << border << endl;
        cout << "Products: " << endl;
        while (iter) {
            cout << iter->item->serial << ". Product: " << iter->item->name << "\tprice: " << iter->item->price << endl;
            iter = iter->next;
        }
        cout << border << endl;
        cout << endl;
    }

    ElectronicToolsDetails * search(int serial) {
        ElectronicToolsNode *temp = this->head;

        while (temp) {

            if (temp->item->serial == serial) {
                return temp->item;
            }

            temp = temp->next;
        }

        return NULL;
    }

};

ElectronicToolsLinkedList * storedItems() {
    int serial = 1;

    ElectronicToolsDetails *new_items[] = {
        new ElectronicToolsDetails(
            "keyboard",
            serial++,
            "accessory",
            600.00
        ),
        new ElectronicToolsDetails(
            "mouse",
            serial++,
            "accessory",
            300.00
        ),
        new ElectronicToolsDetails(
            "monitor",
            serial++,
            "accessory",
            2000.00
        )
    };

    ElectronicToolsNode *new_node = new ElectronicToolsNode(
        &*new_items[0]
    );

    ElectronicToolsLinkedList *list = new ElectronicToolsLinkedList();

    list->addItem(&*new_node);

    for (int i = 1; i < 3; i++) {
        ElectronicToolsNode *temp_node = new ElectronicToolsNode(
                &*new_items[i]
        );

        list->addItem(&*temp_node);
    }

    return list;
}

bool check_access(string username, string password) {
    if (username == "admin" && password == "12345") {
        return true;
    } else {
        return false;
    }
}

int getIndex(vector<string> v, string K) {
    auto it = find(v.begin(), v.end(), K);

    if (it != v.end()) {
        int index = it - v.begin();

        return index;

    } else {

        return -1;
    }
}

vector <string> months = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

string getMonth(int month_number) {
    if (month_number > months.size()) {
        return "No month";
    } else {
        return months[month_number - 1];
    }
}

string dataProcess(queue <ElectronicToolsDetails *> customers_order) {
    vector<string> item_names;
    vector<int> item_count;
    vector<double> item_price;

    while (!customers_order.empty()) {

        if (count(item_names.begin(), item_names.end(), customers_order.front()->name)) {
            int index = getIndex(item_names, customers_order.front()->name);
            item_count[index] += 1;
        } else {
            item_names.push_back(customers_order.front()->name);
            item_count.push_back(1);
            item_price.push_back(customers_order.front()->price);
        }

        customers_order.pop();
    }

    double total_price = 0;
    string report_by_item;

    time_t now = time(0);

    tm *ltm = localtime(&now);

    string month = getMonth(1 + ltm->tm_mon);

    report_by_item += "Month: " + month + "\n";

    for (int i = 0; i < item_names.size(); i++) {
        double total_price_by_item = item_price[i] * item_count[i];
        total_price += total_price_by_item;

        stringstream stream_total_price_by_item;
        stream_total_price_by_item << fixed << setprecision(2) << total_price_by_item;
        string str_total_price_by_item = stream_total_price_by_item.str();

        report_by_item += "Product: " + item_names[i] + "\tQuantity: " + to_string(item_count[i]) +
                          "\tTotal Price: " + str_total_price_by_item + "\n";
    }

    stringstream stream_total_price;
    stream_total_price << fixed << setprecision(2) << total_price;
    string str_total_price = stream_total_price.str();

    report_by_item += "Total Price: " + str_total_price + "\n";

    return report_by_item;
}

int main() {
    int n;

    ElectronicToolsLinkedList *list = storedItems();

    stack<string> sales_report;
    stack<string> sales_report_copy;

    do {
        cout << "Pick User Profile: " << endl;
        cout << "1. Customer " << endl;
        cout << "2. Shop owner " << endl;
        cout << "3. Exit" << endl;
        cout << "Action: ";
        cin >> n;

        cout << endl;

        if (n == 1) {
            int item_serial, action, counter = 0;
            queue <ElectronicToolsDetails *> customers_order;

            do {
                list->display();

                cout << endl;
                cout << "Pick Action: " << endl;
                cout << "1. Add Product to the cart " << endl;
                cout << "2. Remove first Product" << endl;
                cout << "3. Exit " << endl;
                cout << "Action: ";

                cin >> action;

                if (action == 1) {
                    do {
                        cout << "Enter number of the item you'd like to buy (Enter -1 to exit): ";
                        cin >> item_serial;

                        if (item_serial == -1) {
                            cout << endl;
                            break;
                        }

                        ElectronicToolsDetails *required_item = list->search(item_serial);

                        customers_order.push(required_item);

                        cout << "Product Added" << endl;
                        cout << endl;

                        counter++;

                    } while (item_serial != -1);

                } else if (action == 2) {
                    if (!customers_order.empty()) {
                        customers_order.pop();
                        cout << "First product removed" << endl;
                        cout << endl;

                    } else {
                        cout << "No product in the cart" << endl;
                        cout << endl;
                    }

                    counter--;

                } else if (action == 3) {
                    cout << endl;
                    break;

                } else {
                    cout << "Wrong input" << endl;
                    cout << endl;
                }

            } while (action != 3);

            if (counter) {
                string report_by_item = dataProcess(customers_order);

                sales_report.push(report_by_item);

                cout << report_by_item;
            }

        } else if (n == 2) {
            string username;
            string password;

            cout << "username: ";
            cin >> username;

            cout << "password: ";
            cin >> password;

            if (check_access(username, password)) {

                int action;

                do {
                    list->display();
                    cout << endl;

                    cout << "Pick Action: " << endl;
                    cout << "1. Add Product " << endl;
                    cout << "2. Remove Product " << endl;
                    cout << "3. Last sold report " << endl;
                    cout << "4. Monthly report " << endl;
                    cout << "5. Exit " << endl;
                    cout << endl;

                    cout << "Action: ";
                    cin >> action;

                    if (action == 1) {
                        string product_name;
                        double price;

                        cout << "Enter product name: ";
                        cin >> product_name;

                        cout << "Enter product price: ";
                        cin >> price;

                        ElectronicToolsDetails *new_item = new ElectronicToolsDetails(
                                product_name,
                                list->last_serial,
                                price
                        );

                        ElectronicToolsNode *new_node = new ElectronicToolsNode(
                                &*new_item
                        );

                        list->addItem(&*new_node);

                        cout << "Product added" << endl;
                        cout << endl;

                    } else if (action == 2) {
                        list->display();

                        int serial;
                        cout << "Enter number for product you'd like to remove: ";
                        cin >> serial;

                        list->removeItem(serial);

                        cout << "Product Removed" << endl;
                        cout << endl;

                    } else if (action == 3) {
                        if (!sales_report.empty()) {
                            cout << sales_report.top() << endl;
                        } else {
                            cout << "No sales" << endl;
                        }

                        cout << endl;

                    } else if (action == 4) {
                        int month;

                        for (int i = 0; i < months.size(); i++) {
                            cout << i + 1 << ". " << months[i] << endl;
                        }

                        cout << "Enter month: ";
                        cin >> month;

                        vector <string> monthly_reports;

                        while (!sales_report.empty()) {
                            if (sales_report.top().find(months[month - 1]) == 7) {
                                monthly_reports.push_back(sales_report.top());
                            }

                            sales_report_copy.push(sales_report.top());
                            sales_report.pop();
                        }

                        if (monthly_reports.empty()) {
                            cout << "No monthly reports" << endl;
                        } else {
                            for (int i = 0; i < monthly_reports.size(); i++) {
                                cout << monthly_reports[i] << endl;
                                cout << endl;
                            }
                        }

                        while (!sales_report_copy.empty()) {
                            sales_report.push(sales_report_copy.top());
                            sales_report_copy.pop();
                        }

                        cout << endl;

                    } else if (action == 5) {
                        break;
                    } else {
                        cout << "Wrong input" << endl;
                        cout << endl;
                    }
                } while (action != 5);

            } else {
                cout << "Access denied" << endl;
            }

        } else if (n == 3) {
            cout << endl;
            break;
        } else {
            cout << "Wrong input" << endl;
        }

        cout << endl;

    } while (n != 3);

    return 0;
}
