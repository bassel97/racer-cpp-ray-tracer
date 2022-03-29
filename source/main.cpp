/**********************************************************************************************
 * file: main.cpp
 * author: Bassel Mostafa
 * purpose: Entry point
 *********************************************************************************************/
#include <application.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char *argv[]){

    return racer::Application().Run();

}