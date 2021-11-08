import pandas as pd
from Crypto.Cipher import AES
from Crypto.Hash import MD5
import codecs

def build_codeword(ID, trapdoor):
    ID_index = MD5.new()
    ID_index.update(str(ID).encode("utf-8"))
    ECB_cipher = AES.new(trapdoor, AES.MODE_ECB)
    return codecs.encode(ECB_cipher.encrypt(ID_index.digest()), 'hex')

def search_index(document, trapdoor):
    search_result = []
    data_index = pd.read_csv(document)
    data_index = data_index.values
    for row in range(data_index.shape[0]):
        if str(build_codeword(row, trapdoor)) in data_index[row]:
            search_result.append(row)

    return search_result

def build_trapdoor(MK, keyword):
    keyword_index = MD5.new()
    keyword_index.update(str(keyword).encode("utf-8"))
    ECB_cipher = AES.new(MK, AES.MODE_ECB)
    return ECB_cipher.encrypt(keyword_index.digest())

if __name__ == "__main__":

    keyword = input("Please input the keyword you want to search:  ")
    master_key = "hdakudynzjdylopt"
    keyword_trapdoor = build_trapdoor(master_key, keyword)

    index_file_name = "sample_single_index.csv"
    search_result = search_index(index_file_name, keyword_trapdoor)
    print ("The identifiers of files that contain the keyword are: \n", search_result)



