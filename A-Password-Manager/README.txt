整个工程包括三个文件，分别是keychain.cpp, keychain.h和main.cpp。

keychain.cpp和keychain.h
keychain.h定义了对数据的具体操作，如数据的插入，插入，查询和修改等，以及一些必要的辅助函数，如将字符串与十六进制数之间的相互转换等。而keychain.cpp实现了对数据的具体操作。
  insert(string domain_name， string password )
    -首先检查用户所输入的Domain Name是否存在；
    -打开文件，用HMAC函数对Domain Name进行哈希，用AES对密码进行加密；
    -将Domain Name，Hash后Domain Name的值以及加密后的Password以空格分离的方式添加到文件中。

  remove(string domain_name)
    -首先检查用户所输入的Domain Name是否存在；
    -打开用户文件，并创建一个临时文件temp；
    -查找用户文件中的每一行，看是否为目标Domain Name，若不是则将此行输出至文件temp中。若找到目标Domain Name则不做操作;
    -将用户文件删除，将temp文件改名为用户文件的名称。
    因为C++中不允许直接对进行文件进行删除数据操作，只能用储存了删除过数据的文件来替换原文件达到删除数据的目的。

  get(string domain_name, string& paswd)
    -首先检查用户所输入的Domain Name是否存在；
    -打开用户文件，查找目标Domain Name，若找到Domain Name，用AES_Decryption函数将对应的密码进行解密，并将解密后的结果赋值给paswd。
  
  update(string domain_name, string password)
    -首先检查用户所输入的Domain Name是否存在；
    -查找用户文件中的每一行，看是否为目标Domain Name，若不是则将此行直接输出至文件temp中。若找到目标Domain Name则将传进来的新的Password进行AES加密并替换原有的加密过后     的password;
    -将用户文件删除，将temp文件改名为用户文件的名称。
    因为C++中不允许直接对进行文件进行修改数据操作，只能用储存了修改过数据的文件来替换原文件达到修改数据的目的。
  
  addUser(string username, string password)}
    -创建用户文件，名称为username；
    -将Master Password用PBDKF2进行加密；
    -将加密后的文件输入至文件中去。
  
  check_master_password(string username, string master_password)}
    -打开用户文件，读取第一行数据；
    -将传进来的password用PBDKF2进行加密并与原本储存在文件中的原密码加密后的结果进行对比，如果不完全相等则说明用户输入的密码错误，返回错误。否则就返回正确的值。
  
  check_for_integrity()
    -打开用户文件；
    -对文件中的每一行的Domain Name用HMAC函数进行哈希，和原本存储在文件中的Hashed Domain Name进行对比；
    -每一行的Domain Name对应的HMAC值都与存储的一样，则说明文件未经篡改。只要有一个不一样，则说明文件得到了篡改。

  if_domain_exist(string domain_name)
    -打开用户文件，搜寻每一行；
    -如果搜到存储的Domain Name与domain_name相同，说明存在，返回正确的值。否则返回错误的值。

main.cpp
此文件定义了整个工程的框架，如登陆界面和数据修改界面的进入和跳转等。
  Login_Signup()
    -登陆界面，要求用户输入Username和Master Password；
    -检查是否为新用户。若是新用户，则建立一个新的文件；
    -若不是新用户，则检查文件的完整性。

  data_oper(int choice)
    -若传进来的choice为1，进行数据插入操作。要求用户输入要插入的域名和密码，调用keychian类变量中的insert函数；
    -若传进来的choice为2，进行数据删除操作。要求用户输入要删除的域名，调用keychian类变量中的delete函数；
    -若传进来的choice为3，进行数据查询操作。要求用户输入要查询的域名，调用keychain类变量中的get函数；
    -若传进来的choice为4，进行数据修改操作。要求用户输入要域名和替换用的密码，调用keychain类变量中的update函数。
  
  data_manage()
    -要求用户输入想要进行的操作指令，1代表插入，2代表删除，3代表查询，4代表修改。并将值直接传入data_oper函数中。

  ask_for_login_again()
    -询问用户是否原因重新登录。如果用户输入1则代表想重新登陆，返回正确的值。否则返回错误的值。

与Cryptopp库相关的函数
在keychain类中有四个private类型的函数，分别是:PBFDK2_Encryption，HMAC_Encryption，AES_Encryption和AES_Decryption。这四个都用到了Cryptopp库中的相关变量类型和函数，来实现加密和解密的目的。

  PBFDK2_Encryption(string master_password)函数
  此函数用于将用户输入的Master Password转换为可用于HMAC和AES的Key。

  HMAC_Encryption(string pt, string pas)函数
  此函数用于将输入的pt以pas为Key进行哈希得到一个映射后的值。

  AES_Encryption(string pt, string mas_paswd)函数
  此函数用于将输入的pt以pas为Key进行哈希得到一个加密后的值。

  AES_Decryption(string ct, string mas_paswd)函数
  此函数用于将输入的ct以pas为Key进行哈希得到一个解密后的值。
