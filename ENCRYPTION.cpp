#include "DATA.h"
#include "OpenSSL/include/openssl/evp.h"
#include "OpenSSL/include/openssl/Rand.h"



string DecryptFunc(string& encryped) 
//decrypt cordinate encryption
{
    //goes through arr , if digit and not zero set digit to shared E_KEY-digit
    string decrypted;
    for (char tav : encryped)
    {
        if (isdigit(tav)&&tav!='0')
        {
            int num = tav -'0';
            char c = (10 - num) + '0';
            decrypted += c;
        }
        else
        {
            decrypted += tav;
        }
    }
    return decrypted;
}



#include "OpenSSL/include/openssl/bio.h"
#include "OpenSSL/include/openssl/buffer.h"





// AES Encryption Function
string AESencrypt(const string& key, string& str)
{
    // If the input string is shorter than the AES block size, padding is added
    while (str.size() < AES_BLOCK_SIZE)
    {
        str.push_back('~');
    }

    // A new context for the encryption operation is created
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    // A new random initialization vector is generated
    unsigned char iv[EVP_MAX_IV_LENGTH];
    RAND_bytes(iv, EVP_MAX_IV_LENGTH);

    // The type of cipher is set (AES 128-bit in CBC mode)
    const EVP_CIPHER* cipherType = EVP_aes_128_cbc();
    EVP_EncryptInit_ex(ctx, cipherType, NULL, reinterpret_cast<const unsigned char*>(key.c_str()), iv);

    // The input text is encrypted
    int len;
    string ciphertext;
    unsigned char outbuf[1024];
    EVP_EncryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
    ciphertext = string(reinterpret_cast<char*>(outbuf), len);

    // Encryption is finalized
    EVP_EncryptFinal_ex(ctx, outbuf, &len);
    ciphertext += string(reinterpret_cast<char*>(outbuf), len);

    // Padding is added to the encrypted text to make it a multiple of the base64 block size
    while (ciphertext.size() % BASE_64_BLOCK != 0)
    {
        ciphertext.push_back('~'); // Using '~' as padding character
    }

    // The context for the encryption operation is freed
    EVP_CIPHER_CTX_free(ctx);

    // The IV and encrypted string are combined into a single string
    vector<unsigned char> result(iv, iv + EVP_MAX_IV_LENGTH);
    result.insert(result.end(), ciphertext.begin(), ciphertext.end());

    // The binary data is converted into a Base64 string before returning it
    return base64encode(string(result.begin(), result.end()));
}



string AESdecrypt(const string& key, string& encryptedStr)
{
    string data = base64decode(encryptedStr);

    // Create and initialize context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_set_padding(ctx, 1); // Enable automatic padding

    // Extract IV from the beginning of the encrypted input
    string iv = data.substr(0, EVP_MAX_IV_LENGTH);
    string ciphertext = data.substr(EVP_MAX_IV_LENGTH);

    // Set cipher type and mode
    const EVP_CIPHER* cipherType = EVP_aes_128_cbc();
    EVP_DecryptInit_ex(ctx, cipherType, NULL, reinterpret_cast<const unsigned char*>(key.c_str()), reinterpret_cast<const unsigned char*>(iv.c_str()));

    // Decrypt the ciphertext
    int len;
    string padded;
    string plaintext;
    unsigned char outbuf[1024];
    EVP_DecryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.length());
    padded = string(reinterpret_cast<char*>(outbuf), len);

    // Finalize decryption
    EVP_DecryptFinal_ex(ctx, outbuf, &len);
    padded += string(reinterpret_cast<char*>(outbuf), len);

    // Free context
    EVP_CIPHER_CTX_free(ctx);

    // Remove padding
    size_t index = 0;
    while (index != padded.size() && padded[index] != '~')
    {
        plaintext += padded[index];
        index++;
    }

    return plaintext;
}

// Function to encode data into Base64 format
string base64encode(const string& str) {
    BIO* bio = BIO_new(BIO_f_base64());
    BIO* bmem = BIO_new(BIO_s_mem());
    bio = BIO_push(bio, bmem);
    BIO_write(bio, str.c_str(), str.size());
    BIO_flush(bio);
    BUF_MEM* bptr = NULL;
    BIO_get_mem_ptr(bio, &bptr);

    string output(bptr->data, bptr->length - 1); // -1 is to remove the newline at the end

    BIO_free_all(bio);
    return output;
}

// Function to decode Base64 data back into its original format
string base64decode(const string& str) {
    BIO* bio = BIO_new_mem_buf(str.c_str(), -1);
    bio = BIO_push(BIO_new(BIO_f_base64()), bio);
    string output(str.size(), '\0'); // Max size to read
    int decoded_size = BIO_read(bio, &output[0], output.size());
    output.resize(decoded_size);

    BIO_free_all(bio);
    return output;
}