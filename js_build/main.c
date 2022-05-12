#include<stdio.h>
#include<stdlib.h>


#include "rust-bindings.h"


int main()
{
   JsonBuilder *js = jb_new_object();
   if(!js){
       printf("js is null \n");
       exit(-1);
   }

   jb_set_string(js, "timestamp", "202205122202");

   jb_set_uint(js, "sensor_id", 123456);
   
   jb_close(js);
   printf("jslen:%d\n",jb_len(js));
   printf("%s\n",jb_ptr(js));



   return 0;
}