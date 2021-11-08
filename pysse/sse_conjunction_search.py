import pandas as pd
from Crypto.Cipher import AES
from Crypto.Hash import MD5
from Crypto import Random
import codecs
import pickle
import base64

BLOCK_SIZE = 16
pad = lambda s: s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * chr(BLOCK_SIZE - len(s) % BLOCK_SIZE)
unpad = lambda s: s[:-ord(s[len(s) - 1:])]

def build_codeword(ID, trapdoor):
    ID_index = MD5.new()
    ID_index.update(str(ID).encode("utf-8"))
    ECB_cipher = AES.new(trapdoor, AES.MODE_ECB)
    return codecs.encode(ECB_cipher.encrypt(ID_index.digest()), 'hex')

def search_index(data_index, trapdoor):
    search_result = []
    if trapdoor in data_index:
        search_result = data_index[trapdoor]
        real_result = [get_index(trapdoor, i) for i in search_result]
    return real_result

def build_trapdoor(MK, keyword):
    keyword_index = MD5.new()
    keyword_index.update(str(keyword).encode("utf-8"))
    ECB_cipher = AES.new(MK, AES.MODE_ECB)
    return ECB_cipher.encrypt(keyword_index.digest())

def get_index(trapdoor, encrypt_text):
    enc = base64.b64decode(encrypt_text)
    iv = enc[:16]
    cipher = AES.new(trapdoor, AES.MODE_CBC, iv)
    return unpad(cipher.decrypt(enc[16:]))

if __name__ == "__main__":

    keyword = input("Please input the keyword you want to search:  ")
    master_key = "hdakudynzjdylopt"
    keyword = keyword.replace("%","")
    keyword_list = list(keyword)

    index_file_name = "sample_conjunction_index.pkl"
    data_index = pickle.load(open(index_file_name, "rb"))
    
    result = set()
    for i in keyword_list:
        keyword_trapdoor = build_trapdoor(master_key, i)
        if not result:
            result = set(search_index(data_index, keyword_trapdoor))
        else:
            result = result.intersection(set(search_index(data_index, keyword_trapdoor)))
    raw_data = pd.read_csv("sample_conjunction.csv")
    features = list(raw_data)
    raw_data = raw_data.values
    for i in result:
        print(raw_data[int(i)])
    



