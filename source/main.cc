/**********************************************************************************************
 * file: main.cpp
 * author: Bassel Mostafa
 * purpose: Entry point
 *********************************************************************************************/
#include <iostream>
#include "application/application.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

int main(int argc, char *argv[])
{
    try
    {
        return racer::Application().Run();
    }
    catch (const char* msg)
    {
        std::cerr << "Closed With Error Message::" << msg << std::endl;
        return -1;
    }

    return 0;
}