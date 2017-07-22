此程序不需要文件输入。
此程序有四个函数，initInput(), xorOper(), decrypt(), printModificationHint()。

initInput()负责初始化所有给定的CT，和需要破译的目标CT，以及最终的结果目标PT。
如果需要改变破译的内容，只需要在这个函数中改变targetCT的值和targetPT的长度即可。
targetPT的长度需要是targetCT的一半，因为CT中两个十六进制数表示PT中的一个字符。

xorOper()函数定义了两个十六进制的字符间的异或运算，返回的结果是异或之后的十六进制字符串。

decrypt()函数查找所有给定的CT对应的PT空格所在的位置，并利用空格异或字母的特性直接得到targetPT相应位置的字母。如果不是字母的话，则无法得到。
另外，此函数也会查找targetPT空格所在的位置，方法类似。
decrypt()采用的方法直接跳过了求解key的实质值，而是直接利用空格异或字母得到字母的性质进行targetPT的求解。

printModificationHint()函数允许用户对于程序输出的内容进行更改。
因为在大部分的字母已经被破译的情况下，人脑可以自动补足其他的字母，从而使得到的结果更加精确。

此程序输出的内容为程序处理过程的提示以及最终的targetPT结果。
因为没有计算key的实质值，程序不会输出给定的CT对应的PT的预测值。