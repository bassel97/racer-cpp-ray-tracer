/**********************************************************************************************
 * file: main.cpp
 * author: Bassel Mostafa
 * purpose: Entry point
 * purpose: full application
 *********************************************************************************************/
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#include <glm.hpp>
#define SMALL_NUMBER    1.e-8f

// TODO(Bassel): Seperate files.


struct CameraData
{
    float near;    
    float left;    float right;
    float bottom;    float top;
};

struct ScreenRes
{
    int x;    int y;
};

struct SphereData
{
    std::string name;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 color;
    float Ka;    float Kd;    float Ks;    float Kr;
    int n;
};

struct Light
{
    std::string name;
    glm::vec3 position;
    glm::vec3 color;
};

struct IntersectionData{
    bool intersected;
    glm::vec3 pointOfIntersection;
    glm::vec3 normalToIntersection;

    double t;
};

void save_image(int Width, int Height, char* fname, unsigned char* pixels);

CameraData cameraData;
ScreenRes screenRes;
std::vector<SphereData> sphereDataVector;
std::vector<Light> lightVector;
glm::vec3 back;
glm::vec3 ambient;
std::string outputFileName;

void DidIntersect(glm::vec3 origin, glm::vec3 direction, SphereData sphere, IntersectionData* intersectionData){

    origin = (origin - sphere.position) / sphere.scale;

    intersectionData->intersected = false;

    glm::vec3 scaledDirection = direction / sphere.scale;

    float A = glm::dot(scaledDirection,scaledDirection);

    float B = - 2 * glm::dot(origin,scaledDirection);

    float C = glm::dot(origin,origin) - 1;

    float D = B * B - 4 * A * C;

    if(D < 0){
        return;
    }

    float tpos = ( -B + sqrt(D) ) / ( 2.0f * A );
    float tneg = ( -B - sqrt(D) ) / ( 2.0f * A );

    float t = -(SMALL_NUMBER + std::max(tpos,tneg));

    glm::vec3 point = origin + t * scaledDirection;

    glm::vec3 normal = point / sphere.scale;
    normal = glm::normalize(normal);

    intersectionData->intersected = true;

    intersectionData->t = t;

    intersectionData->pointOfIntersection = ( point * sphere.scale ) + sphere.position ;
    
    intersectionData->normalToIntersection = normal;
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
    IntersectionData nearIntersectedSphereData;
    SphereData nearSphere;

    for (size_t sphere = 0; sphere < sphereDataVector.size(); sphere++)
    {
        IntersectionData intersectionData;

        DidIntersect(origin, direction, sphereDataVector[sphere], &intersectionData);

        if(intersectionData.intersected)
        {
            if(intersectionData.t < tmin && intersectionData.t > 1){
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
                    IntersectionData shadowIntersection;
                    DidIntersect(nearIntersectedSphereData.pointOfIntersection, L, sphereDataVector[otherSphere], &shadowIntersection);
                    
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

            glm::vec3 corretedNormal = ( nearIntersectedSphereData.normalToIntersection ) * nearSphere.scale;

            glm::vec3 corretedPoint = ( nearIntersectedSphereData.pointOfIntersection * nearSphere.scale ) + nearSphere.position ;

            // TODO(Bassel): Correct
            glm::vec3 reflectedCalculatedRayColor = getPixelColor(corretedPoint, corretedNormal, recursionLevel+1);

            pixelColor += nearSphere.Kr * reflectedCalculatedRayColor;
        }
    }

    pixelColor.r = std::min(1.0f, std::max(0.0f, pixelColor.r));
    pixelColor.g = std::min(1.0f, std::max(0.0f, pixelColor.g));
    pixelColor.b = std::min(1.0f, std::max(0.0f, pixelColor.b));
    return pixelColor;
}

int main(int argc, char *argv[]){

    std::cout << "Test";

    std::string fileName = argv[1];

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
            cameraData.near = atof(strtok(NULL, " "));
        }
        if(word == "LEFT") {
            cameraData.left = atof(strtok(NULL, " "));
        }
        if(word == "RIGHT") {
            cameraData.right = atof(strtok(NULL, " "));
        }
        if(word == "BOTTOM") {
            cameraData.bottom = atof(strtok(NULL, " "));
        }
        if(word == "TOP") {
            cameraData.top = atof(strtok(NULL, " "));
        }
        if(word == "RES") {
            screenRes.x = atoi(strtok(NULL, " "));
            screenRes.y = atoi(strtok(NULL, " "));
        }
        if(word == "SPHERE") {
            SphereData sphereData;
            
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
    pixels = new unsigned char [3 * screenRes.x * screenRes.y];

	// This loop just creates a gradient for illustration purposes only. You will not use it.
	for(int i = 0; i < screenRes.y; i++) {
		for (int k = 0; k < screenRes.x; k++) {
            
            float u = cameraData.left + ((cameraData.right - cameraData.left) / (screenRes.x)) * (float)(k);
            float v = cameraData.top + ((cameraData.bottom - cameraData.top) / (screenRes.y)) * (float)(i);
            float d = -cameraData.near;

            glm::vec3 direction = {u, v, d};
            glm::vec3 pixelColor = getPixelColor({0,0,0}, direction, 0);

			pixels[i * screenRes.x * 3 + k * 3] = (char)(pixelColor.r * 255);
			pixels[i * screenRes.x * 3 + k * 3 + 1] = (char)(pixelColor.g * 255);
			pixels[i * screenRes.x * 3 + k * 3 + 2] = (char)(pixelColor.b * 255);
		}
	}

    save_image(screenRes.x, screenRes.y, (char*)outputFileName.c_str(), pixels);


    return 0;
}

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