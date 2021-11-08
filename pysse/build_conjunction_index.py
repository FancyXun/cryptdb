import pandas as pd
from Crypto.Cipher import AES
from Crypto.Hash import MD5
from Crypto.Random import random
from Crypto import Random
import numpy as np
import time
import codecs
import pickle
import base64


BLOCK_SIZE = 16
pad = lambda s: s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * chr(BLOCK_SIZE - len(s) % BLOCK_SIZE)
unpad = lambda s: s[:-ord(s[len(s) - 1:])]

def build_trapdoor(MK, keyword):
    keyword_index = MD5.new()
    keyword_index.update(str(keyword).encode("utf-8"))
    ECB_cipher = AES.new(MK, AES.MODE_ECB)
    return ECB_cipher.encrypt(keyword_index.digest())


def build_codeword(ID, trapdoor):
    raw = pad(str(ID))
    iv = Random.new().read(AES.block_size)
    CBC_cipher = AES.new(trapdoor, AES.MODE_CBC, iv)
    return base64.b64encode(iv + CBC_cipher.encrypt(raw))


def build_index(MK, ID, trapdoor):
    codeword = build_codeword(ID, trapdoor)
    return codeword

def searchable_encryption(raw_data_file_name, master_key, keyword_type):
    raw_data = pd.read_csv(raw_data_file_name)
    features = list(raw_data)
    raw_data = raw_data.values
    
    feature_index = 0
    for i in range(0, len(features)):
        if features[i] == keyword_type:
            feature_index = i
            break

    kw = {}
    start_time = time.time()
    for row in range(raw_data.shape[0]):
        record = raw_data[row]
        tmp_kw = []
        for i in list(record[feature_index]):
            trapdoor = build_trapdoor(master_key, i)
            if trapdoor in tmp_kw:
                continue
            tmp_kw.append(trapdoor)
            if trapdoor not in kw.keys():
                kw[trapdoor] = []
            record_index = build_index(master_key, row, trapdoor)
            kw[trapdoor].append(record_index)

    time_cost = time.time() - start_time
    print (time_cost)
    document_index_dict = open(raw_data_file_name.split(".")[0] + "_index.pkl", "wb")
    pickle.dump(kw, document_index_dict)

if __name__ == "__main__":

    document_name = "sample_conjunction.csv"
    master_key = "hdakudynzjdylopt"
    keyword_type = "a3"

    searchable_encryption(document_name, master_key, keyword_type)
    print ("Finished")
