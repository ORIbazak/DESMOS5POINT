#pragma once
#include "imports.h"
#include "DATA.h"
#include "Graph.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#define FUNC_MAX_LENGTH 20 +1 //one for /0 null terminator
class gUi
{
private:

    vector<float> x; // holds x_values 
    vector<float> y; //holds_y_values
    vector<float>  boundries; // holds boundries
    vector<bool>defined; // true if the value in same index in x vec is defined


    vector<float> more_def;

    vector<float> intersects; //hold intersects
    bool intersects_calc; //true if requested interscrts flag for gui to change display

    float y_val;

    MinMax_Obj minMaxData; //holds minmax
    bool minmax_calc;//true if requested interscrts flag for gui to change display
    bool minmax_exist;

    bool drawCalc;


    SOCKET conn; //hold connection to server
    string funcstr; //string that will hold functions
    bool boundry_calc; // true if requested boundries9 flag for gui to change display
    bool is_constant_function; //true if constant function, used in draw
    bool has_more_def; //bool if has more boundries (assymptotes minmax), used for drawing
    bool show_TABLE; //bool to if show table 
    bool show_INPUT; //bool to if show input
    bool show_another_func; //bool if to show another func btn
    bool valid_func; //if its ok string
    int bytesReceived = 0;//hold bytes recieved 
    vector<char> inputField; //hold imput
    char* BUF; //holds msg from client

    bool ongoing_process; // boolean is server is worknig on request, if true disable gui (disable sending another requests)

    bool has_y_intersect;
    float y_intersect;

public:
    RequestType request_type; //holds request type 

    void resetData();

    float parse_constant_value(const string& str);
    bool parse_const_flag(const string& str);
    vector<float> parse_more_def(const string& str);
    vector<float> convertFloatStringToVector(string& str);
    vector<bool> convertBoolStringToVector(string& str);
    bool SendRequestToServer();
    gUi(SOCKET& conn);
    ~gUi();
    GLFWwindow* windowptr;
    void PrintStateofRequest(bool& result);
    bool init();
    void run();
    void renderGUI();
   
    //all type of requests
    void HandleDrawRESPONSE();

    vector<float>  HandleIntersectsRESPONSE();

    MinMax_Obj HandleMinMaxRESPONSE();
    vector<float>  Handle_BoundriesRESPONSE();
    void Handle_TerminateRESPONSE();
};

inline bool gUi::parse_const_flag(const string& str)
{
    return (str[0] == '1');
}

void windowCloseCallback(GLFWwindow* window);
