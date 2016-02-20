#include<stdio.h>

int main(){
  FILE* f=fopen("result.txt","r");
  float temp;
  float sum=0;
  while (!feof(f)){
    fscanf(f,"%f",&temp);
    sum+=temp;
  }
  printf("%f\n",sum/10000);
  fclose(f);
  return 0;
}
