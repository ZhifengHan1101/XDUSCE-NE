import random


def gcd(a, b):
    """
    用欧几里得算法计算最大公因数
    """
    if a < b:
        temp = a
        a = b
        b = temp

    if a % b == 0:
        return b

    return gcd(b, a % b)


def modular_exponentiation(a, p, m):
    """
    快速模指数运算，a^p(mod m)
    """
    binary_p = bin(p)
    reversed_binary_p = binary_p[len(binary_p):1:-1]

    # 运算结果
    result = 1
    # 每一个二进制位对应底数
    bn = a
    for n in range(len(reversed_binary_p)):
        result = result * bn ** int(reversed_binary_p[n]) % m
        bn = bn ** 2 % m

    return result


def main():
    k = 0
    print("请输入安全参数k：")
    k = int(input())

    m = 0
    print("请输入待检验的奇数：")
    m = int(input())

    if m % 2 == 0:
        print("该数为合数")
        return 0
    if m < 3:
        print("请输入不小于3的奇数")
        return 0
    if m == 3:
        print("该数为素数")
        return 0

    for i in range(k):
        a = random.randint(2, m - 2)
        if gcd(a, m) != 1:
            print("该数为合数")
            return 0
        if modular_exponentiation(a, m - 1, m) != 1:
            print("该数为合数")
            return 0

    print("该数为素数的概率为", 1 - 0.5 ** k, sep='')


if __name__ == '__main__':
    main()
