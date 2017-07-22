#include "keychain.h"

Keychain::Keychain(){}
//Insert the data
bool Keychain::insert(string domain_name, string password){
	if(if_domain_exist(domain_name)){
		cout << "Error: Domain Name Duplicated!" << endl;
		return false;
	}
	else{
		ofstream f;
		f.open(cur_user.c_str(),ofstream::app); 
		if(f.is_open()){
			string domName, hashDomName, decry_password;
			domName = domain_name;
			hashDomName = HMAC_Encryption(domain_name, decry_mas_pas);//map the domain name
			decry_password = AES_Encryption(password, decry_mas_pas); //encrypt the password
			f << domName << ' ' << hashDomName << ' ' << decry_password << endl;
			f.close();
			return true;
		}
	}
}
//Delete the data
bool Keychain::remove(string domain_name){
	if(if_domain_exist(domain_name)){
		ifstream f(cur_user.c_str());
		ofstream fn("temp");
		std::string line;
		while (std::getline(f, line)){
			std::istringstream iss(line);
		    string domName, hashName, encryPas;
		    if (iss >> domName >> hashName >> encryPas) {
		    	if(domName == domain_name){
		    		//do nothing to delete the data
		    	}
		    	else{
		    		fn << domName << ' ' << hashName << ' ' << encryPas << endl;
		    	}
		    }
		}
		f.close();
    	fn.close();
    	//replace the old file with a new one without target domain name
    	remove(cur_user.c_str()); 
    	rename("temp", cur_user.c_str());
    	return true;
	}
	else{
		return false;
	}
}
//Obtain the data
bool Keychain::get(string domain_name, string& paswd){
	if(!if_domain_exist(domain_name)){
		cout << "Error: Domain Name don't exist!" << endl;
		return false;
	}
	else{
		fstream f(cur_user.c_str(), ofstream::in);
		std::string line;
		while (std::getline(f, line)){
			std::istringstream iss(line);
		    string domName, hashName, encryPas;
		    if (iss >> domName >> hashName >> encryPas) {
		    	if(domName == domain_name){
		    		paswd = AES_Decryption(encryPas, decry_mas_pas);//decrypt the encrypted password
		    		return true;
		    	}
		    }
		}
	}
}
//Modify the data
bool Keychain::update(string domain_name, string password){
	if(!if_domain_exist(domain_name)){
		return false;
	}
	else{
		fstream f(cur_user.c_str(), ofstream::in);
		ofstream fn("temp");
		std::string line;
		while (std::getline(f, line)){
			std::istringstream iss(line);
		    string domName, hashName, encryPas;
		    if (iss >> domName >> hashName >> encryPas) {
		    	if(domName == domain_name){
		    		fn << domName << ' ' << hashName << ' ' << AES_Encryption(password, decry_mas_pas) << endl;
		    	}
		    	else{
		    		fn << domName << ' ' << hashName << ' ' << encryPas << endl;
		    	}
		    }
		}
		f.close();
    	fn.close();
    	
    	return true;
	}
}
//Check whether the user exists or not
bool Keychain::if_user_exist(string username){
  	std::ifstream infile(username.c_str());
    return infile.good();
}
//Add the new user
void Keychain::addUser(string username, string master_password){
	cur_user = username;
	fstream f(username.c_str(), ofstream::out);
	if(f.is_open()){
		f << "Master Password: ";
		string result = PBFDK2_Encryption(master_password);//get the master password encrypted
		decry_mas_pas = result;
		f << result << endl;
		f.close();
	}
}
//Check whether the master password is correct
bool Keychain::check_master_password(string username, string master_password)
{
	cur_user = username;
	fstream f(username.c_str(), ofstream::in);
	std::string line;
	string notation1, notation2, mas_paswrd;
	if(f.is_open()){
		std::getline(f, line);
		std::istringstream iss(line);
		iss >> notation1 >> notation2 >> mas_paswrd;
		f.close();
	}

	string decy_pas = PBFDK2_Encryption(master_password);
	if(decy_pas == mas_paswrd){//check whether the master password is correct or not
		decry_mas_pas = decy_pas;
		return true;
	}
	else
		return false;
}
//Check whether the domain exists or not
bool Keychain::if_domain_exist(string domain_name){
	fstream f(cur_user.c_str(), ofstream::in);
	std::string line;

	if(f.is_open()){
		while (std::getline(f, line))
		{
		    std::istringstream iss(line);
		    string domName, hashName, encryPas;
		    if (iss >> domName >> hashName >> encryPas) {
		     	if(domain_name != domName){
		     		//do nothing
		     	}
		     	else{
		     		f.close();
		     		return true;
		     	}
		    }
		}
		f.close();
	}
	return false;
}
//Check whether the data is modified or not
bool Keychain::check_for_integrity(){
	fstream f(cur_user.c_str(), ofstream::in);
	std::string line;
	string key;
	int lineCnt = 0;
	if(f.is_open()){
		while (std::getline(f, line))
		{
			lineCnt++;
		    std::istringstream iss(line);
		    if(lineCnt == 1)  continue;
		    else{
		    	string domName, hashName, encryPas;
			    if (iss >> domName >> hashName >> encryPas) {
			    	//check whether the domain name is modified or not
			    	if(HMAC_Encryption(domName, decry_mas_pas) != hashName){
			     		f.close();
			     		return false;
			     	}
			    }
		    }
		}
		f.close();
	}

	return true;
}

string Keychain::PBFDK2_Encryption(string master_password)
{
	int iterations = 100;
	string salt_val = "1234567812345678";
	const byte* s = (const byte *) salt_val.data();
	size_t len = salt_val.size();
	SecByteBlock pwsalt(s, len);

	SecByteBlock derivedkey(AES::DEFAULT_KEYLENGTH);

	PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
	pbkdf.DeriveKey(
		// buffer that holds the derived key
		derivedkey, derivedkey.size(),
		// purpose byte. unused by this PBKDF implementation.
		0x00,
		// password bytes. careful to be consistent with encoding...
		(byte *) master_password.data(), master_password.size(),
		// salt bytes
		pwsalt, pwsalt.size(),
		// iteration count. See SP 800-132 for details. You want this as large as you can tolerate.
		// make sure to use the same iteration count on both sides...
		iterations
		);

	string encoded;
	encoded.clear();
	StringSource(derivedkey, derivedkey.size(), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	);

	return encoded;
}

string Keychain::HMAC_Encryption(string pt, string pas){
	const byte* k = (const byte *) pas.data();
	size_t len = pas.size();
	SecByteBlock key(k, len);
	string mac, encoded;

	// Pretty print key
	encoded.clear();
	StringSource(key, key.size(), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource

	try
	{
		HMAC< SHA256 > hmac(key, key.size());		

		StringSource(pt, true, 
			new HashFilter(hmac,
				new StringSink(mac)
			) // HashFilter      
		); // StringSource
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	// Pretty print MAC
	encoded.clear();
	StringSource(mac, true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource

	return encoded;
}

string Keychain::AES_Encryption(string pt, string mas_paswd){
	const byte* key = (const byte *) mas_paswd.data();
	SecByteBlock derivedkey(key, mas_paswd.size());

	// Cipher Text Sink
	std::string CipherText;

	// Encryptor
	CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption 
	//Encryptor( key, sizeof(key), iv );
	Encryptor( derivedkey, derivedkey.size());

	// Encryption
	CryptoPP::StringSource( pt, true,
	  new CryptoPP::StreamTransformationFilter( Encryptor,
	    new CryptoPP::StringSink( CipherText )
	  ) // StreamTransformationFilter
	); // StringSource

	return string_to_hex(CipherText);
}

string Keychain::AES_Decryption(string ct, string mas_paswd){
	string cipher = hex_to_string(ct);
	const byte* key = (const byte *) mas_paswd.data();
	SecByteBlock derivedkey(key, mas_paswd.size());

	// Recovered Text Sink
	std::string RecoveredText;

	// Decryptor
	CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption
	// Decryptor( key, sizeof(key), iv );
	Decryptor( derivedkey, derivedkey.size() );

	// Decryption
	CryptoPP::StringSource(cipher, true,
	  new CryptoPP::StreamTransformationFilter( Decryptor,
	    new CryptoPP::StringSink( RecoveredText )
	  ) // StreamTransformationFilter
	); // StringSource
	return RecoveredText;
}

string Keychain::string_to_hex(const string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

string Keychain::hex_to_string(const string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}