#include <stdio.h>


#include "app.h"




// Main code
int main(int, char**)
{
    App app;
    // // Init
    app.initializeWindow();
    app.initializeUI();
    // // Running
    app.renderLoop();
    // // Close 
    app.destroyUI();
    
}
