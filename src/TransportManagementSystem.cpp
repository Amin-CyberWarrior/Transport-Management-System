#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

class Vehicle {
public:
    int vehicleId;
    string type;
    int capacity;
    int availableSeats;
    int routeId;  // Added: Vehicle must be assigned to a route
};

class Route {
public:
    int routeId;
    string source;
    string destination;
};

class Booking {
public:
    int bookingId;
    string passengerName;
    int routeId;
    int vehicleId;
};

vector<Vehicle> vehicles;
vector<Route> routes;
vector<Booking> bookings;

// File handling functions
void saveVehicles() {
    ofstream file("vehicles.txt");
    for (auto &v : vehicles)
        file << v.vehicleId << "|" << v.type << "|" << v.capacity << "|" << v.availableSeats << "|" << v.routeId << "\n";
}

void loadVehicles() {
    vehicles.clear();
    ifstream file("vehicles.txt");
    Vehicle v;
    string line;
    while (getline(file, line)) {
        size_t p1 = line.find('|'), p2 = line.find('|', p1+1), p3 = line.find('|', p2+1), p4 = line.find('|', p3+1);
        if(p1 == string::npos) continue;
        v.vehicleId = stoi(line.substr(0, p1));
        v.type = line.substr(p1+1, p2-p1-1);
        v.capacity = stoi(line.substr(p2+1, p3-p2-1));
        v.availableSeats = stoi(line.substr(p3+1, p4-p3-1));
        v.routeId = stoi(line.substr(p4+1));
        vehicles.push_back(v);
    }
}

void saveRoutes() {
    ofstream file("routes.txt");
    for (auto &r : routes)
        file << r.routeId << "|" << r.source << "|" << r.destination << "\n";
}

void loadRoutes() {
    routes.clear();
    ifstream file("routes.txt");
    Route r;
    string line;
    while (getline(file, line)) {
        size_t p1 = line.find('|'), p2 = line.find('|', p1+1);
        if(p1 == string::npos) continue;
        r.routeId = stoi(line.substr(0, p1));
        r.source = line.substr(p1+1, p2-p1-1);
        r.destination = line.substr(p2+1);
        routes.push_back(r);
    }
}

void saveBookings() {
    ofstream file("bookings.txt");
    for (auto &b : bookings)
        file << b.bookingId << "|" << b.passengerName << "|" << b.routeId << "|" << b.vehicleId << "\n";
}

void loadBookings() {
    bookings.clear();
    ifstream file("bookings.txt");
    Booking b;
    string line;
    while (getline(file, line)) {
        size_t p1 = line.find('|'), p2 = line.find('|', p1+1), p3 = line.find('|', p2+1);
        if(p1 == string::npos) continue;
        b.bookingId = stoi(line.substr(0, p1));
        b.passengerName = line.substr(p1+1, p2-p1-1);
        b.routeId = stoi(line.substr(p2+1, p3-p2-1));
        b.vehicleId = stoi(line.substr(p3+1));
        bookings.push_back(b);
    }
}

int generateBookingId() {
    int mx = 1000;
    for (auto &b : bookings)
        if (b.bookingId > mx) mx = b.bookingId;
    return mx + 1;
}

// Helper function to check if a route has any vehicles assigned
bool routeHasVehicles(int routeId) {
    for (auto &v : vehicles) {
        if (v.routeId == routeId) {
            return true;
        }
    }
    return false;
}

// Helper function to check if a route has any bookings
bool routeHasBookings(int routeId) {
    for (auto &b : bookings) {
        if (b.routeId == routeId) {
            return true;
        }
    }
    return false;
}

// Helper function to check if vehicle has any bookings
bool vehicleHasBookings(int vehicleId) {
    for (auto &b : bookings) {
        if (b.vehicleId == vehicleId) {
            return true;
        }
    }
    return false;
}

void viewRoutes() {
    cout << "\nRoute ID\tSource\t\tDestination\n";
    for (auto &r : routes)
        cout << r.routeId << "\t\t" << r.source << "\t\t" << r.destination << "\n";
}

void viewVehicles() {
    cout << "\nVehicle ID\tType\tCapacity\tAvailable\tRoute ID\n";
    for (auto &v : vehicles) {
        // Find route source/destination for display
        string routeInfo = "N/A";
        for (auto &r : routes) {
            if (r.routeId == v.routeId) {
                routeInfo = r.source + " -> " + r.destination;
                break;
            }
        }
        cout << v.vehicleId << "\t\t" << v.type << "\t" << v.capacity << "\t\t" 
             << v.availableSeats << "\t\t" << routeInfo << "\n";
    }
}

void viewVehiclesByRoute(int routeId) {
    cout << "\nVehicle ID\tType\tCapacity\tAvailable\n";
    bool found = false;
    for (auto &v : vehicles) {
        if (v.routeId == routeId) {
            cout << v.vehicleId << "\t\t" << v.type << "\t" << v.capacity << "\t\t" << v.availableSeats << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No vehicles available for this route.\n";
    }
}

void bookTicket() {
    // Validation: Check if routes exist
    if(routes.empty()) {
        cout << "No routes available. Cannot book ticket.\n";
        return;
    }

    // Validation: Check if vehicles exist
    if(vehicles.empty()) {
        cout << "No vehicles available. Cannot book ticket.\n";
        return;
    }
    
    Booking b;
    cout << "Passenger Name: ";
    getline(cin >> ws, b.passengerName);

    // Show available routes
    viewRoutes();
    cout << "Enter Route ID: ";
    cin >> b.routeId;
    
    // Validate Route ID
    bool routeFound = false;
    for(auto &r : routes) {
        if(r.routeId == b.routeId) {
            routeFound = true;
            break;
        }
    }
    
    if(!routeFound) {
        cout << "Invalid Route ID.\n";
        return;
    }

    // Show only vehicles assigned to the selected route
    cout << "\nVehicles available for selected route:\n";
    viewVehiclesByRoute(b.routeId);
    
    cout << "Enter Vehicle ID: ";
    cin >> b.vehicleId;

    // Validate Vehicle ID and ensure it belongs to the selected route
    bool vehicleFound = false;
    for (auto &v : vehicles) {
        if (v.vehicleId == b.vehicleId) {
            vehicleFound = true;
            // Check if vehicle belongs to selected route
            if (v.routeId != b.routeId) {
                cout << "Error: This vehicle does not belong to the selected route.\n";
                return;
            }
            // Check seat availability
            if (v.availableSeats <= 0) {
                cout << "No seats available on this vehicle.\n";
                return;
            }
            
            // Process booking
            b.bookingId = generateBookingId();
            bookings.push_back(b);
            v.availableSeats--;
            saveBookings();
            saveVehicles();
            cout << "Ticket Booked Successfully!\nBooking ID: " << b.bookingId << "\n";
            return;
        }
    }
    
    if (!vehicleFound) {
        cout << "Vehicle not found.\n";
    }
}

void cancelTicket() {
    int id;
    cout << "Enter Booking ID: ";
    cin >> id;

    for (size_t i = 0; i < bookings.size(); i++) {
        if (bookings[i].bookingId == id) {
            // Find the vehicle and increase available seats safely
            for (auto &v : vehicles) {
                if (v.vehicleId == bookings[i].vehicleId) {
                    // Ensure we don't exceed capacity
                    if (v.availableSeats < v.capacity) {
                        v.availableSeats++;
                    }
                    break;
                }
            }
            bookings.erase(bookings.begin() + i);
            saveBookings();
            saveVehicles();
            cout << "Booking Cancelled Successfully!\n";
            return;
        }
    }
    cout << "Booking not found.\n";
}

void searchBooking() {
    int id;
    cout << "Enter Booking ID: ";
    cin >> id;

    for (auto &b : bookings) {
        if (b.bookingId == id) {
            cout << "\nBooking ID: " << b.bookingId
                 << "\nPassenger: " << b.passengerName
                 << "\nRoute ID: " << b.routeId
                 << "\nVehicle ID: " << b.vehicleId << "\n";
            return;
        }
    }
    cout << "Booking not found.\n";
}

void viewAllBookings() {
    if (bookings.empty()) {
        cout << "\nNo bookings found.\n";
        return;
    }

    cout << "\nBookingID\tPassenger\tRoute\t\t\t\tVehicle\n";

    for (auto &b : bookings) {
        string routeInfo = "N/A";

        // Find matching route
        for (auto &r : routes) {
            if (r.routeId == b.routeId) {
                routeInfo = r.source + " -> " + r.destination;
                break;
            }
        }

        cout << b.bookingId << "\t\t"
             << b.passengerName << "\t\t"
             << b.routeId << " "
             << routeInfo << "\t"
             << b.vehicleId << "\n";
    }
}

void addVehicle() {
    Vehicle v;

    cout << "Enter Vehicle ID: ";
    cin >> v.vehicleId;

    // Check for duplicate Vehicle ID
    for(auto &veh : vehicles) {
        if(veh.vehicleId == v.vehicleId) {
            cout << "Error: Vehicle ID already exists.\n";
            return;
        }
    }

    cout << "Enter Type: ";
    cin >> v.type;

    cout << "Enter Capacity: ";
    cin >> v.capacity;

    // Validate capacity > 0
    if (v.capacity <= 0) {
        cout << "Error: Capacity must be greater than zero.\n";
        return;
    }

    // Display available routes
    if (routes.empty()) {
        cout << "Error: No routes available. Please add a route first.\n";
        return;
    }
    
    cout << "\nAvailable Routes:\n";
    viewRoutes();
    cout << "Enter Route ID for this vehicle: ";
    cin >> v.routeId;

    // Verify route exists
    bool routeExists = false;
    for (auto &r : routes) {
        if (r.routeId == v.routeId) {
            routeExists = true;
            break;
        }
    }

    if (!routeExists) {
        cout << "Error: Route ID does not exist. Vehicle not added.\n";
        return;
    }

    v.availableSeats = v.capacity;
    vehicles.push_back(v);
    saveVehicles();
    cout << "Vehicle Added Successfully.\n";
}

void updateVehicle() {
    int id;
    cout << "Enter Vehicle ID: ";
    cin >> id;

    for(auto &v : vehicles) {
        if(v.vehicleId == id) {
            string newType;
            int newCapacity;
            int newRouteId;

            cout << "Enter New Type: ";
            cin >> newType;

            cout << "Enter New Capacity: ";
            cin >> newCapacity;

            // Validate new capacity > 0
            if (newCapacity <= 0) {
                cout << "Error: Capacity must be greater than zero.\n";
                return;
            }

            int bookedSeats = v.capacity - v.availableSeats;
            
            // Don't allow reducing capacity below booked seats
            if(newCapacity < bookedSeats) {
                cout << "Error: Cannot reduce capacity below booked seats (" << bookedSeats << ").\n";
                return;
            }

            // Display available routes for update
            cout << "\nAvailable Routes:\n";
            viewRoutes();
            cout << "Enter New Route ID: ";
            cin >> newRouteId;

            // Verify new route exists
            bool routeExists = false;
            for (auto &r : routes) {
                if (r.routeId == newRouteId) {
                    routeExists = true;
                    break;
                }
            }

            if (!routeExists) {
                cout << "Error: Route ID does not exist. Vehicle not updated.\n";
                return;
            }

            v.type = newType;
            v.capacity = newCapacity;
            v.routeId = newRouteId;
            v.availableSeats = newCapacity - bookedSeats;

            saveVehicles();
            cout << "Vehicle Updated Successfully.\n";
            return;
        }
    }
    cout << "Vehicle not found.\n";
}

void deleteVehicle() {
    int id;
    cout << "Enter Vehicle ID: ";
    cin >> id;

    // Check if vehicle has active bookings
    if(vehicleHasBookings(id)) {
        cout << "Error: Cannot delete vehicle. Active bookings exist for this vehicle.\n";
        return;
    }

    for(size_t i = 0; i < vehicles.size(); i++) {
        if(vehicles[i].vehicleId == id) {
            vehicles.erase(vehicles.begin() + i);
            saveVehicles();
            cout << "Vehicle Deleted Successfully.\n";
            return;
        }
    }
    cout << "Vehicle not found.\n";
}

void addRoute() {
    Route r;

    cout << "Enter Route ID: ";
    cin >> r.routeId;

    // Check for duplicate Route ID
    for(auto &rt : routes) {
        if(rt.routeId == r.routeId) {
            cout << "Error: Route ID already exists.\n";
            return;
        }
    }

    cout << "Enter Source: ";
    getline(cin >> ws, r.source);

    cout << "Enter Destination: ";
    getline(cin >> ws, r.destination);

    routes.push_back(r);
    saveRoutes();
    cout << "Route Added Successfully.\n";
}

void updateRoute() {
    int id;
    cout << "Enter Route ID: ";
    cin >> id;
    
    for (auto &r : routes) {
        if (r.routeId == id) {
            cout << "Enter New Source: ";
            getline(cin >> ws, r.source);
            cout << "Enter New Destination: ";
            getline(cin >> ws, r.destination);
            saveRoutes();
            cout << "Route Updated Successfully.\n";
            return;
        }
    }
    cout << "Route not found.\n";
}

void deleteRoute() {
    int id;
    cout << "Enter Route ID: ";
    cin >> id;

    // Check if route has any vehicles assigned
    if(routeHasVehicles(id)) {
        cout << "Error: Cannot delete route. Vehicles are assigned to this route.\n";
        return;
    }

    // Check if route has any bookings
    if(routeHasBookings(id)) {
        cout << "Error: Cannot delete route. Active bookings exist for this route.\n";
        return;
    }

    for(size_t i = 0; i < routes.size(); i++) {
        if(routes[i].routeId == id) {
            routes.erase(routes.begin() + i);
            saveRoutes();
            cout << "Route Deleted Successfully.\n";
            return;
        }
    }
    cout << "Route not found.\n";
}

void reports() {
    int ch;
    do {
        cout << "\n===== Reports Menu =====\n";
        cout << "1. Total Vehicles\n2. Total Routes\n3. Total Bookings\n4. Available Seats Report\n5. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        
        switch(ch) {
            case 1: 
                cout << "Total Vehicles: " << vehicles.size() << "\n"; 
                break;
            case 2: 
                cout << "Total Routes: " << routes.size() << "\n"; 
                break;
            case 3: 
                cout << "Total Bookings: " << bookings.size() << "\n"; 
                break;
            case 4: 
                viewVehicles(); 
                break;
            case 5: 
                break;
            default: 
                cout << "Invalid Choice\n";
        }
    } while(ch != 5);
}

void vehicleMenu() {
    int ch;
    do {
        cout << "\n===== Vehicle Management =====\n";
        cout << "1. Add Vehicle\n2. View Vehicles\n3. Update Vehicle\n4. Delete Vehicle\n5. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        
        switch(ch) {
            case 1:
                addVehicle();
                break;
            case 2:
                viewVehicles();
                break;
            case 3:
                updateVehicle();
                break;
            case 4:
                deleteVehicle();
                break;
            case 5:
                break;
            default:
                cout << "Invalid Choice\n";
        }
    } while(ch != 5);
}

void routeMenu() {
    int ch;
    do {
        cout << "\n===== Route Management =====\n";
        cout << "1. Add Route\n2. View Routes\n3. Update Route\n4. Delete Route\n5. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        
        switch(ch) {
            case 1:
                addRoute();
                break;
            case 2:
                viewRoutes();
                break;
            case 3:
                updateRoute();
                break;
            case 4:
                deleteRoute();
                break;
            case 5:
                break;
            default:
                cout << "Invalid Choice\n";
        }
    } while(ch != 5);
}

void userMenu() {
    int ch;
    do {
        cout << "\n===== User Menu =====\n";
        cout << "1. View Routes\n2. View Vehicles\n3. Book Ticket\n4. Cancel Ticket\n5. View My Booking\n6. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        
        switch(ch) {
            case 1:
                viewRoutes();
                break;
            case 2:
                viewVehicles();
                break;
            case 3:
                bookTicket();
                break;
            case 4:
                cancelTicket();
                break;
            case 5:
                searchBooking();
                break;
            case 6:
                break;
            default:
                cout << "Invalid Choice\n";
        }
    } while(ch != 6);
}

bool adminLogin() {
    string username, password;
    string fileUser, filePass;

    ifstream file("admin.txt");
    
    // Check if file exists and read credentials
    if (file.is_open()) {
        getline(file, fileUser);
        getline(file, filePass);
        file.close();
    } else {
        // Create default admin account if file doesn't exist
        ofstream newFile("admin.txt");
        newFile << "admin\nadmin123\n";
        newFile.close();
        fileUser = "admin";
        filePass = "admin123";
    }

    cout << "\n===== Admin Login =====\n";
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if(username == fileUser && password == filePass) {
        cout << "\nLogin Successful!\n";
        return true;
    }

    cout << "\nInvalid Username or Password!\n";
    return false;
}

void changeAdminPassword() {
    string username;
    string oldPassword;
    string fileUser;
    string filePass;

    ifstream infile("admin.txt");
    
    if (!infile.is_open()) {
        cout << "\nError: Admin file not found.\n";
        return;
    }

    getline(infile, fileUser);
    getline(infile, filePass);
    infile.close();

    cout << "\n===== Change Admin Password =====\n";
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Current Password: ";
    cin >> oldPassword;

    if(username != fileUser || oldPassword != filePass) {
        cout << "\nIncorrect Username or Password!\n";
        return;
    }

    string newPassword;
    string confirmPassword;

    cout << "Enter New Password: ";
    cin >> newPassword;
    cout << "Confirm New Password: ";
    cin >> confirmPassword;

    if(newPassword != confirmPassword) {
        cout << "\nPasswords do not match!\n";
        return;
    }

    ofstream outfile("admin.txt");
    outfile << fileUser << endl;
    outfile << newPassword << endl;
    outfile.close();

    cout << "\nPassword Changed Successfully!\n";
}

void adminMenu() {
    int ch;
    do {
        cout << "\n===== Admin Menu =====\n";
        cout << "1. Manage Vehicles\n2. Manage Routes\n3. View All Bookings\n4. Search Booking\n5. Reports\n6. Change Admin Password\n7. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        
        switch(ch) {
            case 1:
                vehicleMenu();
                break;
            case 2:
                routeMenu();
                break;
            case 3:
                viewAllBookings();
                break;
            case 4:
                searchBooking();
                break;
            case 5:
                reports();
                break;
            case 6:
                changeAdminPassword();
                break;
            case 7:
                break;
            default:
                cout << "Invalid Choice\n";
        }
    } while(ch != 7);
}

int main() {
    // Load all data
    loadVehicles();
    loadRoutes();
    loadBookings();

    int choice;
    do {
        cout << "\n===== Transport Management System =====\n";
        cout << "1. User\n2. Admin\n3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                userMenu();
                break;
            case 2:
                if (adminLogin()) {
                    adminMenu();
                }
                break;
            case 3:
                cout << "\nThank You for using Transport Management System!\nGoodbye!\n";
                break;
            default:
                cout << "Invalid choice\n";
        }
    } while(choice != 3);

    return 0;
}