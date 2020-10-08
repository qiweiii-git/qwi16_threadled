//*****************************************************************************
//    # #              Name   : qwi16_threadled.c
//  #     #            Date   : Oct. 08, 2020
// #    #  #  #     #  Author : Qiwei Wu
//  #     #  # #  # #  Version: 1.0
//    # #  #    #   #
// This module is the GPIO control application of qwi16_threadled project.
//*****************************************************************************

//*****************************************************************************
// Headers
//*****************************************************************************
//*****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

//*****************************************************************************
// Variables
//*****************************************************************************
#define PRJ_N "qwi16_threadled"
#define H_VER 1
#define L_VER 0
#define THREAD_NUM 2

//*****************************************************************************
// Functions
//*****************************************************************************
void *GpioLedCtrl1(void *arg)
{
   int value;
   int export;
   int direction;

   // export GPIO controller
   export = open("/sys/class/gpio/export", O_WRONLY);
   if (export < 0)
   {
      printf("Cannot open GPIO controller export\n");
      exit(1);
   }

   write(export, "905", 4);
   close(export);

   printf("GPIO controller export successfully\n");

   // Modify GPIO controller direction
   direction = open("/sys/class/gpio/gpio905/direction", O_RDWR);
   if (direction < 0)
   {
      printf("Cannot open GPIO controller direction\n");
      exit(1);
   }

   write(direction, "out", 4);
   close(direction);

   printf("GPIO controller direction changed to output successfully\n");

   // Modify GPIO controller value
   value = open("/sys/class/gpio/gpio905/value", O_RDWR);
   if (value < 0)
   {
      printf("Cannot open GPIO controller value\n");
      exit(1);
   }

   // Swap GPIO controller value each 1 second
   while (1)
   {
      sleep(1);
      write(value,"1", 2);

      sleep(1);
      write(value,"0", 2);
   }
}

void *GpioLedCtrl2(void *arg)
{
   int value;
   int export;
   int direction;

   // export GPIO controller
   export = open("/sys/class/gpio/export", O_WRONLY);
   if (export < 0)
   {
      printf("Cannot open GPIO controller export\n");
      exit(1);
   }

   write(export, "904", 4);
   close(export);

   printf("GPIO controller export successfully\n");

   // Modify GPIO controller direction
   direction = open("/sys/class/gpio/gpio904/direction", O_RDWR);
   if (direction < 0)
   {
      printf("Cannot open GPIO controller direction\n");
      exit(1);
   }

   write(direction, "out", 4);
   close(direction);

   printf("GPIO controller direction changed to output successfully\n");

   // Modify GPIO controller value
   value = open("/sys/class/gpio/gpio904/value", O_RDWR);
   if (value < 0)
   {
      printf("Cannot open GPIO controller value\n");
      exit(1);
   }

   // Swap GPIO controller value each 1 second
   while (1)
   {
      sleep(2);
      write(value,"1", 2);

      sleep(2);
      write(value,"0", 2);
   }
}

int main()
{
   int i;
   int ret;
   pthread_t threadId[THREAD_NUM];

   printf("\r\n========================================\r\n");
   printf("=   Project: %s. Ver:v%d.%d   =\r\n", PRJ_N, H_VER, L_VER);
   printf("=   Date: %s, %s        =\r\n", __DATE__, __TIME__);
   printf("========================================\r\n");

   // Thread 1 for GPIO LED 1
   ret = pthread_create(&threadId[0], NULL, GpioLedCtrl1, NULL);
   if(ret != 0)
   {
      printf("Create thread 1 failed!\r\n");
      exit(-1);
   }

   // Thread 2 for GPIO LED 2
   ret = pthread_create(&threadId[1], NULL, GpioLedCtrl2, NULL);
   if(ret != 0)
   {
      printf("Create thread 2 failed!\r\n");
      exit(-1);
   }

   // Wait for all threads
   pthread_exit(NULL);

   return 0;
}
