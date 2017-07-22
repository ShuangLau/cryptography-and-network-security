#ifndef KEYCHAIN_H
#define KEYCHAIN_H

#include <string>
#include <map>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include "cryptopp562/default.h"
#include "cryptopp562/modes.h"
#include "cryptopp562/osrng.h"
#include "cryptopp562/secblock.h"
#include "cryptopp562/cryptlib.h"
#include "cryptopp562/filters.h"
#include "cryptopp562/hex.h"
#include "cryptopp562/aes.h"
#include "cryptopp562/modes.h"
#include "cryptopp562/sha.h" // or "md5.h" for MD5, etc
#include "cryptopp562/hmac.h"
#include "cryptopp562/pwdbased.h"

using namespace std;
using namespace CryptoPP;

#define MAX_KV_NUM (100)
#define MAX_KV_LENGTH (64)

struct keyvalue
{
	string domain_name_pt;
	string domain_hash_val;
	string password_ct;
};
 
class Keychain{
public:
	Keychain();
	bool insert(string domain_name, string password);
	bool remove(string domain_name);
	bool get(string domain_name, string& paswd);
	bool update(string domain_name, string password);
	bool if_user_exist(string username);
	void addUser(string username, string master_password);
	bool check_master_password(string username, string master_password);
	bool check_for_integrity();
private:
	string cur_user;
	string decry_mas_pas;
	string PBFDK2_Encryption(string master_password);
	string HMAC_Encryption(string pt, string pas);
	string AES_Encryption(string pt, string mas_paswd);
	string AES_Decryption(string ct, string mas_paswd);
	string string_to_hex(const string& input);
	string hex_to_string(const string& input);
	bool if_domain_exist(string domain_name);
};

#endif