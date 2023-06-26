
#include "imports.h"
#include "GLFW/glfw3.h"
class DRAWER {
private:

	GLFWwindow* windowptr; //holds pointer to 2ndary window (grpah windwo(
	vector<float> x; //hold x values
	vector<float> y; //hold y_values
	vector<bool>defined; // true if the value in same index in x vec is defined

	vector<float>more_defined; // holds additional found  boundries (+- asyymptoes mostly)
	bool has_more_def; //bool if function has  additional found  boundries (+- asyymptoes mostly)
	float y_val;// holds y constant value
	bool isConst;//flag for is const function
	void drawAxis();
	void DrawGraph();
	void draw_constant_function();

public:
	DRAWER(const vector<float>& xVals, const vector<float>& yVals, const vector<bool>& definedVals, const vector<float>& moredefVals, bool has_more_def, float constYal, bool isConstVal);
	void createWin(GLFWwindow* previousContext);



};

