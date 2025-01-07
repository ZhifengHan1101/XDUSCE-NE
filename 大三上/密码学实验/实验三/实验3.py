import random

from rsa.common import inverse


class Equation:
    a = 0
    m = 0

    def __init__(self, a, m):
        self.a = a
        self.m = m


def main():
    k = []
    with open("secret1.txt") as secret1:
        for line in secret1:
            k.append(int(line))
    secret1.close()

    with open("secret2.txt") as secret2:
        for line in secret2:
            k.append(int(line))
    secret2.close()
    n = int(input("请输入n:\n"))
    t = int(input("请输入t:\n"))

    # 分别对两个文件中的秘密进行秘密共享处理
    for i in range(2):
        d = []
        initialise_d(n, t, d, k[i])
        [N, M] = calculate_proper_d(d, n, t, k[i])
        print("第" + str(i + 1) + "个秘密的d分别为：")
        for j in range(len(d)):
            print(d[j])

        print("第" + str(i + 1) + "个秘密的N为：")
        print(N)
        print("第" + str(i + 1) + "个秘密的M为：")
        print(M)
        print()

        # 分别测试选择t-1、t、t+1个d时能否正确得到秘密
        d_num = [t - 1, t, t + 1]
        for test_round in range(3):
            eq = []

            print("请输入秘密还原要使用的的"+str(d_num[test_round])+"个di的序号")
            chosen_d_str = input().split()
            list_chosen_d = [int(num) for num in chosen_d_str]

            # 针对选择的d使用中国剩余定理反求原秘密
            for j in list_chosen_d:
                eq.append(Equation(k[i] % d[j], d[j]))
            result = chinese_remainder_theorem(eq)

            print("使用", end="")
            for index in range(len(list_chosen_d)):
                print("d" + str(list_chosen_d[index]), end="")
                if index < len(list_chosen_d) - 1:
                    print("、", end="")
            print("恢复秘密为：")
            print(result[0])
            if result[0] - k[i] == 0:
                print("恢复的秘密与原秘密相同")
            else:
                print("恢复的秘密与原秘密不同")
            print()
        print()


def initialise_d(n, t, d, k):
    """
    初始化di
    """
    len_k = len(str(k))

    # 根据秘密大小生成相应位数的d
    d.append(random.randint(10 ** (int(len_k / t) + 1), 10 ** (int(len_k / t) + 2)))
    while len(d) < n:
        d.append(d[len(d) - 1] + 1)
        while not is_relatively_prime(d):
            d[len(d) - 1] = d[len(d) - 1] + 1


def calculate_proper_d(d, n, t, k):
    """
    寻找符合要求的d
    """
    N = 1
    for i in range(t):
        N = N * d[i]

    M = 1
    for i in range(n - 1, n - t, -1):
        M = M * d[i]

    # 不满足要求则持续更改d
    while not (N > k > M):
        if not N > M:
            d.clear()
            initialise_d(n, t, d, k)
        elif k >= N:
            # k >= N时，选择的d整体增大
            for i in range(len(d) - 1):
                d[i] = d[i + 1]
            while not is_relatively_prime(d):
                d[len(d) - 1] = d[len(d) - 1] + 1
        else:
            # k <= M时，选择的d整体减小
            for i in range(len(d) - 1, 0, -1):
                d[i] = d[i - 1]
            while not is_relatively_prime(d):
                d[0] = d[0] - 1

        N = 1
        for i in range(t):
            N = N * d[i]

        M = 1
        for i in range(n - 1, n - t, -1):
            M = M * d[i]

    return [N, M]


def chinese_remainder_theorem(eq) -> [int, int]:
    """
    中国剩余定理
    return [x, m]
    """
    # 中国剩余定理求解
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


def is_relatively_prime(d):
    """
    判断di是否两两互素
    返回True表示两两互素，否则返回False
    """
    for i in range(len(d)):
        for j in range(i + 1, len(d)):
            if gcd(d[i], d[j]) != 1:
                return False
    return True


if __name__ == "__main__":
    main()
