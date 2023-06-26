#pragma once
#include "imports.h"
#define BIT_8 8
#define BIT_6 6

#define POW '^'
#define PLUS '+'
#define MINUS '-'
#define DIVIDE '/'
#define MULTIPLY '*'
#define PAR_OPEN '('
#define PAR_CLOSE ')'
#define DOT '.'



#define SLEEP std::this_thread::sleep_for(std::chrono::seconds(5)); // sleep btween sending large chink opf data

static vector<char>  valid_char = { POW ,MINUS,PLUS , DIVIDE,MULTIPLY, PAR_OPEN,PAR_CLOSE, DOT , '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' , 'X','x'}; //accepted chars in equation 

#define MinMax_Obj pair<vector<pair<float, float>>, vector<pair<float, float>>>
//base 64 section


 const string AESkey = "ORICOHENBAZAK:)"; //AES key
#define AES_BLOCK_SIZE 16
 #define BASE_64_BLOCK 3
string AESencrypt(const string& key,  string& str);
string AESdecrypt(const string& key,  string& str);
string base64encode(const string& str);
string base64decode(const string& str);
enum class RequestType {
    //enum that is used to assign each request with integer to add as flag in request send to server
    newFunc = 1,
    intersects = 2,
    MinMax = 3,
    boundries = 4,
    terminate = 5,
    DRAW = 6,
    reset = 7,
};



//SOCKET
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <chrono>
#include <thread>

#define PORT 1500
#define SUCCESS_RESPONSE 1
#define ERROR_RESPONSE 0 



#pragma comment(lib,"OpenSSL/lib/libcrypto.lib")
#pragma comment(lib,"OpenSSL/lib/libssl.lib")


string DecryptFunc(string& str);


//GUI

vector<bool> mergeDefinedVectors(vector<bool>& v1, vector<bool>& v2);

