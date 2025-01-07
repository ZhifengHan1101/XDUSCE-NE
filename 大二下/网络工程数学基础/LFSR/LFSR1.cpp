#include <iostream>
#include <stdlib.h>
#include <bitset>
#define n 33
using namespace std;
int main()
{
	//初始化n位的二进制数据,初始状态为二进制1000，及十进制8
	bitset<n>bint(64);
	bitset<n>str(bint);
	string s1,s2;//保存输出结果
	
	cout << "初始状态为："<<bint.to_string() << endl;
	
	//生成LFSR的状态变化
	do
	{
		int j = bint[n - 1] ^ bint[0];
		bint.operator >>= (1);//向右移1位
		//第1个数据和第n个数据异或然后赋值给第n个
		bint[n - 1] = j;
		cout << bint.to_string() << endl;
		//把bint转换成string放入s1
		s1 = bint.to_string();
		//把s1放入s2中
		s2.push_back(s1[3]);
 
	} 
	while (str.to_string() != bint.to_string());
	cout << "输出序列为：" <<s2<< endl;
	return 0;
}