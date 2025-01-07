from rsa.common import inverse


class Equation:
    a = 0
    m = 0

    def __init__(self, a, m):
        self.a = a
        self.m = m


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


def is_relatively_prime(eq : [Equation]) -> [int, int]:
    for i in range(len(eq)):
        for j in range(i + 1, len(eq)):
            if gcd(eq[i].m, eq[j].m) != 1:
                return [i, j]
    return [-1, -1]


def chinese_remainder_theorem(eq) -> [int, int]:
    """
    中国剩余定理
    return [x, m]
    """
    # 当mi不互素时，尝试分解使其互素
    not_relatively_prime_num = is_relatively_prime(eq)
    while not_relatively_prime_num != [-1, -1]:
        gcd_num = gcd(eq[not_relatively_prime_num[0]].m, eq[not_relatively_prime_num[1]].m)
        # 存在不互素的mi时，先判断是否成倍数，成倍数则删去较小数，否则分解
        if gcd_num == eq[not_relatively_prime_num[0]].m:
            eq.remove(eq[not_relatively_prime_num[0]])
        elif gcd_num == eq[not_relatively_prime_num[1]].m:
            eq.remove(eq[not_relatively_prime_num[1]])
        else:
            # 中国剩余定理无法求解
            if eq[not_relatively_prime_num[0]].a % gcd_num != eq[not_relatively_prime_num[1]].a % gcd_num:
                return [-1, -1]
            eq[not_relatively_prime_num[0]].m = eq[not_relatively_prime_num[0]].m / gcd_num
            eq[not_relatively_prime_num[0]].a = eq[not_relatively_prime_num[0]].a % eq[not_relatively_prime_num[0]].m

            eq[not_relatively_prime_num[1]].m = eq[not_relatively_prime_num[1]].m / gcd_num
            eq[not_relatively_prime_num[1]].a = eq[not_relatively_prime_num[1]].a % eq[not_relatively_prime_num[1]].m
            eq.append(Equation(eq[not_relatively_prime_num[0]].a % gcd_num, gcd_num))

        not_relatively_prime_num = is_relatively_prime(eq)

    M = 1
    x = 0
    for i in range(len(eq)):
        M = M * eq[i].m

    for i in range(len(eq)):
        mi = 1
        for j in range(len(eq)):
            if j != i:
                mi = mi * eq[j].m
        x = x + mi * inverse(mi, eq[i].m) * eq[i].a
        x = x % M

    return [x, M]


def main():
    equations1 = []
    a1 = []
    m1 = []
    with open('1.txt', 'r') as f:
        count = 0
        for line in f:
            count = count + 1
            if count <= 3:
                a1.append(int(line))
            else:
                m1.append(int(line))
    f.close()

    for i in range(len(a1)):
        equations1.append(Equation(a1[i], m1[i]))
    result = [chinese_remainder_theorem(equations1)]

    equations2 = []
    a2 = []
    m2 = []
    with open('2.txt', 'r') as f:
        count = 0
        for line in f:
            count = count + 1
            if count <= 3:
                a2.append(int(line))
            else:
                m2.append(int(line))
    f.close()

    for i in range(len(a2)):
        equations2.append(Equation(a2[i], m2[i]))
    result.append(chinese_remainder_theorem(equations2))

    equations3 = []
    a3 = []
    m3 = []
    with open('3.txt', 'r') as f:
        count = 0
        for line in f:
            count = count + 1
            if count <= 3:
                a3.append(int(line))
            else:
                m3.append(int(line))
    f.close()

    for i in range(len(a3)):
        equations3.append(Equation(a3[i], m3[i]))
    result.append(chinese_remainder_theorem(equations3))

    equations4 = []
    a4 = []
    m4 = []
    with open('4.txt', 'r') as f:
        count = 0
        for line in f:
            count = count + 1
            if count <= 3:
                a4.append(int(line))
            else:
                m4.append(int(line))
    f.close()

    for i in range(len(a4)):
        equations4.append(Equation(a4[i], m4[i]))
    result.append(chinese_remainder_theorem(equations4))

    if result[0] == [-1, -1]:
        print("1.txt中一次同余方程由于mi不互素，无法用中国剩余定理求解")
    else:
        print("1.txt中一次同余方程的解为", result[0][0], sep='')
    if result[1] == [-1, -1]:
        print("2.txt中一次同余方程由于mi不互素，无法用中国剩余定理求解")
    else:
        print("2.txt中一次同余方程的解为", result[1][0], sep='')
    if result[2] == [-1, -1]:
        print("3.txt中一次同余方程由于mi不互素，无法用中国剩余定理求解")
    else:
        print("3.txt中一次同余方程的解为", result[2][0], sep='')
    if result[3] == [-1, -1]:
        print("4.txt中一次同余方程由于mi不互素，无法用中国剩余定理求解")
    else:
        print("4.txt中一次同余方程的解为", result[3][0], sep='')


if __name__ == '__main__':
    main()
# eq = [Equation(11, 12), Equation(3, 10)]
# print(chinese_remainder_theorem(eq))
