
#include "CLI.h"
#include "imports.h"
#include "DATA.h"
#include "gUi.h"
#include "Graph.h"
#include "DRAWER.h"
#include "vectors.h"


//prints if request was send succesulty according to passed boo;
void gUi::PrintStateofRequest(bool & result)

{
    //takes boolean print to client if request was successflly sent
    if (result)
    {
        cout << "***REQUEST SUCCESSFULLY SENT" << endl;
    }
    else
    {
        cout << "* ****REQUEST FAILED, CLOSING CONN" << endl;
    }
    return;

}

bool gUi::SendRequestToServer()
//send request to server with params of gUi class set in main gui function, and return bool stating if all bytes were sent
{
        //request is int represntation of enum Class RequestType with ; afterwards to seperate end
        string str = "";
        int value = static_cast<int>(request_type);
        str += std::to_string(value);

        str += ";";
        if (request_type == RequestType::newFunc)//if this new func, add funcstr after ;
        {
            str += funcstr;
        }
        
        string encryped = AESencrypt(AESkey, str);// encrypt via AES with const shared key

        int bytes_sent = send(conn, encryped.c_str(), encryped.size(), 0);


        

        if (bytes_sent == size(encryped))//check if sending was successful

        {
            return true;
        }
        else
        {
            return false;
        }
    
   

}





void gUi::renderGUI()
{

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse; //cant resize

    ImGui::SetNextWindowPos(ImVec2(100, 100)); // Set the window position

    ImGui::SetNextWindowSize(ImVec2(800, 700)); // Set the gui window size

    ImGui::Begin("GRAPH CALCULATOR", NULL, window_flags);


    // Show an input text field
    ImGui::SetNextItemWidth(ImGui::GetWindowSize().x * 0.5f);
    ImGui::InputText("enter a function", inputField.data(), inputField.size());


    // Display a Enter  button
    if (show_INPUT && ImGui::Button("Enter"))
    {
        // Copy the input text to funcstr when the button is pressed
        funcstr = inputField.data();
        funcstr = trimStr(funcstr);
        cout << funcstr;
        if (validate_equation(funcstr))//check if input truly represents function
        {
            show_TABLE = true;
            show_INPUT = false;
            show_another_func = true;
            request_type = RequestType::newFunc;

            cout << "***SEND REquest:" << static_cast<int>(request_type) << endl;
            bool result = SendRequestToServer();
            PrintStateofRequest(result);

            if (!result)
            {
                closesocket(conn);
                glfwDestroyWindow(windowptr);

                // Terminate GLFW
                glfwTerminate();
            }
            ongoing_process = true;
            valid_func = true;
            bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
            if (bytesReceived > 0)
            {
                ongoing_process = false;
            }

        }
        else {
            //not ,open popup stating that
            valid_func = false;

            ImGui::OpenPopup("Invalid Function");
        }
    }
    if (ImGui::BeginPopupModal("Invalid Function"))
    {
        ImGui::Text("Invalid function. Please try again.");

        if (ImGui::Button("OK"))
        {
            inputField.clear(); //clear old func
            inputField.resize(FUNC_MAX_LENGTH);
            ImGui::CloseCurrentPopup();
            ImGui::SetKeyboardFocusHere(-1); // Set focus back to input field
            show_INPUT = true; // Reset the flag to true

        }

        ImGui::EndPopup();

        // Clear the input field

    }

    if (show_TABLE && valid_func)
    {
        ImGui::Columns(3, "DATA", false);
    }
    if (!show_TABLE)ImGui::Text("SPECIAL CORDINATES THAT CAN BE CALCUALTED");
    float buttonHeight = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImGui::Text("Boundaries Data");

    // Handle click event for "Boundaries" header
    if (show_TABLE && valid_func && !ongoing_process && !boundry_calc)
    {

        // Set the headers for the columns
        if (ImGui::Button("Boundries", ImVec2(-1, buttonHeight)))
        {


            if (!boundry_calc)
            {
                request_type = RequestType::boundries;
                cout << "***SEND REquest:" << static_cast<int>(request_type) << endl;

                bool result = SendRequestToServer();
                ongoing_process = true;
                PrintStateofRequest(result);
                try {
                    boundries = Handle_BoundriesRESPONSE();
                    boundry_calc = true;
                }
                catch (const std::exception& e)
                {
                    cerr << "Exception caught in getting boundries: " << e.what() << '\n';



                }
                catch (...)
                {
                    cerr << "Unknown exception caught in getting boundries.\n";


                }
                ongoing_process = false;

            }





        }
    }
    //DISPLAY boundries data 
    if (boundry_calc)
    {
        ImGui::NewLine();


        if (size(boundries) > 0)
        {
            for (const auto& boundaryItem : boundries) {
                ImGui::SameLine();

                ImGui::Text("Boundary: %.2f", boundaryItem);
                ImGui::NewLine();



            }
        }
        else {
            ImGui::SameLine();

            ImGui::Text("No Boundries");
        }


    }






    ImGui::NextColumn();


    // Handle click event for "MinMax" header
    ImGui::Text("MinMax Data");

    if (show_TABLE && valid_func && !ongoing_process && !minmax_calc)
    {


        if (ImGui::Button("MinMax", ImVec2(-1, buttonHeight)))
        {
            request_type = RequestType::MinMax;
            cout << "***SEND REquest:" << static_cast<int>(request_type) << endl;
            ongoing_process = true;
            bool result = SendRequestToServer();
            PrintStateofRequest(result);

            try {
                minMaxData = HandleMinMaxRESPONSE();
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught in getting MinMAx: " << e.what() << '\n';



            }
            catch (...)
            {
                std::cerr << "Unknown exception caught in getting MinMAx.\n";


            }
            ongoing_process = false;



        }

    }

    //DISPLAY MINMAX DATA
    if (minmax_calc)
    {

        ImGui::NewLine();

        ImGui::AlignTextToFramePadding();
        if (size(minMaxData.first) > 0)
        {
            minmax_exist = true;
            for (const auto& minmaxPair : minMaxData.first) {



                ImGui::Text("Min: ( %.2f, %.2f)", minmaxPair.first, minmaxPair.second);
                ImGui::NewLine();


            }
        }
        if (size(minMaxData.second) > 0)
        {
            minmax_exist = true;
            // Iterate through the vector of pairs in minMaxData.second (maximum values)
            for (const auto& minmaxPair : minMaxData.second) {


                ImGui::Text("Max: ( %.2f, %.2f)", minmaxPair.first, minmaxPair.second);
                ImGui::NewLine();



            }
        }

        if (!minmax_exist)
        {
            ImGui::Text("NO MINMAX");

        }

    }
    ImGui::NextColumn();






    // Handle click event for "Intersects" header

    ImGui::Text("Intersects Data");

    if (show_TABLE && valid_func && !ongoing_process && !intersects_calc)
    {
        if (ImGui::Button("Intersects", ImVec2(-1, buttonHeight)))
        {

            request_type = RequestType::intersects;
            cout << "***SEND REquest:" << static_cast<int>(request_type) << endl;

            bool result = SendRequestToServer();
            PrintStateofRequest(result);



            try {
                intersects = HandleIntersectsRESPONSE();
                intersects_calc = true;
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught in getting Interesects: " << e.what() << '\n';



            }
            catch (...)
            {
                std::cerr << "Unknown exception caught in getting Interesects.\n";


            }

            ongoing_process = false;

        }
    }




    //DISPLAY INTERSECTS DATA
    ImGui::NewLine();
    if (intersects_calc)
    {

        ImGui::AlignTextToFramePadding();
        if (size(intersects) > 0)
        {
            for (const auto& intersect : intersects) {
                ImGui::SameLine();

                ImGui::Text("(%.2f,0)", intersect);
                ImGui::NewLine();


            }
        }
        if (has_y_intersect)ImGui::Text("(0,%.2f)", y_intersect);
    
    if (size(intersects) == 0 && !has_y_intersect)
    {
        ImGui::Text("NO INTERSECTS");
    }

    }





    ImGui::Columns(1); // Back to 1 column 
    ImGui::Columns(3, "DATA", false); // 3 columns again


  

    ImGui::Columns(1);

    float windowHeight = ImGui::GetWindowSize().y;
    float buttonPositionY = (windowHeight - buttonHeight) * 0.5f;
    ImGui::SetCursorPosY(buttonPositionY);



    // Add data to the table cells

    // End the columns
    ImGui::Columns(1);

    if (!ongoing_process && show_TABLE)
    {
        if (ImGui::Button("DRAW", ImVec2(-1, 0))) {
            request_type = RequestType::DRAW;
            ongoing_process = true;
            cout << "***SEND REquest:" << static_cast<int>(request_type) << endl;
            bool result = SendRequestToServer();
            PrintStateofRequest(result);
            try {
                HandleDrawRESPONSE();
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught in DRAWING: " << e.what() << '\n';

                
                
            }
            catch (...)
            {
                std::cerr << "Unknown exception caught in DRAWING.\n";

            
            }
            ongoing_process = false;

        }
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();



    if (show_another_func && !ongoing_process)
    {
        if (ImGui::Button("PRESS FOR A NEW FUNCTION", ImVec2(-1, 0))) {
            resetData();
            request_type = RequestType::reset;
            ongoing_process = true;
            bool result = SendRequestToServer();
            PrintStateofRequest(result);

            bytesReceived = recv(conn, BUF, BUF_SIZE, 0);
            if (bytesReceived > 0)
            {
                ongoing_process = false;
            }


        }
    }


    ImGui::End(); // Match with ImGui::begin


}


void gUi::resetData()
{
    x.clear();
    y.clear();
    more_def.clear();
    boundries.clear();
    defined.clear();
    x.resize(RANGE);
    y.resize(RANGE);
    defined.resize(RANGE);
    has_y_intersect = true;
    y_intersect = 0;
    y_val = 0;
    intersects.clear();
    intersects_calc = false;

    minMaxData = MinMax_Obj();
    minmax_calc = false;
    minmax_exist = false;
    drawCalc = false;

    is_constant_function = false;
    has_more_def = false;

    boundry_calc = false;
    request_type = RequestType::reset;

    show_TABLE = false;
    show_INPUT = true;
    show_another_func = false;
    valid_func = true;

    inputField.clear();
    inputField.resize(FUNC_MAX_LENGTH);


    memset(BUF, 0, BUF_SIZE);
}