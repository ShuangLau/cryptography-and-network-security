/*
 *Author: Shuang Liu
 *03/09/2016
 *This program is to decrypt the stream cyphier using C++ language.
 *The key point is to use the characteristic of the space, which is that K XOR M = m 
 *and (A XOR K)XOR(B XOR K) = A XOR B
 */
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
using namespace std;

string CT[10];
string targetPT;
string targetCT;

void initInput()
{
	cout << "Intializing the given ciphertexts and the target texts..." << endl;
	CT[0]  =  
	"315c4eeaa8b5f8aaf9174145bf43e1784b8fa00dc71d885a804e5ee9fa40b16349c146fb778cdf2d3aff021dfff5b403b510d0d0455468aeb98622b137dae857553ccd8883a7bc37520e06e515d22c954eba5025b8cc57ee59418ce7dc6bc41556bdb36bbca3e8774301fbcaa3b83b220809560987815f65286764703de0f3d524400a19b159610b11ef3e";
	CT[1]  =  
	"234c02ecbbfbafa3ed18510abd11fa724fcda2018a1a8342cf064bbde548b12b07df44ba7191d9606ef4081ffde5ad46a5069d9f7f543bedb9c861bf29c7e205132eda9382b0bc2c5c4b45f919cf3a9f1cb74151f6d551f4480c82b2cb24cc5b028aa76eb7b4ab24171ab3cdadb8356f";
	CT[2]  =  
	"32510ba9a7b2bba9b8005d43a304b5714cc0bb0c8a34884dd91304b8ad40b62b07df44ba6e9d8a2368e51d04e0e7b207b70b9b8261112bacb6c866a232dfe257527dc29398f5f3251a0d47e503c66e935de81230b59b7afb5f41afa8d661cb";
	CT[3]  =  
	"32510ba9aab2a8a4fd06414fb517b5605cc0aa0dc91a8908c2064ba8ad5ea06a029056f47a8ad3306ef5021eafe1ac01a81197847a5c68a1b78769a37bc8f4575432c198ccb4ef63590256e305cd3a9544ee4160ead45aef520489e7da7d835402bca670bda8eb775200b8dabbba246b130f040d8ec6447e2c767f3d30ed81ea2e4c1404e1315a1010e7229be6636aaa";
	CT[4]  =  
	"3f561ba9adb4b6ebec54424ba317b564418fac0dd35f8c08d31a1fe9e24fe56808c213f17c81d9607cee021dafe1e001b21ade877a5e68bea88d61b93ac5ee0d562e8e9582f5ef375f0a4ae20ed86e935de81230b59b73fb4302cd95d770c65b40aaa065f2a5e33a5a0bb5dcaba43722130f042f8ec85b7c2070";
	CT[5]  =  
	"32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd2061bbde24eb76a19d84aba34d8de287be84d07e7e9a30ee714979c7e1123a8bd9822a33ecaf512472e8e8f8db3f9635c1949e640c621854eba0d79eccf52ff111284b4cc61d11902aebc66f2b2e436434eacc0aba938220b084800c2ca4e693522643573b2c4ce35050b0cf774201f0fe52ac9f26d71b6cf61a711cc229f77ace7aa88a2f19983122b11be87a59c355d25f8e4";
	CT[6]  =  
	"32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd90f1fa6ea5ba47b01c909ba7696cf606ef40c04afe1ac0aa8148dd066592ded9f8774b529c7ea125d298e8883f5e9305f4b44f915cb2bd05af51373fd9b4af511039fa2d96f83414aaaf261bda2e97b170fb5cce2a53e675c154c0d9681596934777e2275b381ce2e40582afe67650b13e72287ff2270abcf73bb028932836fbdecfecee0a3b894473c1bbeb6b4913a536ce4f9b13f1efff71ea313c8661dd9a4ce";
	CT[7]  =  
	"315c4eeaa8b5f8bffd11155ea506b56041c6a00c8a08854dd21a4bbde54ce56801d943ba708b8a3574f40c00fff9e00fa1439fd0654327a3bfc860b92f89ee04132ecb9298f5fd2d5e4b45e40ecc3b9d59e9417df7c95bba410e9aa2ca24c5474da2f276baa3ac325918b2daada43d6712150441c2e04f6565517f317da9d3";
	CT[8]  =  
	"271946f9bbb2aeadec111841a81abc300ecaa01bd8069d5cc91005e9fe4aad6e04d513e96d99de2569bc5e50eeeca709b50a8a987f4264edb6896fb537d0a716132ddc938fb0f836480e06ed0fcd6e9759f40462f9cf57f4564186a2c1778f1543efa270bda5e933421cbe88a4a52222190f471e9bd15f652b653b7071aec59a2705081ffe72651d08f822c9ed6d76e48b63ab15d0208573a7eef027";
	CT[9] =  
	"466d06ece998b7a2fb1d464fed2ced7641ddaa3cc31c9941cf110abbf409ed39598005b3399ccfafb61d0315fca0a314be138a9f32503bedac8067f03adbf3575c3b8edc9ba7f537530541ab0f9f3cd04ff50d66f1d559ba520e89a2cb2a83";
	targetCT = 
	"255603efa6a9acecf107155ea506b57540caa3018a108b08c11c03a0e85fa0660cde47b4";
	targetPT = 
	"************************************";
	cout << "Initialization done." << endl;
}
//the length of the result is the same as a's
string xoOper(string a, string b)
{
	string result;
	char a_lett[2];char b_lett[2];
	long a_value;  long b_value;
	long xor_value;
	int targetLen = targetCT.length();
	//get the minimum length among a, b, and targetCT
	int cnt1 = min(a.length(), b.length());
	int cnt = min(targetLen, cnt1);
	ostringstream oss;
	for(int i = 0;i < cnt;i=i+2){
		a_lett[0] = a[i];a_lett[1] = a[i + 1];
		b_lett[0] = b[i];b_lett[1] = b[i + 1];
		//Turn string into a decimal number
		a_value = strtol(a_lett, NULL, 16);
		b_value = strtol(b_lett, NULL, 16);
		xor_value = a_value ^ b_value;
		//Consider the case of 0x0_
		if(xor_value < 16){
			oss << 0 << std::hex << xor_value;
		}
		else{
			oss << std::hex << xor_value;
		}
	}
	result = oss.str();
	return result;
}

void decrypt()
{
	cout << "Now preparing to decrypt..." << endl;
	string xoResult[9];//store the XOR result between one of the CT and all other CTs 
	string targetXOResult[10];//store the XOR result between the targetCT and CTs
	
	int strLen = 0;
	for(int i = 0;i < 10;i++){
		int resultCnt = 0;
		strLen = CT[i].length();
		string targetXOTempResult;
		for(int j = 0;j < 10;j++){
			if(i != j){
				//get the XOR result
				xoResult[resultCnt] = xoOper(CT[i], CT[j]);
				resultCnt++;
			}
		}
		
		//store the XOR result between i-th CT and the targetCT
		targetXOTempResult = xoOper(CT[i], targetCT);
		//store the XOR result between the targetCT and i-th CT
		targetXOResult[i] = xoOper(targetCT, CT[i]);
		//get the minimum length among the XO results.
		int loop1 = min(min(xoResult[0].length(), xoResult[1].length()), xoResult[2].length());
		int loop2 = min(min(xoResult[3].length(), xoResult[4].length()), xoResult[5].length());
		int loop3 = min(min(xoResult[6].length(), xoResult[7].length()), xoResult[8].length());
		int loop =  min(min(loop1, loop2), loop3);
		for(int k = 0;k < loop;k=k+2){
			int letCnt = 0;
			for(int ind = 0;ind < 9;ind++){
				char let[2];
				let[0] = xoResult[ind][k];
				let[1] = xoResult[ind][k+1];
				long lttr = strtol(let, NULL, 16);
				//judge if the value is a letter
				if((lttr <= 122 && lttr >= 97) || (lttr <= 90 && lttr >= 65)){
					letCnt++;
				}
			}
			if(letCnt > 6){//k-th value of CT[i] is a space
				char letter[2];
				letter[0] = targetXOTempResult[k];
				letter[1] = targetXOTempResult[k + 1];
				int lttr = strtol(letter, NULL, 16);
				if((lttr <= 122 && lttr >= 97) || (lttr <= 90 && lttr >= 65)){
					//interchange bewtween the uppercase number and the lowercase number
					targetPT[k/2] = (const char)lttr ^ 32;
				}
			}
			
		}
	}
	cout << "Decryption done." << endl;
	cout << "Now preparing to decipher the tagretCT..." << endl;
	//get the minimum length among the targetXOResult
	int targetLen1 = min(min(targetXOResult[0].length(), targetXOResult[1].length()), targetXOResult[2].length());
	int targetLen2 = min(min(targetXOResult[3].length(), targetXOResult[4].length()), targetXOResult[5].length());
	int targetLen3 = min(targetXOResult[6].length(), targetXOResult[7].length());
	int targetLen4 = min(targetXOResult[8].length(), targetXOResult[9].length());
	int targetLen = min(min(min(targetLen1, targetLen2), targetLen3), targetLen4);
	for(int i = 0;i < targetLen;i=i+2){
		int letCnt = 0;
		for(int ind = 0;ind < 10;ind++){
			char let[2];
			let[0] = targetXOResult[ind][i];
			let[1] = targetXOResult[ind][i+1];
			long lttr = strtol(let, NULL, 16);
			//judge if the value is a letter
			if((lttr <= 122 && lttr >= 97) || (lttr <= 90 && lttr >= 65)){
				letCnt++;
			}
		}
		if(letCnt > 6){//i-th value of targetPT is a space
			if(targetPT[i/2] == '*'){
				targetPT[i/2] = ' ';
			}
			else{
				//There is some prediction wrong
				targetPT[i/2] = '?';
			}
		}
	}
	cout << "Deciphering the targetCT done." << endl;
}

void printModificationHint()
{
	while(1){
		cout << "Do you want to modify the target plain text manually?(Y/N)" << endl;
		char ans;
		cin >> ans;
		if(ans == 'Y' || ans == 'y'){
			cout << 
			"Please input the position you want to change and the value you want to replace with(start from 0)"
			     << endl;
			while(1){
				int pos;
				char charValue;
				cin >> pos;
				cin >> charValue;
				if(pos >= targetPT.length() || pos < 0){
					cout << "The position is invalid, please input again." << endl;
					continue;
				}
				else{
					targetPT[pos] = charValue;
					break; 
				}
			}
			
		}
		else{
			cout << "The process is done." << endl;
			break;
		}
		cout << endl << "The target plain text is as followed:" << endl;
		cout << targetPT << endl;
	}
	
}

int main()
{
	initInput();
	decrypt();
	cout << endl << "The target plain text is as followed:" << endl;
	cout << targetPT << endl;
	printModificationHint();

	return 0;
}

