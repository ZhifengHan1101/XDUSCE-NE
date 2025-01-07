from random import randint
from gmssl import sm3, func
from rsa.common import inverse


def add_points(x1, y1, x2, y2, a, p):
    """
    椭圆曲线上两点相加
    """
    if x1 == x2 and y1 + y2 == p:
        return False
    elif x1 == x2 and y1 == y2:
        lambda_of_points = ((3 * x1 ** 2 + a) * inverse(2 * y1, p)) % p
    else:
        lambda_of_points = ((y2 - y1) * inverse(x2 - x1, p)) % p

    x3 = (lambda_of_points ** 2 - x1 - x2) % p
    y3 = (lambda_of_points * (x1 - x3) - y1) % p
    return x3, y3


def multiple_point(x1, y1, k, a, p):
    """
    计算倍点，使用第一部分附录A.3.2中算法一：二进制展开法快速计算倍点
    """
    k = bin(k)[2:]
    qx, qy = x1, y1
    for i in range(1, len(k)):
        qx, qy = add_points(qx, qy, qx, qy, a, p)
        if k[i] == '1':
            qx, qy = add_points(qx, qy, x1, y1, a, p)
    return qx, qy


def str2byte(str):
    """
    字符串转换为01比特串
    """
    rule = 'utf-8'
    byte_str = str.encode(rule)
    bc = [bin(i)[2:].rjust(8, '0') for i in byte_str]
    bytes = ''.join(bc)
    return bytes


def kdf(z, klen):
    """
    kdf函数
    """
    ct = 1
    v = 256
    k = ''
    for i in range(int(klen / v) + 1):
        k = k + Hv(hex(int(z + '{:02x}'.format(ct), 16))[2:])
        ct = ct + 1

    bin_k = bin(int(k, 16))[2:]
    return hex(int(bin_k[:klen], 2))[2:]


def Hv(str):
    """
    密码杂凑函数SM3，输入输出均为16进制
    """
    return sm3.sm3_hash(func.bytes_to_list(bytes(str, encoding='utf-8')))


def encrypt(m, xG, yG, xB, yB, a, p, n):
    """
    加密算法
    """
    # 首先将消息转换为二进制比特串
    byte_m = str2byte(m)

    while 1:
        # A1：生成随机的k
        k = randint(1, n - 1)
        # k = 0x4C62EEFD6ECFC2B95B92FD6C3D9575148AFA17425546D49018E5388D49DD7B4F

        # A2：计算C1并转换为16进制比特串
        c1x, c1y = multiple_point(xG, yG, k, a, p)
        c1x = hex(c1x)[2:]
        c1y = hex(c1y)[2:]
        C1 = '04' + c1x + c1y
        print("C1:\n"+C1+"\n")

        # A3：根据O + O = O判断S是否是无穷远点
        h = 1
        hPBx, hPBy = multiple_point(xB, yB, h, a, p)
        hPBx2, hPBy2 = multiple_point(hPBx, hPBy, 2, a, p)
        if hPBx == hPBx2 and hPBy == hPBy2:
            print("加密时S为无穷远点，发生错误")
            exit()

        # A4：计算(x2, y2)
        x2, y2 = multiple_point(xB, yB, k, a, p)
        x2 = hex(x2)[2:]
        y2 = hex(y2)[2:]

        print("x2:\n"+x2+"\ny2:\n"+y2)

        # A5：计算t = KDF(x2 || y2, klen)，并根据t是否是全0字符串判断k是否合适
        t = kdf(x2 + y2, len(byte_m))
        if int(t, 16) != 0:
            break

        print("t:\n"+t)

    # A6：计算C2 = M ⊕ t
    C2 = int(byte_m, 2) ^ int(t, 16)
    C2 = str(bin(C2)[2:])
    C2 = hex(int(C2, 2))[2:]

    print("C2:\n"+C2)

    # A7：计算C3 = Hash(x2 || M || y2)
    C3 = Hv(x2 + hex(int(byte_m, 2))[2:] + y2)

    print("C3:\n"+C3)

    # A8：输出密文
    return C1 + C2 + C3


def decrypt(C, a, b, p, dB, point_len):
    """
    解密算法
    """
    C1 = C[:2 + 2 * point_len]
    C2 = C[2 + 2 * point_len:-64]
    C3 = C[len(C1) + len(C2):]

    # B1：取C1并判断是否是椭圆曲线上的点
    C1 = C1[2:]
    x1 = C1[:int(len(C1) / 2)]
    y1 = C1[int(len(C1) / 2):]
    x1 = int(x1, 16)
    y1 = int(y1, 16)

    if pow(y1, 2, p) != (pow(x1, 3, p) + a * x1 + b) % p:
        print("解密时C1不满足椭圆曲线方程，发生错误")
        exit()

    print("x1:\n"+hex(x1)[2:]+"\ny1:\n"+hex(y1)[2:])

    # B2：根据O + O = O判断S是否是无穷远点
    h = 1
    hPBx, hPBy = multiple_point(x1, y1, h, a, p)
    hPBx2, hPBy2 = multiple_point(hPBx, hPBy, 2, a, p)
    if hPBx == hPBx2 and hPBy == hPBy2:
        print("解密时S为无穷远点，发生错误")
        exit()

    # B3：计算(x2,y2)
    x2, y2 = multiple_point(x1, y1, dB, a, p)
    x2 = hex(x2)[2:]
    y2 = hex(y2)[2:]

    print("x2:\n" + x2 + "\ny2:\n" + y2)

    # B4：计算t = KDF(x2 || y2, klen)，若t是否是全0字符串则报错
    t = kdf(x2 + y2, len(C2) * 4)
    if int(t, 16) == 0:
        print("解密时t取值发生错误")
        exit()
    print("t:\n" + t)

    # B5：取出C2并计算M′ = C2 ⊕ t
    bytes_decrypted_m = int(C2, 16) ^ int(t, 16)

    # B6:计算u = Hash(x2 || M′ || y2)，并判断u是否等于C3，若不等于则报错
    u = Hv(x2 + hex(bytes_decrypted_m)[2:] + y2)
    if u != C3:
        print("解密时u发生错误")
        exit()
    print("u:\n"+u)

    # B7：输出明文
    hex_decrypted_m = hex(bytes_decrypted_m)[2:]
    m = str(bytes.fromhex(hex_decrypted_m))[2:-1]
    print("解密算法中数据验证通过，恢复的消息为：\n" + m)

    return m


if __name__ == "__main__":
    # 初始参数设置，使用Fp上椭圆曲线

    p = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
    a = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
    b = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
    n = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
    xG = 0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
    yG = 0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0

    print("本次加密过程中\np为：\n" + hex(p) + "\na为：\n" + hex(a) + "\nb为：\n" + hex(b) +"\nn为：\n" + hex(n) +"\nGx为：\n" + hex(xG) +"\nGy为：\n" + hex(yG))
    print()
    # 生成公私钥
    dB = randint(1, n - 2)
    print("产生随机数k：",dB)

    xB, yB = multiple_point(xG, yG, dB, a, p)

    print("私钥dB为：\n" + hex(dB) + "\n公钥PB为：\n(" + hex(xB) + ", " + hex(yB) + ")\n")

    m = input("请输入要加密的消息：\n")

    # 使用公钥加密
    print("-------------------------加密参数------------------------")
    C = encrypt(m, xG, yG, xB, yB, a, p, n)

    print("密文C=\n" + str(C), end="\n\n")

    # 使用私钥解密
    print("------------------------解密参数------------------------")
    decrypted_m = decrypt(C, a, b, p, dB, len(str(hex(xG)[2:])))
    if decrypted_m == m:
        print("恢复的消息与原消息一致")
    else:
        print("恢复的消息与原消息不一致")
