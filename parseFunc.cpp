#pragma once
#include "imports.h"
#include "gUi.h"
#include "DATA.h"
#include "flags.h"
// parse a string containing '0' and '1' characters, convert them to boolean values, and store them in a vector. The function terminates and returns the vector when it encounters the 'p' character,
vector<bool> gUi::convertBoolStringToVector(string& str)
{
    //input:: str refrence
    //output: vector<bool>
    vector<bool> v;
    for (char tav : str)
    {
        if (tav == 'p')
        {
            return v;
        }
        v.push_back(tav == '1');


    }
    return v;

}





// parse a string that contains delimited numeric values, extract those values, convert them to floats, and store them in a vector.
//The function stops processing and returns the vector when it encounters the 'p' character, which serves as a delimiter indicating the end of the input values.
vector<float> gUi::convertFloatStringToVector(string& str)
{
    //input : str refrence
    //output : vector<float>
    vector<float> v;
    string numStr = "";
    for (char tav : str)
    {
        if (tav == static_cast<char>(END_OF_DATA))
        {
            return v;
        }
        else if (tav != ';')
        {
            numStr += tav;
        }
        else
        {


            cout << "" << endl;


            v.push_back(std::stof(numStr));
            numStr = "";
        }
    }
    return v;
}



//convert a float value from a string that is positioned after a semicolon, allowing retrieval of the desired numeric value from the provided string input.
float gUi::parse_constant_value(const string& str)
{
    string fstr;
    int i = 0;
    while (str[i] != ';')
    {
        i++;
    }
    i++;
    float y_val = 0;
    while (str[i] != ';')
    {
        fstr += str[i];
        i++;
    }
    return std::stof(fstr);
}



// takes a string str, extracts the more_def str from the third character, and then converts the extracted substring into a vector of floats
vector<float> gUi::parse_more_def(const string& str)
{
   

    size_t startindex = 2; //first is constFlag second is ; third starts moredef
    string slicedStr = str.substr(startindex);
   return convertFloatStringToVector(slicedStr);

   
}
