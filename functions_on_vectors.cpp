
#include "imports.h"
#include "vectors.h"
#include "Graph.h"
#include "DATA.h"

#include "flags.h"






// Parses a string representation of min-max pairs and returns the corresponding pairs.
 //param input The input string to parse.
 //return A pair containing two vectors of min-max pairs.

pair<vector<pair<float, float>>, vector<pair<float, float>>> ParseMinMax(const string& input) {
    pair<vector<pair<float, float>>, vector<pair<float, float>>> minmax;

    
    size_t separatorPos = input.find(static_cast<char>(DATA_SWAP));

    // Extract the string representing the first pair
    string firstPairStr = input.substr(0, separatorPos);

    // Extract the string representing the second pair
    string secondPairStr = input.substr(separatorPos + 1);

    // Parse floats from the first pair
    size_t pairSeparatorPos = firstPairStr.find(";");
    while (pairSeparatorPos != string::npos) {
        string pairStr = firstPairStr.substr(0, pairSeparatorPos);

        size_t commaPos = pairStr.find(",");

        // Extract the min and max values as strings and convert them to floats
        float first = stof(pairStr.substr(0, commaPos));
        float second = stof(pairStr.substr(commaPos + 1));

        // Add the min-max pair to the first pair vector
        minmax.first.emplace_back(first, second);

        // Remove the parsed pair from the string and find the position of the next pair separator
        firstPairStr = firstPairStr.substr(pairSeparatorPos + 1);
        pairSeparatorPos = firstPairStr.find(";");
    }

    // Parse floats from the second pair
    pairSeparatorPos = secondPairStr.find(";");
    while (pairSeparatorPos != string::npos) {
        // Extract the string representation of a single pair
        string pairStr = secondPairStr.substr(0, pairSeparatorPos);

        // Find the position of the comma separating the min and max values
        size_t commaPos = pairStr.find(",");

        // Extract the min and max values as strings and convert them to floats
        float first = stof(pairStr.substr(0, commaPos));
        float second = stof(pairStr.substr(commaPos + 1));

        // Add the min-max pair to the second pair vector
        minmax.second.emplace_back(first, second);

        // Remove the parsed pair from the string and find the position of the next pair separator
        secondPairStr = secondPairStr.substr(pairSeparatorPos + 1);
        pairSeparatorPos = secondPairStr.find(";");
    }

    // Return the parsed min-max pairs
    return minmax;
}



bool validate_equation(string& str)
{
    if (str.empty()) // Check if the equation string is empty
        return false;

    int num_of_par = 0; // Count the number of parentheses
    // Check for invalid characters at the beginning and end of the string
    if (str[0] == PAR_CLOSE || str[str.length() - 1] == PAR_OPEN)
        return false;
    // Check for invalid operators at the beginning of the string
    if (str[0] == DIVIDE || str[0] == PLUS || str[0] == MULTIPLY || str[0] == POW || str[0] == DOT)
        return false;
    // Check for invalid operators at the end of the string
    if (str[str.length() - 1] == DIVIDE || str[str.length() - 1] == PLUS || str[str.length() - 1] == MULTIPLY || str[str.length() - 1] == POW || str[str.length() - 1] == DOT)
        return false;

    for (int i = 0; i < str.length(); i++)
    {
        // Check for invalid characters in the equation (only digits, signs, X, x, '.', '(', and ')'
        if (!ValidTav(str[i]))
            return false;

        // Check for division by zero
        if (str[i] == DIVIDE && i < (str.length() - 1) && str[i + 1] == '0')
            return false;

        if (str[i] == PAR_CLOSE)
            num_of_par++; // Increment the parentheses count for closing parentheses

        if (str[i] == PAR_OPEN)
            num_of_par--; // Decrement the parentheses count for opening parentheses

        // Check for consecutive operators
        if (i > 0 && (str[i] == PLUS || str[i] == POW || str[i] == MULTIPLY || str[i] == DIVIDE) && (str[i - 1] == PLUS || str[i - 1] == POW || str[i - 1] == MULTIPLY || str[i - 1] == DIVIDE))
            return false;

        // Check for invalid placement of parentheses
        if (i > 0 && str[i] == PAR_CLOSE && str[i - 1] == PAR_OPEN)
            return false;

        // Check for invalid placement of decimal point
        if (str[i] == DOT)
        {
            if (!isdigit(str[i - 1]) || !isdigit(str[i + 1]))
                return false;
        }
    }

    // Check if all parentheses are balanced
    if (num_of_par != 0)
        return false;

    return true; // Equation is valid
}


bool isFloatBetween(float start, float end, const vector<float>& v, size_t index) {
    // checks if any float value in the given vector v is between the specified range defined by start and end
    
    // Base case: If we have reached the end of the vector
    if (index == v.size()) {
        return false;
    }

    // Check if the current float is between start and end
    if (v[index] >= start && v[index] <= end) {
        return true;
    }

    // Recursive case: Check the next float in the vector
    return isFloatBetween(start, end, v, index + 1);
}
