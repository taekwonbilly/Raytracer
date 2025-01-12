//#include<printf.h>
#include "src/vector.h"
#include "src/shape.h"
#include "src/sphere.h"
#include "src/plane.h"
#include "src/light.h"
#include "src/box.h"
#include "src/disk.h"
#include "src/hyperboloid.h"
#include "src/mandel.h"
#include "src/mesh.h"
#include "src/Textures/imagetexture.h"
#include "src/Textures/functiontexture.h"
#include "src/Textures/fractalnoise.h"
#include "src/Textures/mandeltexture.h"
#include "src/Textures/colortexture.h"
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <iostream>
using namespace std;
#if defined(_MSC_VER)
#include<fcntl.h>
#include<errno.h>
#endif

unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c){
   unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*3);
   r[0] = a;
   r[1] = b;
   r[2] = c;
   return r;
}
     
const int W = 1000, H = 1000;

unsigned char* DATA = (unsigned char*)malloc(W*H*3*sizeof(unsigned char));
unsigned char get(int i, int j, int k){
   return DATA[3*(i+j*W)+k]; 
}
unsigned char* getPos(int i, int j){
   return &DATA[3*(i+j*W)]; 
}
void set(int i, int j, unsigned char r, unsigned char g, unsigned char b){
   DATA[3*(i+j*W)] = r; 
   DATA[3*(i+j*W)+1] = g; 
   DATA[3*(i+j*W)+2] = b; 
}

void refresh(Autonoma* c){
  #pragma omp parallel for shared(c) shared(DATA) schedule(dynamic)
   for(int n = 0; n<H*W; ++n) 
   { 
      Vector ra = c->camera.forward+((double)(n%W)/W-.5)*((c->camera.right))+(.5-(double)(n/W)/H)*((c->camera.up));
      calcColor(&DATA[3*n], c, Ray(c->camera.focus, ra), 0);
   }
}

void outputPPM(FILE* f){

   fprintf(f, "P6 %d %d 255 ", W, H);
   fwrite(DATA, 1, W*H * 3, f);
      /*  int i, j;
   for(j = 0; j<H; j++){
      for(i = 0; i<W; i++){
      
         fprintf(f, "%c%c%c", 255, 0, 0);
      }
   }*/

/*
   fprintf(f, "P3 %d %d 255 ", W, H);
   int i, j;
   for(j = 0; j<H; j++){
      for(i = 0; i<W; i++){
      
         fprintf(f, "%u %u %u ", get(i, j, 0), get(i, j, 1), get(i, j, 2));
      }
   }
   */
/*
      fprintf(f, "P3\n%d %d\n255\n", W, H);
   int i, j;
   for(j = 0; j<H; j++){
      for(i = 0; i<W; i++){
         fprintf(f, "%d %d %d\n", get(i, j, 0), get(i, j, 1), get(i, j, 2));
      }
   }*/
}

void outputPPM(char* file){

#if defined(_MSC_VER)
   FILE* f = fopen(file, "wb");
#else
   FILE* f = fopen(file, "w");
#endif
   outputPPM(f);
   fclose(f);
}
void output(char* file){

   char command[2000];
   FILE* f;
   /*
   sprintf(command, "%s.ppm", file);
   f = fopen(command, "w");
   outputPPM(f);
  fclose(f);*/
   
   sprintf(command, "convert ppm:- %s", file);
   printf("%s\n",command);
#if defined(_MSC_VER)
   f = popen(command, "wb");
#else

   f = popen(command, "w");
#endif
 //  FILE* f = popen("cat", "w");
 //FILE* f = fopen("ola.ppm", "wb");
   outputPPM(f);
   fclose(f);
}

int main(int argv, char** argc){

#if defined(_MSC_VER)
int err = _set_fmode(_O_BINARY);
   if (err == EINVAL)
   {
      printf( "Invalid mode.\n");
      return 1;
   }
#endif

ImageTexture* ola = new ImageTexture(2, 2);
   ola->setColor(0, 0, ola->setColor(1, 1, 0, 0, 0));
   ola->setColor(1, 0, ola->setColor(0, 1, 255, 255, 255));
   ola->reflection = .2;
   
//  ola->normalMap = new ImageTexture("images/sea.jpg");
//  ola->mapX = ola->mapY = 10;

/*ImageTexture* ola = new ImageTexture(20, 20);
for(int i = 0; i<20*20; i++){
if(i%2==0)ola->setColor(i%20, i/20, 255, 0, 255);
else ola->setColor(i%20, i/20, 255,255,0);
}*/
  // Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(1.502048-1, 0.582642, -2.499250), M_PI_2, 0, 0), new ImageTexture("images/skybox.jpg"));
//    Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(1.502048, 0.582642, -2.499250-1), M_PI_2, 0, 0), new ImageTexture("images/skybox.jpg"));
 cout << "HM" << endl << flush;
 Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(0, 2, 0), 0, 0, 0),new ImageTexture("images/skybox.jpg"));
 cout << "HM" << endl << flush;
 MAIN_DATA->addLight(new Light(Vector(0, 5, 0), getColor(255, 255, 255)));
  Texture* ft = new ColorTexture(150,150,150);
  ft->reflection = 0;
    MAIN_DATA->addShape(new Plane(Vector(0, 0, 20), ft, 0,M_PI_4, -M_PI_2, .6, .6));
   MAIN_DATA->addShape(new Plane(Vector(0, 0, 20), ft, 0,M_PI_4, M_PI_2, .6, .6));
   
   MAIN_DATA->addShape(new Plane(Vector(0, 0, 0), ola, M_PI_4, M_PI_2,  0, 1, 1));
   MAIN_DATA->addShape(new Disk(Vector(0,.01,8),RED,M_PI_4, M_PI_2,  0, 2, 2));
   MAIN_DATA->addShape(new Box(Vector(0-3/sqrt(2),1,8+3/sqrt(2)),BLUE,-M_PI_4, 0,  0, 7, 2));
   MAIN_DATA->addShape(new Box(Vector(0-3.8/sqrt(2),2.54,8+3.8/sqrt(2)),ORANGE,-M_PI_4, -M_PI_4*1.25,  0, 7, 2));
//  MAIN_DATA->addShape(new Plane(Vector(0,0,0), ola, 0, 0, -M_PI_2, 1, 1));
//  MAIN_DATA->addShape(new Plane(Vector(0,0,0), ola, 0, 0, -M_PI_2, 1, 1));
  // ImageTexture* normal = new ImageTexture("images/sea.jpg");
//Texture* alpha;= new ImageTexture("images/montreal.png");
//Texture *alpha = new FractalNoise(1., 1., 0., 0., 0.);
//alpha->maskImageA(150,150,150,0);
//Texture* alpha = GREEN;
/*Plane* b;
//alpha->ambient = 1.;
//alpha->opacity = .7;
MAIN_DATA->addShape(b = new Box(Vector(0, 0, 1),alpha, 0,0,0,1,1));

//b->normalMap = normal;
b->mapX = 1.;
b->mapY = 1.;*/
   //Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(1.6, .5, -2.6), -M_PI_2, 0, 0), new ImageTexture("images/skybox.jpg"));
   
   /*
   Vector a = Vector(2, 3, 1), b = Vector(-2, -3, -1), c = Vector(-5, 3, 4), d = Vector(4, -1, 2);
   MAIN_DATA->addShape(new Triangle(a, b, c, BLUE));
   MAIN_DATA->addShape(new Sphere(a, RED,0,0,0,.1));
   MAIN_DATA->addShape(new Sphere(b, BLUE,0,0,0,.1));
   MAIN_DATA->addShape(new Sphere(c, BLACK,0,0,0,.1));
   MAIN_DATA->addShape(new Sphere(c, GREEN,0,0,0,.1));
   */
   //Mesh* mesh = new Mesh("data/x.txt", 1586, "data/f.txt", 3168,new ColorTexture(200, 200, 200));
   //Texture* tex = new ColorTexture(200, 200, 200);
   //tex->reflection = .6;
   //Mesh* mesh = new Mesh("data/elepx.txt", 62779, "data/elepf.txt", 111748,tex);
   //Mesh* mesh = new Mesh("data/billyp.txt", 4, "data/billyv.txt", 2,new ColorTexture(200, 200, 200));
   //for(int i = 0; i<mesh->polyCount; i++)
   //MAIN_DATA->addShape(mesh);
//   Shape* mesh = new Sphere(Vector(1.502048, 0.582642, -2.499250), new ColorTexture(200, 200, 200),0,0,0, 0.249319);
  // MAIN_DATA->addShape(mesh);
   
   //importMesh(MAIN_DATA, "data/x.txt", "data/f.txt");
   //Triangle* tr = (Triangle*)MAIN_DATA->listStart->data;
   //printf("%f %f %f y:%f p:%f r:%f||\n",tr->center.x, tr->center.y, tr->center.z, tr->yaw, tr->pitch, tr->roll);
//   MAIN_DATA->addShape(new Box(Vector(1.500000, 0.666667, -2.500000), RED, M_PI_2, 0, 0, 1, 1));
  
//   printf("seg?\n");
   //Texture* te;
   //MAIN_DATA->addShape(new Sphere(Vector(.5-.5, .5-.5, 1./6.),te = BLUE, 0, 0, 0, 1./6.));
   //te->reflection = .7;
   //MAIN_DATA->addShape(new Sphere(Vector(5./6.-.5, .5-.5, .5), GREEN, 0, 0, 0, 1./6.));
   //Texture* tx = new ImageTexture("images/world.jpg");
   //tx->ambient=0.1;
   //ImageTexture* clouds = new ImageTexture("images/cloud.jpg");
   //clouds->maskImageAlpha();
   //Shape* mars = new Sphere(Vector(1./3.-.5, 2./3.-.5, 2./3.), tx, 0, 0, 0, 1./3.);
   //MAIN_DATA->addShape(mars);
   //Sphere* cl;
   //MAIN_DATA->addShape(cl = new Sphere(Vector(1./3.-.5, 2./3.-.5, 2./3.), clouds, 0, 0, 0, .34));
   //mars->normalMap = new ImageTexture("images/EarthNormalMap.png");
   //mars->mapX = mars->mapY = 1;
  // Shape* mand;
   //Texture* mp;
   
  // MAIN_DATA->addShape(mand = new Mandelbrot(Vector(0, .5, .5),mp = new MandelTexture(), 0, 0, 0, 3.5/2, 2./2, 13));
   //mp->ambient = .7;
   //MAIN_DATA->addShape(new Triangle(Vector(0,0,0),Vector(1,1,1),Vector(1,0,0), RED));
      //for(int i = 0; i<4; i++) printf("%d %d %d %d\n", ola->imageData[4*i], ola->imageData[4*i+1], ola->imageData[4*i+2], ola->imageData[4*i+3]);
  // Plane* pl = new Plane(Vector(0-.5, -.5, 0), AQUA, 0, -M_PI/2,  0, .6, .6);
  // pl->texture->reflection = .1;
  // pl->texture->opacity = .7;
  // MAIN_DATA->addShape(pl);
  // pl->normalMap = normal;
  // pl->mapX = pl->mapY = 1;
   
   int frame;
   int frameLen = 1;
   char command[200];
   printf("loaded\n");
   
//   omp_set_num_threads(1);
   for(frame = 0; frame<frameLen; frame++){
   
    //  mand->setYaw(exp((double)frame/48)); 
     // mars->setYaw(-(double)frame/24);
      //MAIN_DATA->camera.setPitch(M_PI*frame/frameLen);
   //   MAIN_DATA->camera.setYaw(2*M_PI*(.25-(double)frame/frameLen));
   //   MAIN_DATA->camera.focus.z = -2.499250-sin(2*M_PI*frame/frameLen);
   //   MAIN_DATA->camera.focus.x = 1.502048-cos(2*M_PI*frame/frameLen);
    //   pl->mapOffY = (double)frame/48;
    //   mars->setYaw((double)-frame/frameLen*M_TWO_PI);
    //   cl->setYaw((double)frame/48*M_TWO_PI);
      refresh(MAIN_DATA);
      
     
     // sprintf(command, "output/tempfile.ppm", frame);
     // outputPPM(command);
      
      
      sprintf(command, "TEWWG.png", frame);
      //system(command);
      output(command);
      
      
      printf("Done--%7d|\n", frame);
   }
//system("ffmpeg -r 24 -i output/frame%07d.png -vcodec ffv1 output/out.avi && ffmpeg -i output/out.avi -c:v libx264 -preset veryslow -qp 0 -r 24 outputA.mp4");
   
   //system("ffmpeg -r 24 -i output/frame%07d.png -vcodec ffv1 -sameq output/out.avi && ffmpeg -i output/out.avi -c:v libx264 -preset veryslow -qp 0 -r 24 outputA.mp4 && rm output/out.avi");
   
   return 0;
   
}
