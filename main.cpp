#include "AppBackend.h"
#include "AppUI.h"
#include <iostream>
using namespace std;

int main()
{
    // Setting up backend 
    AppBackend backend;

    backend.setSystemDate(15, 11, 2017);
    backend.loadAllData();

    //Setting up UI and backend through it 
    AppUI ui;
    ui.setBackend(&backend);

    // Starting the Application
    ui.run();

    return 0;
}