�������̰��������ļ����ֱ���keychain.cpp, keychain.h��main.cpp��

keychain.cpp��keychain.h
keychain.h�����˶����ݵľ�������������ݵĲ��룬���룬��ѯ���޸ĵȣ��Լ�һЩ��Ҫ�ĸ����������罫�ַ�����ʮ��������֮����໥ת���ȡ���keychain.cppʵ���˶����ݵľ��������
  insert(string domain_name�� string password )
    -���ȼ���û��������Domain Name�Ƿ���ڣ�
    -���ļ�����HMAC������Domain Name���й�ϣ����AES��������м��ܣ�
    -��Domain Name��Hash��Domain Name��ֵ�Լ����ܺ��Password�Կո����ķ�ʽ��ӵ��ļ��С�

  remove(string domain_name)
    -���ȼ���û��������Domain Name�Ƿ���ڣ�
    -���û��ļ���������һ����ʱ�ļ�temp��
    -�����û��ļ��е�ÿһ�У����Ƿ�ΪĿ��Domain Name���������򽫴���������ļ�temp�С����ҵ�Ŀ��Domain Name��������;
    -���û��ļ�ɾ������temp�ļ�����Ϊ�û��ļ������ơ�
    ��ΪC++�в�����ֱ�ӶԽ����ļ�����ɾ�����ݲ�����ֻ���ô�����ɾ�������ݵ��ļ����滻ԭ�ļ��ﵽɾ�����ݵ�Ŀ�ġ�

  get(string domain_name, string& paswd)
    -���ȼ���û��������Domain Name�Ƿ���ڣ�
    -���û��ļ�������Ŀ��Domain Name�����ҵ�Domain Name����AES_Decryption��������Ӧ��������н��ܣ��������ܺ�Ľ����ֵ��paswd��
  
  update(string domain_name, string password)
    -���ȼ���û��������Domain Name�Ƿ���ڣ�
    -�����û��ļ��е�ÿһ�У����Ƿ�ΪĿ��Domain Name���������򽫴���ֱ��������ļ�temp�С����ҵ�Ŀ��Domain Name�򽫴��������µ�Password����AES���ܲ��滻ԭ�еļ��ܹ���     ��password;
    -���û��ļ�ɾ������temp�ļ�����Ϊ�û��ļ������ơ�
    ��ΪC++�в�����ֱ�ӶԽ����ļ������޸����ݲ�����ֻ���ô������޸Ĺ����ݵ��ļ����滻ԭ�ļ��ﵽ�޸����ݵ�Ŀ�ġ�
  
  addUser(string username, string password)}
    -�����û��ļ�������Ϊusername��
    -��Master Password��PBDKF2���м��ܣ�
    -�����ܺ���ļ��������ļ���ȥ��
  
  check_master_password(string username, string master_password)}
    -���û��ļ�����ȡ��һ�����ݣ�
    -����������password��PBDKF2���м��ܲ���ԭ���������ļ��е�ԭ������ܺ�Ľ�����жԱȣ��������ȫ�����˵���û������������󣬷��ش��󡣷���ͷ�����ȷ��ֵ��
  
  check_for_integrity()
    -���û��ļ���
    -���ļ��е�ÿһ�е�Domain Name��HMAC�������й�ϣ����ԭ���洢���ļ��е�Hashed Domain Name���жԱȣ�
    -ÿһ�е�Domain Name��Ӧ��HMACֵ����洢��һ������˵���ļ�δ���۸ġ�ֻҪ��һ����һ������˵���ļ��õ��˴۸ġ�

  if_domain_exist(string domain_name)
    -���û��ļ�����Ѱÿһ�У�
    -����ѵ��洢��Domain Name��domain_name��ͬ��˵�����ڣ�������ȷ��ֵ�����򷵻ش����ֵ��

main.cpp
���ļ��������������̵Ŀ�ܣ����½����������޸Ľ���Ľ������ת�ȡ�
  Login_Signup()
    -��½���棬Ҫ���û�����Username��Master Password��
    -����Ƿ�Ϊ���û����������û�������һ���µ��ļ���
    -���������û��������ļ��������ԡ�

  data_oper(int choice)
    -����������choiceΪ1���������ݲ��������Ҫ���û�����Ҫ��������������룬����keychian������е�insert������
    -����������choiceΪ2����������ɾ��������Ҫ���û�����Ҫɾ��������������keychian������е�delete������
    -����������choiceΪ3���������ݲ�ѯ������Ҫ���û�����Ҫ��ѯ������������keychain������е�get������
    -����������choiceΪ4�����������޸Ĳ�����Ҫ���û�����Ҫ�������滻�õ����룬����keychain������е�update������
  
  data_manage()
    -Ҫ���û�������Ҫ���еĲ���ָ�1������룬2����ɾ����3�����ѯ��4�����޸ġ�����ֱֵ�Ӵ���data_oper�����С�

  ask_for_login_again()
    -ѯ���û��Ƿ�ԭ�����µ�¼������û�����1����������µ�½��������ȷ��ֵ�����򷵻ش����ֵ��

��Cryptopp����صĺ���
��keychain�������ĸ�private���͵ĺ������ֱ���:PBFDK2_Encryption��HMAC_Encryption��AES_Encryption��AES_Decryption�����ĸ����õ���Cryptopp���е���ر������ͺͺ�������ʵ�ּ��ܺͽ��ܵ�Ŀ�ġ�

  PBFDK2_Encryption(string master_password)����
  �˺������ڽ��û������Master Passwordת��Ϊ������HMAC��AES��Key��

  HMAC_Encryption(string pt, string pas)����
  �˺������ڽ������pt��pasΪKey���й�ϣ�õ�һ��ӳ����ֵ��

  AES_Encryption(string pt, string mas_paswd)����
  �˺������ڽ������pt��pasΪKey���й�ϣ�õ�һ�����ܺ��ֵ��

  AES_Decryption(string ct, string mas_paswd)����
  �˺������ڽ������ct��pasΪKey���й�ϣ�õ�һ�����ܺ��ֵ��
