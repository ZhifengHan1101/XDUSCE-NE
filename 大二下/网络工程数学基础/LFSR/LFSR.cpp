#include <iostream>
#include <bitset>
#define N 33
#define maxiteration 130        // 66 + 64

using namespace std;

int main()
{
    bitset<N> shiftRegister = 0b000100010001000100010001000100010;     // 初始化寄存器的值
    bitset<N> PolyNomial = 0b000110011001100110011001100110011;        // 初始化多项式的值

    long int i = 0;

    while(i < maxiteration)
    {
        if(i >= 66)
            cout << shiftRegister[0] << endl;   // LFSR运行66个clock后,输出寄存器最右端的值
        //cout << shiftRegister.to_string() << endl;

        bitset<N> temp = shiftRegister & PolyNomial;    // 多项式与寄存器的值相与
        shiftRegister.operator>>= (1);      // 寄存器中值右移1位
        int feedback = temp.count() % 2;    // 计算temp中1的个数,个数取余2，即求出feedback
        shiftRegister[N - 1] = feedback;    // 更新寄存器最左端的值

        i ++ ;
    }

    cout << "此时寄存器中的值: " << shiftRegister.to_string() << endl;

    return 0;
}