#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<cmath>
#include "flighthashtable.h"
#include "Iterator.h"
using namespace std;

FlightHASHTABLE::FlightHASHTABLE(int capacity)//constructor
{
	buckets = new DoublyLinkedList<HashNode>[capacity];	  //Array of a list/chain
	this->capacity = capacity;
	this->size = 0;
	this->collisions = 0;
}
FlightHASHTABLE::~FlightHASHTABLE()//destructor
{
    for(int i=0; i<capacity; i++)
        while(!buckets[i].empty()) buckets[i].removeBack();
    delete[] this->buckets;
}
long FlightHASHTABLE::hashCode(string key) //implement and test different hash functions
{
    long size = key.length(), sum = 0;
	// polynomial
    for(int i = 0; i < size; i++){
		int temp = i % 5; // limit the size of i so pow won't explode
		int tempKey = key[i]*pow(11, temp); // polynomial
		sum += tempKey % capacity; // mod capacity if tempKey is huge
    }
    return sum %= capacity;
}

int FlightHASHTABLE::importCSV(string path)//Load all the flight-tickets from the CSV file to the HashTable
{
    ifstream fin;
    fin.open(path, ios::in); //, ios::out | ios::app
    // return error if file cannot be opened
    if(!fin){
        cout<<"Can not open the file <" << path << ">...!"<<endl;
        return -1;
    }
    
    int counter = 0;
    string line, x[9];
    getline(fin,line);  //skip first line
    getline(fin,line);
    while(!fin.eof())
    {
        getline(fin, x[0], ','); getline(fin, x[1], ','); getline(fin, x[2], ',');
        getline(fin, x[3], ','); getline(fin, x[4], ','); getline(fin, x[5], ',');
        getline(fin, x[6], ','); getline(fin, x[7], ','); getline(fin, x[8]);
//        stringstream linestream(line);
//        string value;
//        while(getline(linestream, value, ',')){
//            x[num] = value; num++; cout << x[num] << ", ";
//        }
        // loading attributes
        Flight_Ticket* temp = new Flight_Ticket(x[0], stoi(x[1]), x[2], x[3], x[4], x[5], x[6], x[7], x[8]);
        add(temp);
        counter++; // update the number of records
    }
    fin.close();
    return counter;
}
int FlightHASHTABLE::exportCSV(string path) //Export all the flight-tickets from the HashTable to a CSV file in an ascending order
{
    if(size == 0){
        cout << "There is no record!" << endl;
        return -1;
    }
    else {
        ofstream out1;
        out1.open(path);
        for(int i = 0; i < capacity; i++){
            if(!buckets[i].empty()){
                for(DoublyLinkedList<HashNode>::iterator it = buckets[i].begin(); it != buckets[i].end(); ++it){
                    HashNode temp1 = *it;
                    out1 << temp1.getValue().getCompanyName() << ", "
                         << temp1.getValue().getFlightNumber() << ", "
                         << temp1.getValue().getCountryOrigin() << ", "
                         << temp1.getValue().getCountryDestination() << ", "
                         << temp1.getValue().getStopOver() << ", "
                         << temp1.getValue().getPrice() << ", "
                         << temp1.getValue().getTimeDeparture() <<", "
                         << temp1.getValue().getTimeArrival() << ", "
                    << temp1.getValue().getDate() <<endl;
                }
            }
        }
        out1.close();
        cout << size << " records exported!" << endl;
        return 1;
    }
    // cout << "Exporting failed!" << endl;
    return -1;
}

int FlightHASHTABLE::count_collisions()//return the number of collisions on the HashTable
{
    return this->collisions;
}
int FlightHASHTABLE::add(Flight_Ticket* data) //add new flight to the HashTable
{
    // return 1 if added successfully; otherwise -1
    string newString, oldString = data->getCompanyName();
    for (int i = 0; i < oldString.length(); i++) {
        // convert to lowercase
        newString += tolower(oldString[i]);
    }
    string key = newString + to_string(data->getFlightNumber());
    // string key = data->getCompanyName() + to_string(data->getFlightNumber());
    long index = hashCode(key); //Find the bucket index where the new node will fall
    HashNode newElem(key, *data);
    if(!buckets[index].empty()){
        // check if the record has the exact same details
        for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
            HashNode temp1 = *it;
            if(temp1.getKey() == key &&
               temp1.getValue().getCompanyName() == data->getCompanyName() &&
               temp1.getValue().getFlightNumber() == data->getFlightNumber() &&
               temp1.getValue().getCountryOrigin() == data->getCountryOrigin() &&
               temp1.getValue().getCountryDestination() == data->getCountryDestination() &&
               temp1.getValue().getPrice() == data->getPrice() &&
               temp1.getValue().getTimeDeparture() == data->getTimeDeparture() &&
               temp1.getValue().getTimeArrival() == data->getTimeArrival() &&
               temp1.getValue().getDate() == data->getDate() &&
               temp1.getValue().getStopOver() == data->getStopOver()){
                cout << "Same record was added!" << endl;
                return -1;
            }
                
        }
        // calculate the number of collision
        for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
            HashNode temp1 = *it;
            if(temp1.getKey() != key) this->collisions++;
        }
        // sort when added // *** will need to fix the secondLast part
        if(newElem.getValue().getCountryDestination() >= buckets[index].secondLast().getElem().getValue().getCountryDestination()){
            buckets[index].addBack(HashNode(key, *data));
            this->size++;
            return 1;
        }
        for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
            HashNode temp1 = *it;
            if(newElem.getValue().getCountryDestination() <= temp1.getValue().getCountryDestination()){
                buckets[index].addBefore(it.getPtr(), HashNode(key, *data));
                this->size++;
                return 1;
            }
        }
    }
    else{
        buckets[index].addBack(HashNode(key,*data));    //otherwise add the new element into the bucket/list
        this->size++;                                   //Increate number of elements in the array
        return 1;
    }
    return -1;
}
void FlightHASHTABLE::removeRecord(string companyName, int flightNumber)//Delete a record with key from the hashtable
{
    string key = companyName + to_string(flightNumber);
    long index = hashCode(key);
    int counter = 0, num = 1;
    for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it != buckets[index].end(); ++it){
        HashNode temp = *it;
        if(temp.getKey() == key) counter++;
    }
    // check if the record exist or not
    if(counter == 0){
        cout << "No such records exists!" << endl;
        return;
    }
    // print the records that match the comppany name and flight number
    cout << counter << " records found: " << endl;
    for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
        HashNode temp = *it;
        if(temp.getKey() == key){
            cout << num << ". "
                 << temp.getValue().getCompanyName() << ", "
                 << temp.getValue().getFlightNumber() << ", "
                 << temp.getValue().getCountryOrigin() << ", "
                 << temp.getValue().getCountryDestination() << ", "
                 << temp.getValue().getStopOver() << ", "
                 << temp.getValue().getPrice() << ", "
                 << temp.getValue().getTimeDeparture() << ", "
                 << temp.getValue().getTimeArrival() << ", "
            << temp.getValue().getDate() << endl;
            num++;
        }
    }
    
    // user input which record to be deleted
    int option;
    string parameter_string;
    do{
        cout << endl << "Please select the record you want to delete: ";
        string temp;
        cin >> temp;
        stringstream input(temp);
        input >> option;
        if(option == 0 || option < 0 || option > counter ) cout << "Invalid command!" << endl;
    } while(option == 0 || option < 0 || option > counter );
    
    // delete the record based on the user input *** will need to implement the erase function
    counter = 1; // resets counter to 0 for deleting use
    for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
        if(counter == option){
            buckets[index].erase(it.getPtr());
            break;
        }
        else counter++;
    }
    cout << "The "<< counter << " record has been successfully deleted!" << endl;
}
void FlightHASHTABLE::find(string companyName, int flightNumber)//Find and Display records with same key entered 
{
    string key = companyName + to_string(flightNumber); // cout << "key for find: "<< key << endl;
    long index = hashCode(key);
    int counter = 0, num = 1; // cout how many results are found
    for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
        HashNode temp = *it;
        if(temp.getKey() == key) counter++;
    }
    if(counter == 0){
        cout << "Not found!" << endl;
        return;
    }
    cout << counter << " records found: " << endl;
    for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
        HashNode temp = *it;
        if(temp.getKey() == key){
            cout << num << ". " << temp.getValue().getCompanyName() << endl
                 << "   Flight Number: " << temp.getValue().getFlightNumber() << endl
                 << "   Country of Origin: " << temp.getValue().getCountryOrigin() << endl
                 << "   Country of Destination: " << temp.getValue().getCountryDestination() << endl
                 << "   Stopover: " << temp.getValue().getStopOver() << endl
                 << "   Price: " << temp.getValue().getPrice() << endl
                 << "   Time of Departure: " << temp.getValue().getTimeDeparture() << endl
                 << "   Time of Arrival: " << temp.getValue().getTimeArrival() << endl
            << "   Data: " << temp.getValue().getDate() << endl;
            num++;
        }
    }
}

void FlightHASHTABLE::allinday(string date)//Find and Display records with same day entered
{
    int counter = 0, num = 1; // cout how many results are found
    for(int i = 0; i < capacity; i++){
        for(DoublyLinkedList<HashNode>::iterator it = buckets[i].begin(); it!=buckets[i].end(); ++it){
            HashNode temp = *it;
            long tempNum = temp.getValue().getDate().length();
            string tempDate, Date = temp.getValue().getDate();
            for(int i = 0; i < tempNum-1; i++) tempDate += tolower(Date[i]);
            if(tempDate == date) counter++;
        }
    }
    if(counter == 0){
        cout << "No record found!" << endl;
        return;
    }
    cout << counter << " records found!" << endl;
    for(int i = 0; i < capacity; i++){
        if(!buckets[i].empty()){
            for(DoublyLinkedList<HashNode>::iterator it = buckets[i].begin(); it != buckets[i].end(); ++it){
                HashNode temp1 = *it;
                long tempNum = temp1.getValue().getDate().length();
                string tempDate, Date = temp1.getValue().getDate();
                // for(int i = 0; i < tempNum-1; i++) tempDate += Date[i];
                for(int i = 0; i < tempNum-1; i++) tempDate += tolower(Date[i]);
                // cout << tempDate << endl;
                if(tempDate == date){
                    cout << num << ". "
                         << temp1.getValue().getCompanyName() << ", "
                         << temp1.getValue().getFlightNumber() << ", "
                         << temp1.getValue().getCountryOrigin() << ", "
                         << temp1.getValue().getCountryDestination() << ", "
                         << temp1.getValue().getStopOver() << ", "
                         << temp1.getValue().getPrice() << ", "
                         << temp1.getValue().getTimeDeparture() <<", "
                         << temp1.getValue().getTimeArrival() << ", "
                    << temp1.getValue().getDate() <<endl;
                    num++;
                }
            }
        }
    }
}
void FlightHASHTABLE::printASC(string companyName, int flightNumber)//display the collisions in an ascending order
{
    string key = companyName + to_string(flightNumber);
    long index = hashCode(key);
    for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it){
        HashNode temp = *it;
        if(temp.getKey() == key){
            cout << temp.getValue().getCompanyName() << ", "
                 << temp.getValue().getFlightNumber() << ", "
                 << temp.getValue().getCountryOrigin() << ", "
                 << temp.getValue().getCountryDestination() << ", "
                 << temp.getValue().getStopOver() << ", "
                 << temp.getValue().getPrice() << ", "
                 << temp.getValue().getTimeDeparture() << ", "
                 << temp.getValue().getTimeArrival() << ", "
            << temp.getValue().getDate() << endl;
        }
    }
}


// *** different Hashcodes:
// the mad function: summation + division + multiplication
//    for(int i = 0; i < size; i++){
//        int tempKey = (key[i]*379 + 409) % 13; // summation + division + multiplication
//        sum += tempKey % capacity; // mod capacity if tempKey is huge
//    }
//    return sum %= capacity;

// summation
//    for(int i = 0; i < size; i++){
//        int tempKey = key[i]*379; // summation
//        sum += tempKey % capacity; // mod capacity if tempKey is huge
//    }
//    return sum %= capacity;

// quadratic
//    for(int i = 0; i < size; i++){
//        int tempKey = pow(key[i],4); // summation
//        sum += tempKey; // mod capacity if tempKey is huge
//    }
//    return sum %= capacity; // mod capacity if sum is huge
