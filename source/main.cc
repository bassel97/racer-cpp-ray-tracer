/**********************************************************************************************
 * file: main.cpp
 * author: Bassel Mostafa
 * purpose: Entry point
 *********************************************************************************************/
#include <windows.h>
#include <iostream>
#include "application/application.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    try
    {
        return racer::Application().Run();
    }
    catch (const char *msg)
    {
        std::cerr << "Closed With Error Message::" << msg << std::endl;
        return -1;
    }

    return 0;
}