#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <vector>
#include <math.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define SMALL_NUMBER    1.e-4f

#include <utilities/graphics/intersection-data.hpp>
#include <shapes/sphere-shape.hpp>

#include <physics/ray.hpp>

#include <render/camera.hpp>
#include <render/light.hpp>

#include <utilities/window/screen.hpp>

namespace racer {

    class Application{

        std::string fileName;

        Camera camera;
        Screen screen;
        std::vector<racer::Sphere> sphereDataVector;
        std::vector<Light> lightVector;
        glm::vec3 back;
        glm::vec3 ambient;
        std::string outputFileName;

        void save_image(int Width, int Height, char* fname, unsigned char* pixels) {
            FILE *fp;
            const int maxVal=255; 
            
            printf("Saving image %s: %d x %d\n", fname,Width,Height);
            fp = fopen(fname,"wb");
            if (!fp) {
                    printf("Unable to open file '%s'\n",fname);
                    return;
            }
            fprintf(fp, "P6\n");
            fprintf(fp, "%d %d\n", Width, Height);
            fprintf(fp, "%d\n", maxVal);

            for(int j = 0; j < Height; j++) {
                    fwrite(&pixels[j*Width*3], 3,Width,fp);
            }

            fclose(fp);
        }

        glm::vec3 getPixelColor(glm::vec3 origin, glm::vec3 direction, int recursionLevel){
            if(recursionLevel >= 3)
                return {0, 0, 0};
            
            glm::vec3 pixelColor = {0,0,0};
            if(recursionLevel == 0){
                pixelColor = back;
            }

            double tmin = 10000000;
            bool intersected = false;
            racer::IntersectionData nearIntersectedSphereData;
            racer::Sphere nearSphere;

            for (size_t sphere = 0; sphere < sphereDataVector.size(); sphere++)
            {
                racer::IntersectionData intersectionData = sphereDataVector[sphere].DidIntersectWithRay(racer::Ray(origin, direction));  

                if(intersectionData.intersected)
                {
                    if(intersectionData.t < tmin && intersectionData.t > 0){
                        intersected = true;
                        tmin = intersectionData.t;

                        nearIntersectedSphereData = intersectionData;

                        nearSphere = sphereDataVector[sphere];
                    }
                }

            }

            if(intersected){

                pixelColor = nearSphere.Ka * ambient * nearSphere.color;

                for (size_t light_Source = 0; light_Source < lightVector.size(); light_Source++)
                {
                    glm::vec3 L = glm::normalize( lightVector[light_Source].position - nearIntersectedSphereData.pointOfIntersection );

                    bool shadowed = false;
                    for (size_t otherSphere = 0; otherSphere < sphereDataVector.size(); otherSphere++){
                        if(sphereDataVector[otherSphere].name != nearSphere.name){

                            racer::IntersectionData shadowIntersection = sphereDataVector[otherSphere].DidIntersectWithRay(racer::Ray(nearIntersectedSphereData.pointOfIntersection, L)); 
                            
                            if(shadowIntersection.intersected && shadowIntersection.t > 1){
                                shadowed = true;
                                break;
                            }

                        }                    
                    }
                    if(shadowed)
                        continue;

                    float NdolL = std::max(0.0f,(nearIntersectedSphereData.normalToIntersection.x * L.x + nearIntersectedSphereData.normalToIntersection.y * L.y + nearIntersectedSphereData.normalToIntersection.z * L.z));
                    
                    pixelColor += nearSphere.Kd * lightVector[light_Source].color * NdolL * nearSphere.color;

                    glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);

                    glm::vec3 r = (2 * NdolL * nearIntersectedSphereData.normalToIntersection - L);

                    double RdotV = std::max(0.0f,(r.x * view.x + r.y * view.y + r.z * view.z));

                    pixelColor += lightVector[light_Source].color * nearSphere.Ks * (float)std::pow(RdotV, nearSphere.n);
                    
                }
                
                glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);
                glm::vec3 reflectedRay = -view - 2.0f * nearIntersectedSphereData.normalToIntersection * glm::dot(-view, nearIntersectedSphereData.normalToIntersection);
                glm::vec3 reflectedCalculatedRayColor = getPixelColor(nearIntersectedSphereData.pointOfIntersection, reflectedRay, recursionLevel+1);

                pixelColor += nearSphere.Kr * reflectedCalculatedRayColor;

            }

            pixelColor.r = std::min(1.0f, std::max(0.0f, pixelColor.r));
            pixelColor.g = std::min(1.0f, std::max(0.0f, pixelColor.g));
            pixelColor.b = std::min(1.0f, std::max(0.0f, pixelColor.b));
            return pixelColor;
        }

        public:
        Application(){

        }

        Application(std::string _fileName){
            fileName = _fileName;
        }

        int Run(){
            
            // TODO(Bassel): Copy to populate Function
            FILE *file = fopen(fileName.c_str(),"r");
            if (!file) {
                printf("Unable to open file '%s'\n", fileName.c_str());
                return -1;
            }

            char buffer[512];
            while(fgets(buffer, sizeof(buffer), file)) {
            
                std::string word = std::string(strtok(buffer, " "));

                if(word == "NEAR") {
                    camera.nearPLane = atof(strtok(NULL, " "));
                }
                if(word == "LEFT") {
                    camera.left = atof(strtok(NULL, " "));
                }
                if(word == "RIGHT") {
                    camera.right = atof(strtok(NULL, " "));
                }
                if(word == "BOTTOM") {
                    camera.bottom = atof(strtok(NULL, " "));
                }
                if(word == "TOP") {
                    camera.top = atof(strtok(NULL, " "));
                }
                if(word == "RES") {
                    screen.width = atoi(strtok(NULL, " "));
                    screen.height = atoi(strtok(NULL, " "));
                }
                if(word == "SPHERE") {
                    racer::Sphere sphereData;
                    
                    sphereData.name = strtok(NULL, " ");
                    
                    sphereData.position.x = atof(strtok(NULL, " "));
                    sphereData.position.y = atof(strtok(NULL, " "));
                    sphereData.position.z = atof(strtok(NULL, " "));
                    
                    sphereData.scale.x = atof(strtok(NULL, " "));
                    sphereData.scale.y = atof(strtok(NULL, " "));
                    sphereData.scale.z = atof(strtok(NULL, " "));
                    
                    sphereData.color.r = atof(strtok(NULL, " "));
                    sphereData.color.g = atof(strtok(NULL, " "));
                    sphereData.color.b = atof(strtok(NULL, " "));

                    sphereData.Ka = atof(strtok(NULL, " "));
                    sphereData.Kd = atof(strtok(NULL, " "));
                    sphereData.Ks = atof(strtok(NULL, " "));
                    sphereData.Kr = atof(strtok(NULL, " "));
                    
                    sphereData.n = atoi(strtok(NULL, " "));

                    sphereDataVector.push_back(sphereData);
                }
                if(word == "LIGHT") {
                    Light light;
                    
                    std::string name = strtok(NULL, " ");

                    light.position.x = atof(strtok(NULL, " "));
                    light.position.y = atof(strtok(NULL, " "));
                    light.position.z = atof(strtok(NULL, " "));

                    light.color.r = atof(strtok(NULL, " "));
                    light.color.g = atof(strtok(NULL, " "));
                    light.color.b = atof(strtok(NULL, " "));

                    lightVector.push_back(light);
                }
                if(word == "BACK") {            
                    back.r = atof(strtok(NULL, " "));
                    back.g = atof(strtok(NULL, " "));
                    back.b = atof(strtok(NULL, " "));
                }
                if(word == "AMBIENT") {            
                    ambient.r = atof(strtok(NULL, " "));
                    ambient.g = atof(strtok(NULL, " "));
                    ambient.b = atof(strtok(NULL, " "));
                }
                if(word == "OUTPUT") {    
                    outputFileName = strtok(NULL, " ");
                }
            }   
            
            fclose(file);

            unsigned char *pixels; 
            pixels = new unsigned char [3 * screen.width * screen.height];

            // This loop just creates a gradient for illustration purposes only. You will not use it.
            for(int i = 0; i < screen.height; i++) {
                for (int k = 0; k < screen.width; k++) {
                    
                    float u = camera.left + ((camera.right - camera.left) / (screen.width)) * (float)(k);
                    float v = camera.top + ((camera.bottom - camera.top) / (screen.height)) * (float)(i);
                    float d = -camera.nearPLane;

                    glm::vec3 direction = {u, v, d};
                    glm::vec3 pixelColor = getPixelColor({0,0,0}, direction, 0);

                    pixels[i * screen.width * 3 + k * 3] = (char)(pixelColor.r * 255);
                    pixels[i * screen.width * 3 + k * 3 + 1] = (char)(pixelColor.g * 255);
                    pixels[i * screen.width * 3 + k * 3 + 2] = (char)(pixelColor.b * 255);
                }
            }

            save_image(screen.width, screen.height, (char*)outputFileName.c_str(), pixels);

            return 0;
        }

    };

}