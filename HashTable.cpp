#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
// #include<list>
#include "Iterator.h"

using namespace std;
class HashNode
{
	private:
		string key;
		string value;
	public:
		HashNode(string key, string value)
		{
			this->key = key;
			this->value = value;
		}
		friend class HashTable;
		string getKey(){
			return this->key;
		}
		string getValue(){
			return this->value;
		}

};

class HashTable
{
	private:
		DoublyLinkedList<HashNode> *buckets;		// Array of lists of type Hash Nodes
		int size;					    //Current Size of HashTable
		int capacity;				    // Total Capacity of HashTable
		int probes;					// Total Number of probes
	public:
		HashTable(int capacity);
		unsigned long hashCode(string key);
		void insert(string key, string value);
		string search(string key);
		void erase(string key); //uncomment it for Task3
		int getSize();
		int getprobes();
		~HashTable();

};
HashTable::HashTable(int capacity)
{
	buckets = new DoublyLinkedList<HashNode>[capacity];	  //Array of a list/chain
	this->capacity = capacity;
	this->size = 0;
	this->probes = 0;
}

unsigned long HashTable::hashCode(string key)
{
	int m = key.length();
	unsigned int h = 0;
	for (int i = 0; i < m; i++) 
	{
		h = (h << 5) | (h >> 27);
		h += (unsigned int) key[i]; 
	}
	return h % this->capacity;// make sure that the hashcode is in the range 0 - N-1;
}
//================================================
void HashTable::insert(string key, string value)
{
	int index = hashCode(key); //Find the bucket index where the new node will fall, make sure that the index is within the bounds of array
	
	for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin();
        HashNode temp = *it;
        it!=buckets[index].end(); ++it,probes++)
		if(it.key==key)				//if key already present in the hash table
		{
			it.value=value;				//then update the value and return
			return;
		}

	buckets[index].addBack(HashNode(key,value));	//otherwise add the new element into the bucket/list
	this->size++;									//Increate number of elements in the array
}
//===================================================
string HashTable::search(string key)
{
	int index = hashCode(key);			//Get the index of the bucket
	int counter = 0;
	for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it) 
	{
		HashNode temp = *it;
		if(temp.getKey()==key)
			return temp.getValue()+" (comparisions = "+to_string(++counter)+")";
		
		counter++;
	}

	return "Record Not Found...!";			//conclude that data not found in the array
}
//=====================================================
void HashTable::erase(string key)
{
	// int index = hashCode(key);	//Get the index of the bucket

	// for(DoublyLinkedList<HashNode>::iterator it = buckets[index].begin(); it!=buckets[index].end(); ++it) 
	// {
	// 	if(it.key==key)
	// 	{
	// 		buckets[index].erase(it.ptr);
	// 		cout<<"Deleted :"<<key<<", "<<it.value<<endl;
	// 		return;
	// 	}
	// }

	// cout<<"Cant delete....Record Not Found...!"<<endl;			//conclude that data not found in the array

}
//===================================================
int HashTable::getSize()
{
	return this->size;
}
//===================================================
int HashTable::getprobes()
{
	return this->probes;
}
//===================================================
HashTable::~HashTable()
{
	while(!buckets->empty()) buckets->removeBack();
}
//======================================================
int main(void)
{
	ifstream fin;
	fin.open("zipcodes.csv");
	if(!fin){
		cout<<"Can not open the file <zipcodes.txt>...!"<<endl;
		exit(-1);
	}

	string line;
	HashTable myHashTable(24851);// Prime number bigger than >24850 (19880*1.25);
	getline(fin,line);  //skip first line
	while(!fin.eof())
	{
		string key, value;
		getline(fin,key,',');
		getline(fin,value);
		cout<<key<<":"<<value<<endl;
		myHashTable.insert(key,value);

	}
	fin.close();
	cout<<"==================================================="<<endl;
	cout<<"Hash Table size = "<<myHashTable.getSize()<<endl;
	cout<<"Total Number of probes = "<<myHashTable.getprobes()<<endl;
	cout<<"Avg. Number of probes Per Entry = "<<float(myHashTable.getprobes())/myHashTable.getSize()<<endl;
	cout<<"==================================================="<<endl;

	string user_input, command, argument1, argument2;

	while(true)
	{
	
		cout<<">";
		getline(cin,user_input);

		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,argument1,',');
		getline(sstr,argument2);

			if(command == "search") cout<<"Zip code for "<<argument1<<" is: "<<myHashTable.search(argument1)<<endl;
		else if(command == "insert") {myHashTable.insert(argument1,argument2);}
		//Uncomment following line for the Task 3
		else if(command == "erase")	  myHashTable.erase(argument1);
		else if(command == "help")	  cout<<"Commands available \ninsert <key,value>\nsearch <key>\nerase <key>"<<endl;
		else if(command == "exit")	  break;
		else cout<<"Invalid command !!!"<<endl;
	}
	return 0;
}
