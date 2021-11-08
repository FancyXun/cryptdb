import pandas as pd
from Crypto.Cipher import AES
from Crypto.Hash import MD5
from Crypto.Random import random
import numpy as np
import time
import codecs


def build_trapdoor(MK, keyword):
    keyword_index = MD5.new()
    keyword_index.update(str(keyword).encode("utf-8"))
    ECB_cipher = AES.new(MK, AES.MODE_ECB)
    return ECB_cipher.encrypt(keyword_index.digest())


def build_codeword(ID, trapdoor):
    ID_index = MD5.new()
    ID_index.update(str(ID).encode("utf-8"))
    ECB_cipher = AES.new(trapdoor, AES.MODE_ECB)
    return codecs.encode(ECB_cipher.encrypt(ID_index.digest()), 'hex')


def build_index(MK, ID, keyword_list):
    secure_index = [0] * len(keyword_list)
    for i in range(len(keyword_list)):
        codeword = build_codeword(ID, build_trapdoor(MK, keyword_list[i]))
        secure_index[i] = codeword
    random.shuffle(secure_index)
    return secure_index

def searchable_encryption(raw_data_file_name, master_key, keyword_type_list):
    raw_data = pd.read_csv(raw_data_file_name)
    features = list(raw_data)
    raw_data = raw_data.values

    keyword_number = [i for i in range(0, len(features)) if features[i] in keyword_type_list]

    index_header = []
    for i in range(1, len(keyword_type_list) + 1):
        index_header.append("index_" + str(i))

    document_index = []
    start_time = time.time()
    for row in range(raw_data.shape[0]):
        record = raw_data[row]
        record_keyword_list = [record[i] for i in keyword_number]
        record_index = build_index(master_key, row, record_keyword_list)
        document_index.append(record_index)

    time_cost = time.time() - start_time
    print (time_cost)
    document_index_dataframe = pd.DataFrame(np.array(document_index), columns=index_header)
    document_index_dataframe.to_csv(raw_data_file_name.split(".")[0] + "_index.csv")


if __name__ == "__main__":

    document_name = "sample_conjunction.csv"
    master_key = "hdakudynzjdylopt"
    keyword_type_list = ["a3"]

    searchable_encryption(document_name, master_key, keyword_type_list)
    print ("Finished")
