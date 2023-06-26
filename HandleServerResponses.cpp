#include "imports.h"
#include "gUi.h"
#include "DATA.h"
#include "DRAWER.h"
#include "vectors.h"
#include "flags.h"
void printMinMax(const pair<vector<pair<float, float>>, vector<pair<float, float>>>& data) //get MInmax data structue and print it, for chck inly
{
    // Print minimum values
    cout << "MINIMUM:" << endl;
    for (const auto& pair : data.first) {
        std::cout << "(" << pair.first << ", " << pair.second << ")\n";
    }

    // Print maximum values
    cout << "MAXIMUM:" << endl;
    for (const auto& pair : data.second) {
        std::cout << "(" << pair.first << ", " << pair.second << ")\n";
    }
}





void gUi::HandleDrawRESPONSE()
{
    GLFWwindow* previousContext = glfwGetCurrentContext();//save pointer to this window

    
    //check is constant functino should be drawan 
    if (!drawCalc)
    {
        drawCalc = true;
        bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
        std::string const_str(BUF);
        const_str = AESdecrypt(AESkey, const_str); //decrypt it via AES with shared constant key

        is_constant_function = parse_const_flag(const_str); //flag is (irst char in the message 

        memset(BUF, 0, BUF_SIZE);



        if (!is_constant_function)
        {
            //if not constant function , get cordinates from server and draw funcion
      

            //y
            bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
            std::string y_str(BUF);
            y_str = DecryptFunc(y_str);
            cout << "y_str" << y_str;

             y = convertFloatStringToVector(y_str);
            cout << "sizes" << y.size() << endl;

            memset(BUF, 0, BUF_SIZE);

            //x
            bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
            std::string x_str(BUF);
            x_str = DecryptFunc(x_str);
             x = convertFloatStringToVector(x_str);

            cout << "sizes" << x.size() << endl;

            //defined

            bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
            std::string defined_str(BUF);
            defined_str = DecryptFunc(defined_str);


            defined = convertBoolStringToVector(defined_str);
            cout << "sizes" << defined.size() << endl;

            if (const_str[2] == true_char)
            {

                //extra Boundries
                bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
                std::string more_def_str(BUF);
                more_def_str = DecryptFunc(more_def_str);

                more_def = convertFloatStringToVector(more_def_str);
            }


            DRAWER draw = DRAWER(x, y, defined, more_def, more_def.size() > 0, 0.0, false);//params explain in DRAWER construcor
            draw.createWin(previousContext);//draw win saving previous context so glfw know to set focus but to main window if 2ndary is closed
        }
        else {
            //if  constant function , extract constant value (sent after ; after char in the firs message)
            y_val = parse_constant_value(const_str);
            vector<float> emptyVec1;
            vector<bool> emptyVec2;
            DRAWER draw = DRAWER(emptyVec1, emptyVec1, emptyVec2, emptyVec1, false, y_val, true);//params explain in DRAWER construcor

            draw.createWin(previousContext); //draw win saving previous context so glfw know to set focus but to main window if 2ndary is closed
        }
        return;
    }
    else {
        if (is_constant_function)
        {
            vector<float> emptyVec1;
            vector<bool> emptyVec2;
            DRAWER draw = DRAWER(emptyVec1, emptyVec1, emptyVec2, emptyVec1, false, y_val, true);//params explain in DRAWER construcor

            draw.createWin(previousContext); //draw win saving previous context so glfw know to set focus but to main window if 2ndary is closed
            }
        else
        {
            DRAWER draw = DRAWER(x, y, defined, more_def, more_def.size() > 0, 0.0, false);//params explain in DRAWER construcor
            draw.createWin(previousContext);//draw win saving previous context so glfw know to set focus but to main window if 2ndary is closed
        }

    }
}

vector<float> gUi::Handle_BoundriesRESPONSE()
//get boudnries  from delimited string sent from server

{
    bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
    std::string boundry_str(BUF);
    boundry_str = AESdecrypt(AESkey, boundry_str);

    boundry_calc = true; // calc is complete , flag to imgui to erase btn and display data 
    memset(BUF, 0, BUF_SIZE);
    return convertFloatStringToVector(boundry_str);
}




vector<float> gUi::HandleIntersectsRESPONSE()
//get intersects from delimited string sent from server

{
    vector<float>empty;
    bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
    std::string i_str(BUF);
    i_str = AESdecrypt(AESkey, i_str);
    intersects_calc = true; // calc is complete , flag to imgui to erase btn and display data 
    int x_size = i_str[0] - '0';
    int y_size = i_str[2] - '0';

    memset(BUF, 0, BUF_SIZE);

    if (y_size > 0)
    {
        has_y_intersect = true;
        y_intersect = parseYintersect(i_str);
    }
    if (x_size > 0)
    {
        size_t break_from_y = i_str.find(static_cast<char>(DATA_SWAP));
        
        string X_INTERSECTS = i_str.substr(break_from_y + 1);
        return  convertFloatStringToVector(X_INTERSECTS);

    
    }
    return empty;

   
}

float parseYintersect(const string& str)// get y str from end of func
{
    string floatS;
    size_t i = 0;
    while (str[i] != static_cast<char>(Y_BOOL_2_VAL)) i = i + 1;
    i++;
    while (str[i] != static_cast<char>(DATA_SWAP))
    {
        floatS += str[i];
        i++;
    }
    return std::stof(floatS);
}

MinMax_Obj gUi ::HandleMinMaxRESPONSE() //first min ,then max
//get minmax from delimited string sent from server
{
    bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
    std::string minmax_str(BUF);
    minmax_str = AESdecrypt(AESkey,minmax_str);
    memset(BUF, 0, BUF_SIZE);

    MinMax_Obj MinMax = ParseMinMax(minmax_str); //get minmax from str
    minmax_calc = true; // calc is complete , flag to imgui to erase btn and display data 

    return MinMax;

    
}


void gUi::Handle_TerminateRESPONSE() 
//funcion called if user wants to close the window. it server confirmed (got response) close glfw window
{
    bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
    if (bytesReceived > 0)
    {
        glfwSetWindowShouldClose(windowptr, true);

    }
}