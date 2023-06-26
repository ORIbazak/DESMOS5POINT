#pragma once
#include "DRAWER.h"
#include "imports.h"
#include "gUi.h"
#include "Graph.h"
#include "DATA.h"
#include "vectors.h"
DRAWER::DRAWER(const vector<float>& xVals, const vector<float>& yVals, const vector<bool>& definedVals, const vector<float>& moredefVals, bool isMoreDefB, float constYal, bool isConstVal) :
    windowptr(nullptr), x(xVals), y(yVals), defined(definedVals), more_defined(moredefVals), has_more_def(isMoreDefB), y_val(constYal), isConst(isConstVal) {
}


void DRAWER::drawAxis()
{
    //draws Axis:
    
    // Set the color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw the x-axis
    glBegin(GL_LINES);
    glVertex2f(X_MIN, 0.0f);
    glVertex2f(X_MAX, 0.0f);
    glEnd();

    // Draw the y-axis
    glBegin(GL_LINES);
    glVertex2f(0.0f, Y_MIN);
    glVertex2f(0.0f, Y_MAX);
    glEnd();

    // Set the color back to black
    glColor3f(0.0f, 0.0f, 0.0f);
    return;
}
void DRAWER::DrawGraph()
{
    //draws non-constant graph
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < RANGE; i++)
    {
        if (defined[i])
        {
            glBegin(GL_LINE_STRIP); // start line strip
            int j = i;
            while (j < RANGE && defined[j]) //stop line if there is not defined x_Value
            {
                if (j > 0 && has_more_def)
                {
                    
                    if (isFloatBetween(x[j - 1], x[j], more_defined))
                    {
                      
                        break;
                    }
                }
                if (Y_MIN <= y[i] && Y_MAX >= y[i])
                {
                    glVertex2f(x[j], y[j]);
                }
                j++;
            }
            glEnd();
        }
    }
}


void::DRAWER::draw_constant_function()
{
    glColor3f(1.0f, 0.0f, 0.0f); //set color to red
    glBegin(GL_LINES);
    glVertex2f(X_MIN, y_val);//draw line from x_min to x_max in y=y_val (const value)
    glVertex2f(X_MAX, y_val);
    glEnd();
}


void DRAWER::createWin(GLFWwindow* previousContext) {
  //create win that grpah is drawn on, tkaes as param first window poiintr to set back focus after this one wil lbe closed
     windowptr = glfwCreateWindow(SC_WIDTH, SC_HEIGHT, "GRAPH_WINDOW", NULL, NULL);//creates win
    if (!windowptr) {
        glfwTerminate();
        return;
    }

    bool shouldCloseWindow = false;
    glfwMakeContextCurrent(windowptr);
    glViewport(0, 0, SC_WIDTH, SC_HEIGHT);
    glLoadIdentity();
    glOrtho(X_MIN, X_MAX, Y_MIN/5 , Y_MAX/5, -1.0, 1.0); //divide by five so fucntion would have more shape and space in win

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set the display function
    while (!glfwWindowShouldClose(windowptr) && !shouldCloseWindow) {
        // Clear the color buffer to black
        glClear(GL_COLOR_BUFFER_BIT);
        drawAxis();
        if (!isConst) { // draw matching function - const or not
            DrawGraph();
        }
        else {
            draw_constant_function(); 
        }
        if (glfwWindowShouldClose(windowptr)) {
            shouldCloseWindow = true;
        }
        glfwSwapBuffers(windowptr);
        glfwPollEvents();
        if (glfwWindowShouldClose(windowptr)) {
            // Close the win2 window
            glfwSetWindowShouldClose(windowptr, GLFW_TRUE);
        }
        // Check if the window should close
    }

    glfwDestroyWindow(windowptr);
    glfwMakeContextCurrent(previousContext); //set focus to main window
    // Terminate GLFW
}




