#include <iostream>
#include <string>
#include <fstream>
#include "keychain.h"

using namespace std;

Keychain KC;
string curren_user;

//Ask for the username and the master password
bool Login_Signup(){
	string username;
	string master_password;
	cout << "Please enter the username and the master password:" << endl;
	cout << "Username:" << endl;
	cin >> username;
	cout << "Master Password:" << endl;
	cin >> master_password;
	curren_user = username;
	if(!KC.if_user_exist(username)){
		KC.addUser(username, master_password);
	}
	else{
		if(!KC.check_master_password(username, master_password)){
			cout << "The master_password is not correct." << endl;
			return false;
		} 
		else{
			if(!KC.check_for_integrity()){
				cout << "Warning: The data has been modified!" << endl;
				return false;
			}
			return true;
		}
	}

	return true;
}
//Operate the file according to the request
void data_oper(int choice){
	if(choice == 1){//do the insertion
		string domain_name;
		string password;
		cout << "Please enter the domain name and the password(Enter Key for finishing)." << endl;
		cout << "Domain Name:" << endl;
		cin >> domain_name;
		cout << "Password:" << endl;
		cin >> password;
		if(KC.insert(domain_name, password)){
			cout << "Data Insertion Success." << endl;
		}
		else{
			cout << "Data Insertion Failure." << endl;
		}
	}
	else if(choice == 2){//do the deletion
		string domain_name;
		cout << "Please enter the domain name you intend to remove." << endl;
		cout << "Domain Name:" << endl;
		cin >> domain_name;
		if(KC.remove(domain_name)){
			remove(curren_user.c_str());
			rename("temp", curren_user.c_str());
			cout << "Data Deletion Success." << endl;
		}
		else{
			cout << "Error: Domain Name don't exist!" << endl;
			cout << "Data Deletion Failure." << endl;
		}
	}
	else if(choice == 3){//do the acquition
		string domain_name;
		cout << "Please enter the domain name you look for." << endl;
		cout << "Domain Name:" << endl;
		cin >> domain_name;
		string spec_password;
		if(KC.get(domain_name, spec_password)){
			cout << "Data Acquition Success." << endl;
			cout << "The corresponding password is:" << spec_password << endl;
		}
		else{
			cout << "Data Acquition Failure." << endl;
		}
	}
	else{//do the replacement
		string domain_name;
		string password;
		cout << "Please enter the domain name and the new password you want to replace with." << endl;
		cout << "Domain Name:" << endl;
		cin >> domain_name;
		cout << "Password:" << endl;
		cin >> password;
		if(KC.update(domain_name, password)){
			remove(curren_user.c_str());
			rename("temp", curren_user.c_str());
			cout << "Data Modification Success." << endl;
		}
		else{
			cout << "Error: Domain Name don't exist!" << endl;
			cout << "Data Modification Failure." << endl;
		}
	}
}
//manage the data
void data_manage(){
	while(1){
		cout << "Do you want to alter the file? 1 for Yes, and 0 for No." << endl;
		bool contin;
		cin >> contin;
		if(!contin)  break;
		else
		{
			cout << "Please specify the kind of operation:" << endl;
			cout << "1 for Data Insertion."    << endl
			     << "2 for Data Deletion."     << endl
			     << "3 for Data Acquition."    << endl
			     << "4 for Data Modification." << endl;
			int oper_choice;
			cin >> oper_choice;
			data_oper(oper_choice);
		}
	}
	cout << "All operations done." << endl;
}

bool ask_for_login_again(){
	cout << "Do you want to login or sign up again?1 for Yes, and 0 for No." << endl;
	int ans;
	cin >> ans;
	return ans;
}

int main(int argc, char const* argv[]){
	while(1){
		if(Login_Signup()){
			data_manage();
		}
		if(!ask_for_login_again())
			break;
	}
	cout << "Password Managment Service Done." << endl;
	return 0;
}

//g++ -o Lab4.exe Lab4.cpp keychain.cpp cryptopp562/libcryptopp.a