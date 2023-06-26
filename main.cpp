
#include "imports.h"
#include "CLI.h"
#include "gUi.h"
#include "DATA.h"
#include "Graph.h"



    


int main()
{
    CLI client = CLI(); // Create an instance of the CLI class named 'client'.
    client.connectToServer(); // Call the connectToServer function of the CLI class on the 'client' instance.
    gUi GUI = client.conn; // Create an instance of the gUi class named 'GUI' and assign the value of 'client.conn' to it.
    GUI.init(); // Call the init function of the gUi class on the 'GUI' instance.
    GUI.run(); // Call the run function of the gUi class on the 'GUI' instance.
}


        