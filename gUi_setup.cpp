#include "imports.h"
#include "DATA.h"
#include "gUi.h"
#include "Graph.h"

gUi::gUi(SOCKET& socketConn)
//constuctor, intit with defult values with con sett to connection with server
    :
    x(RANGE, 0),
    y(RANGE, 0),
    defined(RANGE, 0),
    boundries(0, true),
    more_def(RANGE, 0),
    y_val(0),
    minMaxData({ {}, {} }),
    intersects(0, 0.0),
    intersects_calc(false),
    minmax_calc(false),
    conn(socketConn),
    request_type(RequestType::reset),
    inputField(FUNC_MAX_LENGTH),
    funcstr(""),
    boundry_calc(false),
    minmax_exist(false),
    drawCalc(false),
    show_TABLE(false),
    show_INPUT(true),
    show_another_func(false),
    has_more_def(false),
    valid_func(true),
    bytesReceived(0),
    is_constant_function(false),
     has_y_intersect(true),
    y_intersect(0),
    ongoing_process(false),

    BUF(new char[BUF_SIZE])
{
    memset(BUF, 0, BUF_SIZE);
}


gUi::~gUi()
{
    //constructor, deleter buffer
    delete[] BUF;
}


bool gUi::init()
{
    if (!glfwInit()) {//init imgui contex
        return false;
    }

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    int screenWidth = videoMode->width;
    int screenHeight = videoMode->height;

    int windowX = (screenWidth - SC_WIDTH / 2) / 2;
    int windowY = (screenHeight - SC_HEIGHT) / 2;
    //send window to place in the middle 
    windowptr = glfwCreateWindow(SC_WIDTH / 2, SC_HEIGHT, "ENTRYPOINT", NULL, NULL);
    glfwSetWindowCloseCallback(windowptr, windowCloseCallback);
    //create window and set callback func


    if (!windowptr) {
        return false;
    }

    glfwSetWindowPos(windowptr, windowX, windowY);
    //set window size and viewpoer

    glfwMakeContextCurrent(windowptr);
    glViewport(0, 0, SC_WIDTH / 2, SC_HEIGHT);
    glLoadIdentity();
    glOrtho(X_MIN / 2, X_MAX / 2, Y_MIN / 2, Y_MAX / 2, -1.0, 1.0);

    // Set the clear color to gray
    glClearColor(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);

   
    //init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(windowptr, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");




    return true; // Return true if initialization is successful
}


void gUi::run() {

    // Add this line at the start of your run function
    double lastTime = glfwGetTime();

    glfwSetWindowUserPointer(windowptr, this);

    while (!glfwWindowShouldClose(windowptr)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculate and set the DeltaTime before the NewFrame call
        double currentTime = glfwGetTime();
        ImGui::GetIO().DeltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (glfwGetKey(windowptr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            cout << "TRIGGERED" << endl;
            break;  // Exit the loop immediately
        }

        // Render  GUI elements
        renderGUI();

        // Render ImGui frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwSwapBuffers(windowptr);
        glfwPollEvents();
    }
    glfwDestroyWindow(windowptr); //if need to close, destroy window

    // Terminate GLFW
    glfwTerminate();
}










void windowCloseCallback(GLFWwindow* window)
{
    // Retrieve gUi instance
    gUi* guiInstance = static_cast<gUi*>(glfwGetWindowUserPointer(window));

    // Perform the necessary action before closing the window: alert server  to close conn
    guiInstance->request_type = RequestType::terminate;
    bool result = guiInstance->SendRequestToServer();
    guiInstance->PrintStateofRequest(result);
}