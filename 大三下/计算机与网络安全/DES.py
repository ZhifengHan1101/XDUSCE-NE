from pyDes import des, CBC, PAD_PKCS5
import binascii

# 秘钥
KEY = 'hanzhifg'

def des_encrypt(s):
    """
    DES 加密
    :param s: 原始字符串
    :return: 加密后字符串，16进制
    """
    secret_key = KEY  # 密码
    iv = secret_key  # 偏移
    # secret_key:加密密钥，CBC:加密模式，iv:偏移, padmode:填充
    des_obj = des(secret_key, CBC, iv, pad=None, padmode=PAD_PKCS5)
    # 返回为字节
    secret_bytes = des_obj.encrypt(s, padmode=PAD_PKCS5)
    # 返回为16进制
    return binascii.b2a_hex(secret_bytes)


def des_descrypt(s):
    """
    DES 解密
    :param s: 加密后的字符串，16进制
    :return:  解密后的字符串
    """
    secret_key = KEY
    iv = secret_key
    des_obj = des(secret_key, CBC, iv, pad=None, padmode=PAD_PKCS5)
    decrypt_str = des_obj.decrypt(binascii.a2b_hex(s), padmode=PAD_PKCS5)
    return decrypt_str


plaintext = "this is the text to be encrypted"
print(f"plaintext: {plaintext}")
des_encrypted = des_encrypt(plaintext)
print(f"encrypted: {des_encrypted}")
des_decrypted = des_descrypt(des_encrypted)
print(f"decrypted: {des_decrypted}")
