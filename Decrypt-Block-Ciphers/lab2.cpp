/*
 *This program is to decrypt the cipher text by CBC and CTR using C++ language
 *Author:Shuang Liu
 *2016/03/15
 *Copyright (c) 2016 Shuang Liu. All rights reserved.
 */
#include <iostream>
#include <cstdio>
#include <sstream>
#include "AES.h"
using namespace std;

//Initialize the keys and ciphertexts of CBC and CTR and the the length of them
Byte CBCKey[16] = {0x14, 0x0b, 0x41, 0xb2, 0x2a, 0x29, 0xbe, 0xb4, 
	               0x06, 0x1b, 0xda, 0x66, 0xb6, 0x74, 0x7e, 0x14};
Byte CTRKey[16] = {0x36, 0xf1, 0x83, 0x57, 0xbe, 0x4d, 0xbd, 0x77,
				   0xf0, 0x50, 0x51, 0x5c, 0x73, 0xfc, 0xf9, 0xf2}; 
int CBCCT_Length = 48;						
Byte CBCCT[100] = {0x4c, 0xa0, 0x0f, 0xf4, 0xc8, 0x98, 0xd6, 0x1e,
                   0x1e, 0xdb, 0xf1, 0x80, 0x06, 0x18, 0xfb, 0x28,
                   0x72, 0x79, 0x6b, 0x82, 0x4a, 0xda, 0xd2, 0x46,
                   0x2c, 0x18, 0x69, 0x0b, 0x46, 0x43, 0x66, 0x79,
                   0x2a, 0x38, 0xa6, 0x71, 0x85, 0xc5, 0xb0, 0xd2,
                   0xf3, 0x88, 0xc7, 0xdf, 0xdb, 0x3f, 0xdb, 0x78};
Byte CBCPT[100];
Byte CBCIV[16] =  {0x4c, 0xa0, 0x0f, 0xf4, 0xc8, 0x98, 0xd6, 0x1e,
	               0x1e, 0xdb, 0xf1, 0x80, 0x06, 0x18, 0xfb, 0x28};
int CTRCT_Length = 44;
Byte CTRCT[100] = {0x69, 0xdd, 0xa8, 0x45, 0x5c, 0x7d, 0xd4, 0x25,
                   0x4b, 0xf3, 0x53, 0xb7, 0x73, 0x30, 0x4e, 0xec,
                   0x06, 0xf6, 0x47, 0x73, 0x7d, 0x09, 0x86, 0xa2,
                   0xb0, 0x51, 0x01, 0x06, 0xaf, 0xb2, 0x18, 0xc9,
                   0x8b, 0x94, 0xa0, 0xf8, 0xbb, 0x00, 0x5f, 0xf8,
                   0x32, 0x25, 0x87, 0xc7};           
Byte CTRPT[100];  
Byte CTRIV[16] =  {0x69, 0xdd, 0xa8, 0x45, 0x5c, 0x7d, 0xd4, 0x25,
	               0x4b, 0xf3, 0x53, 0xb7, 0x73, 0x30, 0x4e, 0xec};
//Use the PKCS7 to do the padding
void CBC_Padding()
{
	int upperbound;//the maximum length of the ciphertext
	int interval;//the number of byte that need padding
	upperbound = 16 * (CBCCT_Length / 16 + 1);
	interval = upperbound - CBCCT_Length;
	for(int i = CBCCT_Length;i < upperbound;i++){
		CBCCT[i] = interval;
	}
	CBCCT_Length = upperbound;
}
//Use 0x0 to do the padding
void CTR_Padding()
{
	int upperbound;//the maximum length of the ciphertext
	if(CTRCT_Length % 6 != 0){
		upperbound = 16 * (CTRCT_Length / 16 + 1);
		for(int i = CTRCT_Length;i < upperbound;i++){
			CTRCT[i] = 0x0;
		}
		CTRCT_Length = upperbound;
	}
}
//Define the exclusive operation between two byte matrices
//the final result is assigned to the matrix a
void xor_Oper(Byte a[][4], Byte b[]){
	Byte temp[4][4];
	for(int i = 0;i < 4;i++){
		for(int j = 0;j <4;j++){
			temp[i][j] = a[i][j] ^ b[i+j*4];
		}
	}
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			a[i][j] = temp[i][j];
		}
	}
}
//Decrypt CBC ciphertext using AES_Decryption
void CBC_Decryption()
{
	cout << "Now preparing to do the padding..." << endl;
	//do the padding
	CBC_Padding();
	cout << "Now preparing to expand the key..." << endl;
	//expand the key
	Byte* fullCBCKey = keyExpansion(CBCKey);
	//calculate the number of iterations we need to do
	int loop = CBCCT_Length / 16;
	cout << "Now preparing to decrypt the CBC cipher texts..." << endl;
	for(int i = 1;i < loop;i++)
	{
		//get new 16 byte cipher texts
		Byte temp_CT[4][4] = {{CBCCT[16*i + 0],  CBCCT[16*i + 4],  CBCCT[16*i + 8],  CBCCT[16*i + 12]},
							  {CBCCT[16*i + 1],  CBCCT[16*i + 5],  CBCCT[16*i + 9],  CBCCT[16*i + 13]},
							  {CBCCT[16*i + 2],  CBCCT[16*i + 6],  CBCCT[16*i + 10], CBCCT[16*i + 14]},
				  			  {CBCCT[16*i + 3],  CBCCT[16*i + 7],  CBCCT[16*i + 11], CBCCT[16*i + 15]}};
		Byte temp_IV[16];
		//initilize the initial vector
		for(int j = 0;j < 16;j++){
			temp_IV[j] = CBCCT[16*(i-1)+j];
		}
		AES_Decryption(temp_CT, fullCBCKey);
		xor_Oper(temp_CT, temp_IV);
		//assign the result to the array which stores the plain text
		for(int j = 0;j < 4;j++){
			for(int k = 0;k < 4;k++){
				CBCPT[(i-1)*16+j+k*4] = temp_CT[j][k];
			}
		}
	}
	cout << "CBC Decryption done." << endl;
	//rid the padding part
	CBCCT_Length -= CBCCT[CBCCT_Length - 1]+16;
}
//Decrypt CTR ciphertext using AES_Encryption
void CTR_Decryption()
{
	cout << "Now preparing to do the padding..." << endl;
	//do the padding
	CTR_Padding();
	cout << "Now preparing to expand the key..." << endl;
	//expand the key
	Byte* fullCTRKey = keyExpansion(CTRKey);
	//find the numbers of iterations we need to go through
	int loop = CTRCT_Length / 16;
	cout << "Now preparing to initialize the nounce counter..." << endl;
	//define the nounce counter
	Byte nounceCounter[4][4] = {{CTRCT[0],  CTRCT[4],  CTRCT[8],  CTRCT[12]},
							    {CTRCT[1],  CTRCT[5],  CTRCT[9],  CTRCT[13]},
					            {CTRCT[2],  CTRCT[6],  CTRCT[10], CTRCT[14]},
				  	  	        {CTRCT[3],  CTRCT[7],  CTRCT[11], CTRCT[15]}};
	cout << "Now preparing to decrypt the CBC cipher texts..." << endl;
	for(int i = 1;i < loop;i++)
	{
		Byte temp_IV[4][4];
		Byte temp_CT[16];
		//get the new 16 bytes cipher texts
		for(int j = 0;j < 16;j++){
			temp_CT[j] = CTRCT[i*16+j];
		}
		//initialize the intitial vector
		for(int j = 0;j < 4;j++){
			for(int k = 0;k < 4;k++){
				temp_IV[j][k] = nounceCounter[j][k];
			}
		}
		AES_Encryption(temp_IV, fullCTRKey);
		xor_Oper(temp_IV, temp_CT);
		//assign the final result to the array which stores the plain text
		for(int j = 0;j < 4;j++){
			for(int k = 0;k < 4;k++){
				CTRPT[(i-1)*16+j+k*4] = temp_IV[j][k];
			}
		}
		//add 1 to the vlaue of the nounce counter, considering the case of carry
		int index = 15;
		while(nounceCounter[index%4][index/4] == 0xff && index >= 0){
			nounceCounter[index%4][index/4] = 0x0;
			index--;
		}
		if(index >= 0){
			nounceCounter[index%4][index/4] += 0x1;
		}

	}
	cout << "CTR Decryption done." << endl;
	//rid the padding part
	int padding_cnt = 0;
	int temp_index = CTRCT_Length - 1;
	while(CTRCT[temp_index] == 0x0){
		padding_cnt++;
		temp_index--;
	}
	CTRCT_Length -= 16 + padding_cnt;
}


int main() 
{
	cout << "Phase1:CBC Decryption" << endl;
	CBC_Decryption();
	cout << "The plain text by CBC Decryption is as followed:" << endl << endl;
	for(int i = 0;i < CBCCT_Length;i++){
		cout << CBCPT[i];
	}
	cout << endl << endl;
	cout << "Phase2:CTR Decryption" << endl;
	CTR_Decryption();
	cout << "The plain text by CTR Decryption is as followed:" << endl << endl;
	for(int i = 0;i < CTRCT_Length;i++){
		cout << CTRPT[i];
	}
	cout << endl << endl;
	system("pause");
	return 0;
}