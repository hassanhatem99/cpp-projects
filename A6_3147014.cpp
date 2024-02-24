/*
Matr.-Nr:                     3147014
Nachname/Surname:             Mostafa
Vorname/Given name:           Hassan
Uni-Email:                    hassan.mostafa@stud.uni-due.de
Studiengang/Course of studis: ISE Software Engineering
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <functional>
using namespace std;

int width = 35; 
int consumption_per_person_ewh = 550;
int consumption_per_person_no_ewh = 200;
int consumption_per_squaremeter = 9;
int consumption_per_electrical_device = 200;
int this_year = 2024;

enum Use{
    once,
    daily, 
    mo_fr,
    sa_su, 
    weekly
};

class Consumer;
class Household;
class Device;
float annual_power_cost_device(Device *device, Household* household);
void input_electric_water_heating(bool& answer);
void input_integer(string question, int& value);
void input_float(string question, float& value);
void input_city(string& city);
void input_string(string question, string& value);

class Address {
private:
    string street;
    string no;
    string zip;
    string city;

public:
    // Public constructor with default values
    Address(string street = "", string no = "", string zip = "", string city = "")
        : street(street), no(no), zip(zip), city(city) {}

    ~Address() {
        if (!(street.empty() && no.empty() && zip.empty() && city.empty()))
            cout << "Address " << to_string() << " at address " << this << " deleted\n";
        street = no = zip = city = "";
    }
    // Getter methods
    string getStreet() const { return street; }
    string getNumber() const { return no; }
    string getZipCode() const { return zip; }
    string getCity() const { return city; }

    // Input method to input the four attributes from standard input
    void input() {
        cout << "\nWhat is the street name? ";
        cin.ignore();
        getline(cin, street);

        cout << "\nWhat is house number? ";
        getline(cin, no);

        cout << "\nWhat is zip code? ";
        getline(cin, zip);

        cout << "\nWhat is the city name? ";
        getline(cin, city);
    }

    // Method to return the address as a single string
    string to_string() const {
        return street + " " + no + ", " + zip + " " + city;
    }

    bool operator==(Address &otherAddress){
        return (street == otherAddress.street) &&
               (street == otherAddress.no) &&
               (street == otherAddress.zip) &&
               (street == otherAddress.city);
               
    }
};




class Device {
    private:
        string description;
        Device* next;
    public:
        Device(Device* ptr = nullptr) : next(ptr) {}

        virtual ~Device() {
            cout << "Device " << description << " at address " << this << " deleted\n";
        }

        virtual void print(int n, float power_price) = 0;
        virtual float annual_kWh() = 0;
        virtual Device* get_a_copy() = 0;
        virtual string get_string_for_file(char sep) = 0;

        string getDescription() const { return description; }
        void setDescription(const string& desc) { description = desc; }  

        Device* getNext() const { return next; }   
        void setNext(Device* n) { next = n; }

        virtual void input() {
            cout << "\nWhat is the description of the Device? ";
            cin.ignore();
            getline(cin, description);
        }
        void print(int n) {
            cout << right << setw(width-2) << n << ": " << left << getDescription() << endl;
            cout << right << setw(width) << "(this: " << left << this << ")\n";
        }
};



class Consumer : public Device {
    private:
        float watt;
        static int total_number;
        
    public:
        Consumer(Device* ptr = nullptr) : watt(0.0), use(once){
            total_number += 1;
        }
        ~Consumer() override {
            total_number -= 1;
            cout << "Consumer " << getDescription() << " at address " << this << " deleted\n";
        }
        enum Use { once, daily, mo_fr, sa_su, weekly } use;
        string use_to_string(Use u){
            if (u == once) {return "once";}
            if (u == daily) { return "daily";}
            if (u == mo_fr) { return "Monday to Friday";}
            if (u == sa_su) { return "Saturday and Sunday";}
            if (u == weekly) { return "Weekly";}
            return "";
        }
        // Getter and Setter methods for Consumer

        float getWatt() const { return watt; }
        void setWatt(float w) { watt = w; }

        Use getUse() const { return use; }
        void setUse(Use u) { use = u; }

        static int getTotalNumber() { return total_number; }
        void setTotalNumber (int t) { total_number = t; }


        void input() override {
            Device::input(); // Call the input function of the base class (Device)
            
            cout << "\nHow many watt it will have? ";
            cin >> watt;

            char input;
            cout << "\nHow often it will be used? ";
            cout << "\ndaily  (d) \nmo_fr  (m) \nonce   (o) \nsa_su  (s) \nweekly (w)? ";
            cin >> input;

            switch (input) {
                case 'o':
                case 'O':
                    use = once;
                    break;
                case 'd':
                case 'D':
                    use = daily;
                    break;
                case 'm':
                case 'M':
                    use = mo_fr;
                    break;
                case 's':
                case 'S':
                    use = sa_su;
                    break;
                case 'w':
                case 'W':
                    use = weekly;
                    break;
                default:
                    cout << "Invalid input" << endl;
            }
        }

        void print(int n) {
            Device::print(n);
            cout << right << setw(width) << "power consumption: " << left << getWatt() << " W" << endl;
        }
    };



class Immobile : public Consumer {
    private:
        float watt_standby;
        float hours;


    public:

    Immobile(Device* ptr = nullptr) : watt_standby(0.0), hours(0.0){
    }
    
    ~Immobile() override {
        cout << "Immobile " << getDescription() << " at address " << this << " deleted\n"; 
    }

        float getWattStandby() const { return watt_standby; }
        void setWattStandby(float ws) { watt_standby = ws; }

        float getHours() const { return hours; }
        void setHours(float h) { hours = h; }

        float annualHoursOfUse() {
            float annualUseHours = hours;
            if (use == daily)
                annualUseHours *= 365;
            else if (use == weekly)
                annualUseHours *= 52;
            else if (use == sa_su)
                annualUseHours *= 104;
            else if (use == mo_fr)
                annualUseHours *= 260;
            return annualUseHours;
        }

        float annualHoursOfStandby() {
            float annualUseHours = annualHoursOfUse();
            return 8760 - annualUseHours;
        }

        float annual_kWh() {
            return (annualHoursOfUse() * getWatt() + annualHoursOfStandby() * watt_standby) / 1000;
        }


        void print(int n, float power_price) {
            Consumer::print(n);
            cout << right << setw(width) << "power consumption: " << left << getWatt() << " W" << endl;
            cout << right << setw(width) << "power consumption standby: " << left << getWattStandby() << " W" << endl;
            cout << right << setw(width) << "annual hours of use: " << left << annualHoursOfUse() << " h" << endl;
            cout << right << setw(width) << "annual hours of standby: " << left << annualHoursOfStandby() << " h" << endl;
            cout << right << setw(width) << "annual consumption: " << left << annual_kWh() << " kWh" << endl;
            cout << right << setw(width) << "annual costs: " << annual_kWh() * power_price << " EUR" << endl;
        }

        void input() override {
            Consumer::input(); // Call the input function of the base class (Device)
            input_float("How many watt standby it will have? ", watt_standby);
        }


        Device* get_a_copy() {
            Immobile* copy = new Immobile;
            copy->setDescription(getDescription());
            copy->setWatt(getWatt());
            copy->setWattStandby(getWattStandby());
            copy->setHours(getHours());
            copy->setUse(getUse());
            return copy;

        }

        string get_string_for_file(char sep) {
            string type = "Immobile";
            return type+sep+Device::getDescription()+sep+to_string(hours)+sep+use_to_string(Consumer::getUse())+sep+to_string(Consumer::getWatt())+sep+to_string(getWattStandby());
        }
        
};

class Mobile : public Consumer {
    private:
        float km;

    public:
    Mobile(Device* ptr = nullptr) : km(0.0){}
    ~Mobile() override {
        cout << "Mobile " << getDescription() << " at address " << this << " deleted\n";
    }

    float getKm() const { return km ;}
    void setKm(float k) { km = k; }

    void input() override {
        Consumer::input();
        cout << "\nHow many km? ";
        cin >> km;
    }
    

    float annual_kWh() {
        float a = km * getWatt();
        if (use == daily)
            a*= 365;
        else if (use == weekly)
            a*= 52;
        else if (use == sa_su)
            a*= 104;
        else if (use == mo_fr)
            a*= 260;
        return a/100000;        
    }

    Device* get_a_copy() {
        Mobile* copy = new Mobile;
        copy->setDescription(getDescription());
        copy->setWatt(getWatt());
        copy->setKm(km);
        copy->setUse(getUse());
        return copy;
    }

    string get_string_for_file(char sep) {
        string type = "Mobile";
        return type+sep+Device::getDescription()+sep+to_string(km)+sep+use_to_string(Consumer::getUse())+sep+to_string(Consumer::getWatt());
    }

    void print(int n, float power_price) {
        Consumer::print(n);
        cout << right << setw(width) << "km driven: " << left << km << " " << use_to_string(getUse()) << endl;
        cout << right << setw(width) << "power consumption 100 km: " << left << getWatt()/100 << " W" << endl;
        cout << right << setw(width) << "annual consumption: " << left << annual_kWh() << " kWh" << endl;
        cout << right << setw(width) << "annual costs: " << annual_kWh() * power_price << " EUR" << endl;

    }

};


class Producer : public Device {

    public:
    Producer(Device* ptr = nullptr) {}
    ~Producer() override {
        cout << "Producer " << getDescription() << " at address " << this << " deleted\n";
    }
    void input () override {
        Device::input();
    }

};

class Solar : public Producer {
    private:
        float watt_peak;
        int year;

    public:
    Solar(Device* ptr = nullptr) : watt_peak(0.0), year(this_year) {}
    ~Solar() { cout << "Solar " << getDescription() << " at address " << this << " deleted\n"; }

    float getWattPeak() const { return watt_peak; }
    void setWattPeak(float wp) { watt_peak = wp; }

    int getYear() { return year; }
    void setYear(int y) { year = y; }

    void input() override {
        Producer::input();
        input_float("\nWatt Peak? ", watt_peak);
        input_integer("Year? ", year);
    }

    float annual_kWh() {
        int age = this_year-year;
        float wp = watt_peak;
        for (int i = 0; i < age; i++){
            wp *= (99.5/100);
        }
        return wp*-1;
    }

    void print(int n, float power_price) {
        cout << right << setw(width-2) << n << ": " << left << getDescription() << endl;
        cout << right << setw(width) << "(this: " << left << this << ")\n";
        cout << right << setw(width) << "solar cells installed power: " << left << watt_peak << " W" << endl;
        cout << right << setw(width) << "age of solar cells:: " << left << this_year-year << " years" << endl;
        cout << right << setw(width) << "annual production: " << left << annual_kWh() << " kWh" << endl;
        cout << right << setw(width) << "annual savings: " << annual_kWh() * power_price << "EUR" << endl;
    }

    Device* get_a_copy(){
        Solar* copy = new Solar;
        copy->setDescription(getDescription());
        copy->setWattPeak(getWattPeak());
        copy->setYear(getYear());

        return copy;
    }

    string get_string_for_file(char sep) {
        string type = "Solar";
        return type+sep+getDescription()+sep+to_string(getYear())+sep+to_string(watt_peak);
    } 

};


class Household;

class Household {
private:
    int persons;
    int square_meters;
    Device* devices;
    bool heated_electrically;
    float power_price;
    string supplier;

public:
    Household() : persons(0), square_meters(0), devices(nullptr), heated_electrically(false), power_price(0.0), supplier("") {}

    ~Household() {
        Device* current_device = devices;
        while (current_device != nullptr) {
            Device* temp = current_device;
            current_device = current_device->getNext();
            delete temp;
        }
        persons = square_meters = 0;
        cout << "Household at address " << this << " deleted\n";
    }

    // Getter and Setter methods for Household
    int getPersons() const { return persons; }
    void setPersons(int p) { persons = p; }

    int getSquareMeters() const { return square_meters; }
    void setSquareMeters(int sqm) { square_meters = sqm; }

    Device* getDevices() const { return devices; }
    void setDevices(Device* dev) { devices = dev; }

    bool getHeatedElectrically() const { return heated_electrically; }
    void setHeatedElectrically(bool heated) { heated_electrically = heated; }

    float getPowerPrice() const { return power_price; }
    void setPowerPrice(float price) { power_price = price; }

    string getSupplier() const { return supplier; }
    void setSupplier(const string& sup) { supplier = sup; }

    bool exists(){
        return (getPersons() != 0) &&
               (getSquareMeters() != 0) &&
               (getDevices() != nullptr) &&
               (getPowerPrice() != 0.0) &&
               (getSupplier() != "");
    }

    // Public member function to input household data from standard input
    void input() {
        input_integer("\nHow many square meters does the household have? ", square_meters);
        input_integer("\nHow many persons live in this household? ", persons);
        input_electric_water_heating(heated_electrically);
        input_float("\nWhat is the price for one kWh in EUR? ", power_price);
        input_string("\nWho is the power supplier? ", supplier);
        devices = nullptr;
    }

    // Public member function to print household data to standard output
    void print(int household_number) {
        cout << "\n\nH O U S E H O L D   N O   " << household_number << "   P O W E R   C O N S U M P T I O N\n";
        cout << "----------------------------------------------------------------------\n";
        cout << right << setw(width) << "(this: " << left << this << ")\n";
        cout << right << setw(width) << "Price for one kWh: " << left << power_price * 100 << " ct/kWh" << endl;
        cout << right << setw(width) << "Power Supplier: " << left << supplier << endl;
        cout << right << setw(width) << "Square metres: " << left << square_meters << " qm" << endl;
        cout << right << setw(width) << "Persons: " << left << persons << endl;

        // Handle if water is heated electrically
        string yes_or_no = "no";
        float consumption_per_person = consumption_per_person_no_ewh;
        if (heated_electrically != 0) {
            yes_or_no = "yes";
            consumption_per_person = consumption_per_person_ewh;
        }
        cout << right << setw(width) << "Water heated using electricity: " << left << setw(width) << yes_or_no << endl;

        cout << right << setw(width) << "List of devices\n";
        Device* current_device = devices;

        int n = 1;
        float total_annual_consumption = ((consumption_per_squaremeter * square_meters) + (consumption_per_person * persons));
        float total_annual_cost = total_annual_consumption * power_price;

        cout << "----------------------------------------------------------------------\n";
        while (current_device != nullptr) {
            float annual_kwh_device = current_device->annual_kWh();
            float annual_cost_device = annual_power_cost_device(current_device, this);
            current_device->print(n, power_price);
            total_annual_consumption += annual_kwh_device;
            total_annual_cost += annual_cost_device;

            // Move to the next device in the linked list
            current_device = current_device->getNext();
            n++;
        }
        cout << "----------------------------------------------------------------------\n";
        cout << right << setw(width) << "power consumption square meters: " << left << consumption_per_squaremeter * square_meters << " kWh" << endl;
        cout << right << setw(width) << "power consumption for all persons: " << left << consumption_per_person * persons << " kWh" << endl;
        cout << right << setw(width) << "total annual power consumption: " << left << total_annual_consumption << " kWh" << endl;
        cout << right << setw(width) << "total annual power costs: " << left << total_annual_cost << " EUR";
    }


    // Public member function for moving a consumer up in the list
    Device* move_up_device(Device* device, int k) {
        if (!device || k < 1) {
            // Invalid input or trying to move an element that doesn't exist
            return device;
        }

        if (k == 1) {
            // Move the first element to the end of the list
            Device* current = device;
            Device* first = current;
            device = device->getNext();

            while (current->getNext()) {
                current = current->getNext();
            }

            current->setNext(first);
            first->setNext(nullptr);
            return device;
        }

        Device* prev = nullptr;
        Device* current = device;
        int count = 1;

        // Traverse the linked list to find the (k-1)th element
        while (current && count < k - 1) {
            prev = current;
            current = current->getNext();
            count++;
        }

        if (count < k - 1 || !current) {
            // If (k-1) is greater than the number of elements or (k-1) is out of bounds, nothing to do
            return device;
        }

        // Unlink the kth element from the list
        Device* target = current->getNext();
        if (!target) {
            // If target is nullptr, there's nothing to move
            return device;
        }

        current->setNext(target->getNext());

        // Insert the kth element one position before
        target->setNext(current);
        if (k == 2) {
            // If k is 2, update the head of the list
            return target;
        } else {
            // Update the previous element's next pointer
            prev->setNext(target);
            return device;
        }
    }
    
    // Public member function to copy consumers from one household to another
    void copy_devices(Household* household_from) {
        Device* current = household_from->getDevices();

        Device* copy_rev = nullptr;
        while (current != nullptr) {
            Device* New = current->get_a_copy();

            New->setNext(copy_rev);
            copy_rev = New;
            current = current->getNext();
        }

        current = copy_rev;
        while (current != nullptr) {
            Device* New = current->get_a_copy();

            New->setNext(devices);
            devices = New;
            current = current->getNext();
        }


        
    }  

    void add_device_to_household(Device* device) {
        device->setNext(devices);
        devices = device;
    }   

    // Public member function to add a consumer to the end of the list
    void add_device_to_household_end(Device* device) {
        device->setNext(nullptr); // Ensure that the new consumer is the last in the list

        if (devices == nullptr) {
            // If the household has no consumers, set the new consumer as the first one
            devices = device;
        } else {
            // Traverse the list to find the last consumer and add the new consumer at the end
            Device* current_device = devices;
            while (current_device->getNext() != nullptr) {
                current_device = current_device->getNext();
            }
            current_device->setNext(device);
        }
    }

};



class House {
private:
    int number_of_households;
    Address address;
public:
    Household** household;
    // Constructor
    House(int num, const Address& addr) : number_of_households(num), address(addr) {
        household = new Household*[number_of_households];
        for (int i = 0; i < number_of_households; ++i) {
            household[i] = nullptr;
        }
    }

    ~House() {
        for (int i = 0; i < number_of_households; i++) {
            if (household[i] != nullptr) {
                delete household[i];
                household[i] = nullptr;
            }
        }
        cout << "House at address " << this << " deleted\n";
    }

    // Public method to get the n-th household
    Household* get_household(int n) const {
        if (n >= 0 && n < number_of_households) {
            return household[n];
        }
        return nullptr;
    }

    // Overloaded unary operator[] with the same functionality
    Household* operator[](int n) const {
        return get_household(n);
    }

    // Public method to set the n-th household
    void set_household(Household* h, int n) {
        if (n >= 0 && n < number_of_households) {
            delete household[n];
            household[n] = h;
        }
    }

    int getCount(){
        return number_of_households;
    }

    void setCount(const int& c) {
        number_of_households = c;
        household = new Household*[number_of_households];
    }

    Address getAddress() const {
        return address;
    }

    void setAddress(const Address& addr) {
        address = addr;
    }

    bool operator==(House &otherHouse){
        return (address == otherHouse.address) &&
               (number_of_households == otherHouse.number_of_households);
    }

    void print_all() const {
        cout << "======================================================================\n";
        cout << "                             H O U S E                                \n";
        cout << "======================================================================\n";
        cout << right << setw(width) << "(this: " << left << this << ")\n"; 
        cout << right << setw(width) << "address: " << left << address.to_string() << endl;
        cout << right << setw(width) << "number of households: " << left << number_of_households << endl;
        cout << right << setw(width) << "total number of consumers: " << left << Consumer::getTotalNumber() << endl;
        cout << "----------------------------------------------------------------------";

        for (int i = 0; i < number_of_households; i++) {
            if (household[i] != nullptr)
                household[i]->print(i);
        }
        cout << "\n======================================================================\n";
    }

    bool read_data_from_file(string& file_name, char separator) {
        ifstream infile(file_name);
        if (!infile.is_open()) {
            cout << "Error opening file: " << file_name << endl;
            return false;
        }

        string task, count_field, street, no, zip, city;
        getline(infile, task, separator);
        getline(infile, count_field, separator);
        getline(infile, street, separator);
        getline(infile, no, separator);
        getline(infile, zip, separator);
        getline(infile, city, '\n');

        Address address(street, no, zip, city);
        this->setCount(stoi(count_field));
        this->setAddress(address);
        
        // Loop until the end of the file
        while (!infile.eof()) {
            string line;
            getline(infile, line, '\n');
            stringstream line_stream(line);

            // Trim whitespace, including newline characters, from the type string
            string type;
            getline(line_stream >> ws, type, separator);

            int index;
            if (type == "Household") {
                string index_str, heated_str, persons_str, area_str, power_price_str, supplier_str;
                getline(line_stream, index_str, separator);
                getline(line_stream, heated_str, separator);
                getline(line_stream, persons_str, separator);
                getline(line_stream, area_str, separator);
                getline(line_stream, power_price_str, separator);
                getline(line_stream, supplier_str, '\n');

                index = stoi(index_str);
                household[index] = new Household;
                household[index]->setPersons(stoi(persons_str));
                household[index]->setHeatedElectrically(heated_str == "true");
                household[index]->setSquareMeters(stoi(area_str));
                household[index]->setPowerPrice(stof(power_price_str));
                household[index]->setSupplier(supplier_str);
                household[index]->setDevices(nullptr);

            } else if (type == "Immobile") {
                Immobile* immobile = new Immobile;

                string description, hours_str, use_str, watt_str, standby_str;
                getline(line_stream, description, separator);
                getline(line_stream, hours_str, separator);
                getline(line_stream, use_str, separator);
                getline(line_stream, watt_str, separator);
                getline(line_stream, standby_str, '\n');

                immobile->setDescription(description);
                immobile->setHours(stof(hours_str));
                immobile->setWatt(stof(watt_str));
                immobile->setWattStandby(stof(standby_str));

                // Process the 'use' string and set the enum
                if (use_str == "once") immobile->setUse(Consumer::once);
                else if (use_str == "daily") immobile->setUse(Consumer::daily);
                else if (use_str == "Monday to Friday") immobile->setUse(Consumer::mo_fr);
                else if (use_str == "Saturday and Sunday") immobile->setUse(Consumer::sa_su);
                else if (use_str == "weekly") immobile->setUse(Consumer::weekly);

                // Add the consumer to the current household
                household[index]->add_device_to_household_end(immobile);
            }

            else if (type == "Mobile") {
                Mobile* mobile = new Mobile;

                string description, km_str, use_str, watt_str;
                getline(line_stream, description, separator);
                getline(line_stream, km_str, separator);
                getline(line_stream, use_str, separator);
                getline(line_stream, watt_str, '\n');

                mobile->setDescription(description);
                mobile->setWatt(stof(watt_str));
                mobile->setKm(stof(km_str));

                // Process the 'use' string and set the enum
                if (use_str == "once") mobile->setUse(Consumer::once);
                else if (use_str == "daily") mobile->setUse(Consumer::daily);
                else if (use_str == "Monday to Friday") mobile->setUse(Consumer::mo_fr);
                else if (use_str == "Saturday and Sunday") mobile->setUse(Consumer::sa_su);
                else if (use_str == "weekly") mobile->setUse(Consumer::weekly);

                // Add the consumer to the current household
                household[index]->add_device_to_household_end(mobile);
            }

            else if (type == "Solar") {
                Solar* solar = new Solar;

                string description, year_str, watt_peak_str;
                getline(line_stream, description, separator);
                getline(line_stream, year_str, separator);
                getline(line_stream, watt_peak_str, '\n');

                solar->setDescription(description);
                solar->setYear(stoi(year_str));
                solar->setWattPeak(stof(watt_peak_str));

                // Add the consumer to the current household
                household[index]->add_device_to_household_end(solar);
            }
        }

        infile.close();
        return true;
    }

    bool write_data_to_file(const string& file_name, char separator) const {
        ofstream outfile(file_name);
        if (!outfile.is_open()) {
            cerr << "Error opening file for writing: " << file_name << endl;
            return false;
        }

        outfile << "A6" << separator << number_of_households << separator << address.getStreet() << separator << address.getNumber() << separator << address.getZipCode() << separator << address.getCity() << endl;

        for (int i = 0; i < number_of_households; ++i) {
            if (household[i]->exists()){
                outfile << "Household" << separator << i << separator << (household[i]->getHeatedElectrically()? "true" : "false") << separator << household[i]->getPersons() << separator << household[i]->getSquareMeters() << separator << household[i]->getPowerPrice() << separator << household[i]->getSupplier() << endl;
            }

            Device* current_device = household[i]->getDevices();
            while (current_device != nullptr) {
                outfile << current_device->get_string_for_file(separator) << endl;

                current_device = current_device->getNext();
            }

        }

        outfile.close();
        return true;
    }
};


int Consumer::total_number = 0;
int main() {
    cout << "CALCULATION OF AVERAGE POWER COST FOR A HOUSE - CLASS VERSION\n";
    char action;
    int index;
    int position;
    Address defaultAddress;
    House* myHouse = new House(0, defaultAddress);
    int household_count = 0;

    while (true)
    {
        cout << "\n(q) quit\n(d) delete house \n(h) house initialisation \n(i) input immobile consumer\n(m) input mobile consumer \n(s) input solar producer \n(u) move up device\n(p) print household\n(a) print all households\n(n) new household\n(c) copy all devices (added to already existing ones)\n(r) read data from file \n(w) write data into file \n>> ";
        cin >> action;

        if (action == 'q')
            break;
        else if (action == 'd')
            delete myHouse;
        else if (action == 'h')
        {
            cout << "How many households does the house have? ";
            cin >> household_count;
            Address houseAddress;
            houseAddress.input();

            myHouse->setAddress(houseAddress);
            myHouse->setCount(household_count);
        }
        else if (action == 'i')
        {
            cout << "Number of household? ";
            cin >> index;

            if (myHouse->household[index] == nullptr) {
                myHouse->set_household(new Household, index);
            }

            // Input consumer data
            Immobile* consumer = new Immobile;
            consumer->input();
            myHouse->household[index]->add_device_to_household(consumer);
        }
        else if (action == 'm')
        {
            cout << "Number of household? ";
            cin >> index;

            if (myHouse->household[index] == nullptr) {
                myHouse->set_household(new Household, index);
            }

            // Input consumer data
            Mobile* consumer = new Mobile;
            consumer->input();
            myHouse->household[index]->add_device_to_household(consumer);
        }
        else if (action == 's')
        {
            cout << "Number of household? ";
            cin >> index;

            if (myHouse->household[index] == nullptr) {
                myHouse->set_household(new Household, index);
            }

            // Input consumer data
            Solar* consumer = new Solar;
            consumer->input();
            myHouse->household[index]->add_device_to_household(consumer);
        }
        else if (action == 'u')
        {
            cout << "Number of household? ";
            cin >> index;
            cout << "\nWhich one? ";
            cin >> position;
            Household* h = myHouse->household[index];
            h->setDevices(h->move_up_device(h->getDevices(), position)); 
        }
        else if (action == 'p')
        {
            cout << "Number of Household? ";
            cin >> index;
            myHouse->household[index]->print(index-1);
        }
        else if (action == 'a')
        {
            if (myHouse->getAddress() == defaultAddress && myHouse->getCount() == 0) {
                cout << "\nhouse is a nullptr, please first choose h to initialise a new house";
            }
            else
                myHouse->print_all();
        }
        else if (action == 'n')
        {
            input_integer("\nNumber of household? ", index);

            if (myHouse->household[index] == nullptr) {
                myHouse->set_household(new Household, index);
                myHouse->household[index]->input();
            }
        }
        else if (action == 'c')
        {
            cout << "\nNumber of household from which to copy from? ";
            cin >> index;
            cout << "\nNumber of household to copy to? ";
            int index_to;
            cin >> index_to;
            Household* h1 = myHouse->household[index];
            Household* h2 = myHouse->household[index_to];
            h2->copy_devices(h1);
        }
        else if (action == 'w')
        {
            string file_name;
            char separator;
            cout << "Input file name: ";
            cin >> file_name;
            cout << "Input separator character: ";
            cin >> separator;

            for (int i = 0; i < myHouse->getCount(); i++){
                if (myHouse->household[i] == nullptr) {
                    myHouse->set_household(new Household, i);
                } 
            }
            if (myHouse->write_data_to_file(file_name, separator))
            {
                cout << "input file \"" << file_name << "\" opened...\n";
                cout << "input file \"" << file_name << "\" closed";
            }
            else
            {
                cout << "Error writing data to file." << endl;
            }
        }
        else if (action == 'r')
        {
            House fileHouse(0, defaultAddress);
            string file_name;
            char separator;
            cout << "Input file name: ";
            cin >> file_name;
            cout << "Input separator character: ";
            cin >> separator;

            int household_count_2;
            if (myHouse->read_data_from_file(file_name, separator))
            {
                cout << "input file \"" << file_name << "\" opened...\n";
                if (myHouse->getCount() < fileHouse.getCount())
                {
                    cout << "file contains up to " << household_count_2 << " households, but here only " << household_count << " are supported\n";
                }
                cout << "input file \"" << file_name << "\" closed";
            }
            else
            {
                cout << "Error reading data from file." << endl;
            }
        }
        else
            cout << "Sorry wrong choice\n";
    }

    // Deallocate memory for households
    for (int i = 0; i < household_count; i++)
    {
        delete myHouse->household[i];
    }

    return 0;
}





































float annual_power_cost_device(Device *device, Household* household) {
    return device->annual_kWh() * household->getPowerPrice();
}

void input_electric_water_heating(bool& answer){
    string yes_or_no;
    cout << "\nIs hot water heated using electricity? (y(es) or n(o)) ";
    cin >> yes_or_no;
    if (yes_or_no == "y")
        answer = true;
    else if (yes_or_no == "n")
        answer = false;
}

void input_integer(string question, int& value){
    cout << question;
    cin >> value;
}

void input_float(string question, float& value){
    cout << question;
    cin >> value;
}


void input_city(string& city){
    cout << "\nIn which city is the household located? ";
    cin.ignore();  
    getline(cin, city);  
}


void input_string(string question, string& value){
    cout << question;
    cin.ignore();
    getline(cin, value);
}