
//=================================================================================
// Name         : DS Assignment#4 
// Author       : Wei Kuo
// Version      : 1.0
// Date Created : 05-05-2022
// Date Modified: 05-08-2022
// Description  : Starter Code for Contact Travel Management System using HASH TABLE in C++
//=================================================================================
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<string>
#include<time.h>

#include "flightticket.h"
#include "flighthashtable.h"

//==========================================================
void listCommands()
{
	cout<<"\n----------------------------------"<<endl;
	cout<<"1. import <path>      	:Import flight-tickets from a CSV file"<<endl
		<<"2. export <path>      	:Export flight-tickets to a CSV file"<<endl
		<<"3. count_collisions   	:Print Number of Collisions"<<endl
		<<"4. add                	:Add a new flight-ticket"<<endl
		<<"5. delete <key>       	:Delete a flight-ticket"<<endl
		<<"6. find <key>         	:Find a flight-ticket's details"<<endl
		<<"7. allinday <date>    	:Display all flight-tickets in a day"<<endl
		<<"8. printASC <key>     	:Print flight-tickets in ascending order"<<endl
		<<"exit               		:Exit the program"<<endl;
}
//==========================================================

bool check_number(string str) {
   for (int i = 0; i < str.length(); i++)
   if (isdigit(str[i]) == false)
      return false;
      return true;
}

//==========================================================
int main(void)
{
	FlightHASHTABLE myFlightHASHTABLE;
	string user_input;
	string command;
	string argument1, oldArgument1, argument2;

	listCommands();

    while(true)
	{
        try
		{
            // check whether the input flight number is a integer
            do{
                argument1 = ""; argument2 = ""; oldArgument1 = "";
                fflush(stdin); // clean up left over input
                cout<<">";
                getline(cin,user_input);
                // parse userinput into command and parameter(s)
                stringstream sstr(user_input);
                getline(sstr,command,' ');
                getline(sstr,oldArgument1,',' ); // the second part, which is the <key>
                getline(sstr,argument2);
                if(check_number(argument2) == false) cout << "Flight number must be an integer" << endl;
            } while(check_number(argument2) == false);
            
            // convert the argument1(company name) into lowercase
            for (int i = 0; i < oldArgument1.length(); i++) {
                // convert to lowercase
                argument1 += tolower(oldArgument1[i]);
            } // cout << "argument1: " << argument1 << endl; cout << "argument2: " << argument2 << endl;
            
            // main program starts from here
		    if(command =="import" || command == "1"){
                int oldSize = myFlightHASHTABLE.getSize();
                myFlightHASHTABLE.importCSV(argument1);
                cout << myFlightHASHTABLE.getSize() - oldSize << " records are imported!" << endl;
            }
			else if(command == "export" || command == "2") myFlightHASHTABLE.exportCSV(argument1);
            else if(command == "count_collisions" || command == "3"){
                int collisions = myFlightHASHTABLE.count_collisions();
                cout << "Total number of collision is " << collisions << endl;
            }
			else if(command == "add" || command == "4")
            {
                fflush(stdin); // clean up left over input
                string companyName, flightNum, countryOrigin, countryDestin, stopOver, price, timeDepart, timeArrive, date;
                do{
                    // input the details for each record
                    cout << "Please enter the details of the flight ticket: " << endl;
                    cout << "Company Name: ";           getline(cin, companyName);
                    cout << "Flight Number: ";          cin >> flightNum;
                    cout << "Country of Origin: ";      getline(cin, countryOrigin);
                    cout << "Country of Destination: "; getline(cin, countryDestin);
                    cout << "Stopover: ";               getline(cin, stopOver);
                    cout << "Price: ";                  getline(cin, price);
                    cout << "Time of Departure: ";      getline(cin, timeDepart);
                    cout << "Time of Arrival: ";        getline(cin, timeArrive);
                    cout << "Date: ";                   getline(cin, date);
                    
                    // check whether flight number is a integer
                    if(check_number(flightNum) == false) cout << "Input invalid! Flight number is an integer!" << endl << endl;
                }while(check_number(flightNum) == false);
                // instantialize the record
                stringstream input(flightNum);
                Flight_Ticket *newTicket = new Flight_Ticket(companyName, stoi(flightNum), countryOrigin, countryDestin, stopOver, price, timeDepart, timeArrive, date);
                myFlightHASHTABLE.add(newTicket);
                cout << "Sucessfully Added!" << endl;
                fflush(stdin); // clean up left over input
            }
			else if(command == "delete" || command == "5") myFlightHASHTABLE.removeRecord(argument1, stoi(argument2));
            else if(command == "find" || command == "6"){
                clock_t start, end;
                double cpu_time_used;
                // measuring the execution time of the find method
                start = clock();
                myFlightHASHTABLE.find(argument1, stoi(argument2));
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                cout << "Time taken: " << cpu_time_used << " seconds" << endl;
            }
			else if(command == "allinday" || command == "7") {
                myFlightHASHTABLE.allinday(argument1);
                // cout << argument1 << endl;
            }
			else if(command == "printASC" || command == "8") myFlightHASHTABLE.printASC(argument1, stoi(argument2));
			else if(command == "exit") break;
			else if(command == "help") listCommands();
			else cout <<"Invalid command !!!"<<endl;
			// fflush(stdin); // clean up left over input
		}
		catch (exception &e)
		{
			cout<<"Exception: "<<e.what()<<endl;
		}
	}
	return 0;
}
