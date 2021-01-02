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

#define SMALL_NUMBER    1.e-8

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
    float posX;    float posY;    float posZ;
    float sclX;    float sclY;    float sclZ;
    float r;    float g;    float b;
    float Ka;    float Kd;    float Ks;    float Kr;
    int n;
};

struct Light
{
    std::string name;
    float posX;    float posY;    float posZ;
    float Ir;    float Ig;    float Ib;
};

struct Back
{
    float r;
    float g;
    float b;
};

struct Ambient{
    float Ir;
    float Ig;
    float Ib;
};

// TODO(Bassel): Change these 2 structs to classes and add functionalities.

struct Color{
    float r;
    float g;
    float b;
};

struct Vector{
    float x;
    float y;
    float z;
};

struct IntersectionData{
    bool intersected;
    Vector pointOfIntersection;
    Vector normalToIntersection;

    double t;
};

void save_image(int Width, int Height, char* fname, unsigned char* pixels);

CameraData cameraData;
ScreenRes screenRes;
std::vector<SphereData> sphereDataVector;
std::vector<Light> lightVector;
Back back;
Ambient ambient;
std::string outputFileName;

void DidIntersect(Vector origin, Vector direction, SphereData sphere, IntersectionData* intersectionData){

    origin.x = ( origin.x - sphere.posX ) / sphere.sclX;
    origin.y = ( origin.y - sphere.posY ) / sphere.sclY;
    origin.z = ( origin.z - sphere.posZ ) / sphere.sclZ;

    intersectionData->intersected = false;

    double direcMag = 1;//std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    direction.x /= direcMag;
    direction.y /= direcMag;
    direction.z /= direcMag;

    double directionx = ( direction.x ) / sphere.sclX;
    double directiony = ( direction.y ) / sphere.sclY;
    double directionz = ( direction.z ) / sphere.sclZ;

    double A = directionx * directionx + directiony * directiony + directionz * directionz;

    double B = - 2 * (origin.x * directionx
    + origin.y * directiony +
    origin.z * directionz);

    double C = (origin.x * origin.x
    + origin.y * origin.y
    + origin.z * origin.z)
    - 1;

    double D = B * B - 4 * A * C;

    if(D < 0){
        return;
    }

    double tpos = ( -B + sqrt(D) ) / ( 2.0f * A );
    double tneg = ( -B - sqrt(D) ) / ( 2.0f * A );

    double t = -(0.000001 + std::max(tpos,tneg));

    Vector point;
    point.x = origin.x + t * directionx;
    point.y = origin.y + t * directiony;
    point.z = origin.z + t * directionz;

    Vector normal;
    normal.x = (point.x / sphere.sclX );
    normal.y = (point.y / sphere.sclY );
    normal.z = (point.z / sphere.sclZ );
    double normalMag = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= normalMag;
    normal.y /= normalMag;
    normal.z /= normalMag;

    intersectionData->intersected = true;

    intersectionData->t = t;

    intersectionData->pointOfIntersection.x = ( point.x * sphere.sclX ) + sphere.posX ;
    intersectionData->pointOfIntersection.y = ( point.y * sphere.sclY ) + sphere.posY ;
    intersectionData->pointOfIntersection.z = ( point.z * sphere.sclZ ) + sphere.posZ ;
    
    intersectionData->normalToIntersection.x = normal.x ;
    intersectionData->normalToIntersection.y = normal.y ;
    intersectionData->normalToIntersection.z = normal.z ;
}

Color getPixelColor(Vector origin, Vector direction, int recursionLevel){
    
    if(recursionLevel >= 3)
        return {0, 0, 0};
    
    Color pixelColor;
    pixelColor.r = 0;
    pixelColor.g = 0;
    pixelColor.b = 0;
    if(recursionLevel == 0){
        pixelColor.r = back.r;
        pixelColor.g = back.g;
        pixelColor.b = back.b;
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

        pixelColor.r = nearSphere.Ka * ambient.Ir * nearSphere.r;
        pixelColor.g = nearSphere.Ka * ambient.Ig * nearSphere.g;
        pixelColor.b = nearSphere.Ka * ambient.Ib * nearSphere.b;

        for (size_t light_Source = 0; light_Source < lightVector.size(); light_Source++)
        {
            Vector L;
            L.x =  lightVector[light_Source].posX - nearIntersectedSphereData.pointOfIntersection.x;
            L.y =  lightVector[light_Source].posY - nearIntersectedSphereData.pointOfIntersection.y;
            L.z =  lightVector[light_Source].posZ - nearIntersectedSphereData.pointOfIntersection.z;
            double LMag = std::sqrt(L.x * L.x + L.y * L.y + L.z * L.z);
            L.x /= LMag;
            L.y /= LMag;
            L.z /= LMag;

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

            double NdolL = std::max(0.0f,(nearIntersectedSphereData.normalToIntersection.x * L.x + nearIntersectedSphereData.normalToIntersection.y * L.y + nearIntersectedSphereData.normalToIntersection.z * L.z));
            
            pixelColor.r += nearSphere.Kd * lightVector[light_Source].Ir * NdolL * nearSphere.r;
            pixelColor.g += nearSphere.Kd * lightVector[light_Source].Ig * NdolL * nearSphere.g;
            pixelColor.b += nearSphere.Kd * lightVector[light_Source].Ib * NdolL * nearSphere.b;

            Vector view;
            view.x = origin.x - nearIntersectedSphereData.pointOfIntersection.x;
            view.y = origin.y - nearIntersectedSphereData.pointOfIntersection.y;
            view.z = origin.z - nearIntersectedSphereData.pointOfIntersection.z;
            double vMag = std::sqrt(view.x * view.x + view.y * view.y + view.z * view.z);
            view.x /= vMag;
            view.y /= vMag;
            view.z /= vMag;

            Vector r;
            r.x = 2 * NdolL * nearIntersectedSphereData.normalToIntersection.x - L.x;
            r.y = 2 * NdolL * nearIntersectedSphereData.normalToIntersection.y - L.y;
            r.z = 2 * NdolL * nearIntersectedSphereData.normalToIntersection.z - L.z;

            double RdotV = std::max(0.0f,(r.x * view.x + r.y * view.y + r.z * view.z));
            
            pixelColor.r += nearSphere.Ks * lightVector[light_Source].Ir * std::pow(RdotV, nearSphere.n);
            pixelColor.g += nearSphere.Ks * lightVector[light_Source].Ig * std::pow(RdotV, nearSphere.n);
            pixelColor.b += nearSphere.Ks * lightVector[light_Source].Ib * std::pow(RdotV, nearSphere.n);

            Vector corretedNormal;
            corretedNormal.x = ( nearIntersectedSphereData.normalToIntersection.x ) * nearSphere.sclX;
            corretedNormal.y = ( nearIntersectedSphereData.normalToIntersection.y ) * nearSphere.sclY;
            corretedNormal.z = ( nearIntersectedSphereData.normalToIntersection.z ) * nearSphere.sclZ;

            Vector corretedPoint;
            corretedPoint.x = ( nearIntersectedSphereData.pointOfIntersection.x * nearSphere.sclX ) + nearSphere.posX ;
            corretedPoint.y = ( nearIntersectedSphereData.pointOfIntersection.y * nearSphere.sclX ) + nearSphere.posY ;
            corretedPoint.z = ( nearIntersectedSphereData.pointOfIntersection.z * nearSphere.sclX ) + nearSphere.posZ ;

            // TODO(Bassel): Correct
            Color reflectedCalculatedRayColor = getPixelColor(corretedPoint, corretedNormal, recursionLevel+1);
        
            pixelColor.r += nearSphere.Kr * reflectedCalculatedRayColor.r;
            pixelColor.g += nearSphere.Kr * reflectedCalculatedRayColor.g;
            pixelColor.b += nearSphere.Kr * reflectedCalculatedRayColor.b;
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
            
            sphereData.posX = atof(strtok(NULL, " "));
            sphereData.posY = atof(strtok(NULL, " "));
            sphereData.posZ = atof(strtok(NULL, " "));
            
            sphereData.sclX = atof(strtok(NULL, " "));
            sphereData.sclY = atof(strtok(NULL, " "));
            sphereData.sclZ = atof(strtok(NULL, " "));
            
            sphereData.r = atof(strtok(NULL, " "));
            sphereData.g = atof(strtok(NULL, " "));
            sphereData.b = atof(strtok(NULL, " "));

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

            light.posX = atof(strtok(NULL, " "));
            light.posY = atof(strtok(NULL, " "));
            light.posZ = atof(strtok(NULL, " "));

            light.Ir = atof(strtok(NULL, " "));
            light.Ig = atof(strtok(NULL, " "));
            light.Ib = atof(strtok(NULL, " "));

            lightVector.push_back(light);
        }
        if(word == "BACK") {            
            back.r = atof(strtok(NULL, " "));
            back.g = atof(strtok(NULL, " "));
            back.b = atof(strtok(NULL, " "));
        }
        if(word == "AMBIENT") {            
            ambient.Ir = atof(strtok(NULL, " "));
            ambient.Ig = atof(strtok(NULL, " "));
            ambient.Ib = atof(strtok(NULL, " "));
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

            Vector direction = {u, v, d};
            Color pixelColor = getPixelColor({0,0,0}, direction, 0);

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