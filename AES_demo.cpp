#include <iostream>
#include <fstream>
#include <string>

#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;
using namespace std;

// --- Hàm hỗ trợ lưu/đọc khóa ---
void SaveKey(const string& filename, const SecByteBlock& key) {
    StringSource ss(key.data(), key.size(), true, new FileSink(filename.c_str()));
}

SecByteBlock LoadKey(const string& filename) {
    string keyStr;
    FileSource fs(filename.c_str(), true, new StringSink(keyStr));
    return SecByteBlock((const CryptoPP::byte*)keyStr.data(), keyStr.size());
}

// --- Logic AES ---
void RunAesEncrypt() {
    string inFile = "input.txt", outFile = "aes_encrypted.bin", keyFile = "aes_key.bin";
    
    AutoSeededRandomPool prng;
    SecByteBlock key(AES::MAX_KEYLENGTH); // 32 bytes (256 bits)
    prng.GenerateBlock(key, key.size());
    SaveKey(keyFile, key); 

    SecByteBlock iv(AES::BLOCKSIZE); // 16 bytes
    prng.GenerateBlock(iv, iv.size());

    CBC_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(key, key.size(), iv);

    string cipher;
    FileSource(inFile.c_str(), true, new StreamTransformationFilter(encryptor, new StringSink(cipher)));

    ofstream out(outFile, ios::binary);
    out.write((const char*)iv.BytePtr(), iv.size());
    out.write(cipher.data(), cipher.size());
    out.close();

    cout << "Encrypted AES sucessfully!\n -> Key: " << keyFile << "\n -> File: " << outFile << endl;
}

void RunAesDecrypt() {
    string inFile = "aes_encrypted.bin", outFile = "aes_decrypted.txt", keyFile = "aes_key.bin";
    SecByteBlock key = LoadKey(keyFile);

    ifstream in(inFile, ios::binary);
    SecByteBlock iv(AES::BLOCKSIZE);
    in.read((char*)iv.BytePtr(), iv.size());

    string cipher((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    string recovered;

    CBC_Mode<AES>::Decryption decryptor;
    decryptor.SetKeyWithIV(key, key.size(), iv);
    StringSource(cipher, true, new StreamTransformationFilter(decryptor, new StringSink(recovered)));

    ofstream out(outFile, ios::binary);
    out.write(recovered.data(), recovered.size());
    cout << "Decrypted AES sucessfully!\n -> Result at: " << outFile << endl;
}

int main() {
    ofstream testFile("input.txt", ios::app);
    testFile << "Data need to encrypt by AES" << endl;
    testFile.close();

    int choice;
    cout << "=== AES_TOOL-256 ===" << endl;
    cout << "1.(Encrypt)" << endl;
    cout << "2.(Decrypt)" << endl;
    cout << "Choose (1-2): ";
    cin >> choice;

    try {
        if (choice == 1) RunAesEncrypt();
        else if (choice == 2) RunAesDecrypt();
        else cout << "The chosen not suitable!" << endl;
    } catch (const exception& e) {
        cerr << "Error AES: " << e.what() << endl;
    }
    return 0;
}