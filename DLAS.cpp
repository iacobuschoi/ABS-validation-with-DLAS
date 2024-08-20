#include <array>
#include <algorithm>
#include <functional>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include <cstddef>
#include <cstdint>

#define TIME 500 //총 프레임 수 
#define DT 0.01 //한 프레임 시간 
#define oneSec 100 //DT 바꾸면 이거 꼭 바꿔야함  oneSec = 1/DT 
#define INF 1000000000.0
#define V 30

int minResault=1000;
int last = TIME;

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using i128 = __int128_t;
using u128 = __uint128_t;
using usize = size_t;
using f32 = float;
using f64 = double;
using f80 = long double;

template<class T, class U>
T incMod(T x, U mod) {
   x += 1;
   return x == mod ? 0 : x;
}

template<class Domain, class CoDomain, size_t LEN = 5>
std::pair<Domain, CoDomain> dlas(
   std::function<CoDomain(Domain&)> f,
   std::function<void(Domain&)> mutate,
   Domain const& initial,
   u64 maxIdleIters = -1ULL
) {
   std::array<Domain, 3> S{initial, initial, initial};
   CoDomain curF = f(S[0]);
   size_t curPos = 0;
   size_t minPos = 0;

   std::array<CoDomain, LEN> fitness;
   std::fill(fitness.begin(), fitness.end(), curF);
   CoDomain minF = curF;
   size_t k = 0;

   for (u64 idleIters = 0; idleIters < maxIdleIters; idleIters += 1) {
      CoDomain prvF = curF;

      size_t newPos = incMod(curPos, 3);
      if (newPos == minPos) newPos = incMod(newPos, 3);

      Domain& curS = S[curPos];
      Domain& newS = S[newPos];

      newS = curS;
      mutate(newS);
      CoDomain newF = f(newS);
      if (newF < minF) {
         idleIters = 0;
         minPos = newPos;
         minF = newF;
      }
      if (newF == curF || newF < *std::max_element(fitness.begin(), fitness.end())) {
         curPos = newPos;
         curF = newF;
      }

      CoDomain& fit = fitness[k];
      if (curF > fit || curF < fit && curF < prvF) {
         fit = curF;
      }
      k = incMod(k, LEN);
   }
   return { S[minPos], minF };
}

struct Car{
   int BrakeArray[TIME];
   float v[TIME];
   float w[TIME];
   int time=0;
   float r=0.5;
   float F_z=350;
   float torque=2800;
   float I=4.5;
   float Math=4*F_z;
   float dt = DT;
   
};

float faprintf(struct Car* car,float D){
   char filename[100];
   sprintf(filename,"%.2f",D);
   strcat(filename,".txt");
   float Distance = 0;
   car->time=0;
   car->v[0]=V;
   car->w[0]=car->v[0]/car->r;
   FILE*fp=fopen(filename,"w");
   FILE*bfp=fopen("brake.txt","w");
   while(1){
      float s = (car->v[car->time]-car->r*car->w[car->time])/car->v[car->time];
      float a_x = 9.8*1.2*sin(1.65*atan(12*s-atan(6*s)));
      float F_x = a_x*car->F_z;
      float dw;
      
      if(s<0) F_x*=-1;
      if(car->BrakeArray[car->time]==1){
         dw =- car->dt*(car->torque-car->r*F_x)/car->I;
      }
      else{
         dw = car->dt*(car->r*F_x)/car->I;
      }
      
      if(car->w[car->time]<=0){
         dw=0;
         car->w[car->time]=0;
         car->BrakeArray[car->time]=0;
      }
      
      car->v[car->time+1]= car->v[car->time] - a_x*car->dt;
      car->w[car->time+1]= car->w[car->time] + dw;
      Distance += car->dt*car->v[car->time+1];
      
      fprintf(fp,"%f          %d       %f           %f          %f           %f\n",((float) car->time)*DT, car->BrakeArray[car->time] ,car->v[car->time],car->w[car->time],a_x,s);
      fprintf(bfp,"%d\n",car->BrakeArray[car->time]);
      if(car->v[car->time+1]<0){
         fprintf(fp,"%f",Distance);
         fclose(fp);
         fclose(bfp);
         return -Distance;
      }
      if(car->time>=TIME-2){
         fclose(fp);
         fclose(bfp);
         return -INF;
      }
      car->time++;
   }
}

float fa(struct Car* car){
   float Distance = 0;
   car->time=0;
   car->v[0]=V;
   car->w[0]=car->v[0]/car->r;
   while(1){
      float s = (car->v[car->time]-car->r*car->w[car->time])/car->v[car->time];
      float a_x = 9.8*1.2*sin(1.65*atan(12*s-atan(6*s)));
      float F_x = a_x*car->F_z;
      float dw;
      
      if(s<0) F_x*=-1;
      if(car->BrakeArray[car->time]==1){
         dw =- car->dt*(car->torque-car->r*F_x)/car->I;
      }
      else{
         dw = car->dt*(car->r*F_x)/car->I;
      }
      
      if(car->w[car->time]<=0){
         dw=0;
         car->w[car->time]=0;
         car->BrakeArray[car->time]=0;
      }
      
      car->v[car->time+1]= car->v[car->time] - a_x*car->dt;
      car->w[car->time+1]= car->w[car->time] + dw;
      Distance += car->dt*car->v[car->time+1];
      if(car->v[car->time+1]<0){
         if(Distance<minResault){
            minResault=Distance;
            faprintf(car,Distance);
            last = car->time+5;
         }
         return -Distance;
      }
      if(car->time>=TIME-2){
         return -INF;
      }
      car->time++;
   }
}
unsigned int PRNG()
{
    static unsigned int seed = 5323;
    seed = 8253729 * seed + 2396403;
    return seed  % 32768;
}
void mutate(struct Car* car){
   //srand(time(NULL));
   int s=rand()%(last-1);
   int e = s+1+PRNG()%(last-s-1);
   printf("%d %d\n",s,e);
   for(int i=s;i<=s;i++){
      car->BrakeArray[i]+=1;
      car->BrakeArray[i]%=2;
   }
}


int main(){
   struct Car mycar;
   struct Car* car=&mycar;
   car->v[0]=V;
   car->w[0]=car->v[0]/car->r;
   FILE*bbfp=fopen("brake.txt","r");
   for(int i=0;i<TIME;i++){
      int d;
      fscanf(bbfp,"%d",&d);
      car->BrakeArray[i]=d;
   }
   fclose(bbfp);
   while(1){
   dlas<Car*,float>(fa,mutate,car,200);
   }
}
