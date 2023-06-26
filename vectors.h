#pragma once
#include "imports.h"
#include "DATA.h"
vector<float> mergeBOUNDRIES(const vector<bool>& bools, const vector<float>& floats);


vector<float> boolVecToFloatsVec(const vector<bool>& boolVec, size_t index = 0);

pair<vector<pair<float, float>>, vector<pair<float, float>>> ParseMinMax(const string& input); 



inline string trimStr(string& str)//remove spaces from input sr
{
  
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}
bool validate_equation(string& str);


bool isFloatBetween(float start, float end, const vector<float>& floatVector, size_t index = 0);
inline bool ValidTav(char& tav) 
{
    return std::find(valid_char.begin(), valid_char.end(), tav) != valid_char.end();
}


float parseYintersect(const string& str);// get y str from end of func
