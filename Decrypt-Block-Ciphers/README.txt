此程序没有输入文件与输出文件。
若需要修改密文或者秘钥，在定义它们的初始值的位置修改即可，注意也需要更新它们的长度。
此程序包含以下几个函数。
CBC_Padding()：
对CBC密文长度不足16的整数倍的进行补位操作。
CTR_Padding()：
对CTR密文长度不足16的整数倍的进行补位操作。
xor_Oper(Byte a[][4], Byte b[])：
二维矩阵与一位数组之间进行异或操作。
CBC_Decryption()：
对CBC的密文进行解密操作。
CTR_Decryption()：
对CTR的密文进行解密操作。