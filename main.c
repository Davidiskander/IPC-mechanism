//
//  main.m
//  project1_class
//  IPC-mechanism
//  Created by David Iskander on 1/9/16.
//  Copyright © 2016 David Iskander. All rights reserved.
//


#include <stdio.h>     /* basic I/O routines.   */
#include <unistd.h>    /* define fork(), etc.   */
#include <sys/types.h> /* define pid_t, etc.    */
#include <sys/wait.h>  /* define wait(), etc.   */
#include <signal.h>
#include <stdlib.h>

volatile sig_atomic_t myFlag = 0;
void myHandler(int);
int child_pid;

int main()
{
    signal (SIGUSR1, myHandler);
    child_pid = fork();
    
    if (child_pid==0) {
        for ( ; ; ) {
            while(myFlag == 0)
                ;
            {
                sigset_t oldmask;
                sigset_t usr1;
                sigemptyset(&oldmask);
                sigemptyset(&usr1);
                sigaddset(&usr1, SIGUSR1);
                sigprocmask(SIG_BLOCK, &usr1, &oldmask);
                myFlag = 0;
                sigprocmask(SIG_SETMASK, &oldmask, NULL);
            }
            FILE *fp=fopen("test","a");
            fwrite("child\n",1,6,fp);
            fclose(fp);
            kill(getppid(),SIGUSR1);
        }
    }
    if (child_pid>0){
        FILE *fp;
        fp=fopen("test","a");
        fwrite("parent\n",1,7,fp);
        fclose(fp);
        
        for ( ; ; ) {
            kill(child_pid, SIGUSR1);
            //kill ()-child_pid ;
            while(myFlag == 0)
                ;
            {
                sigset_t oldmask;
                sigset_t usr1;
                sigemptyset(&oldmask);
                sigemptyset(&usr1);
                sigaddset(&usr1, SIGUSR1);
                sigprocmask(SIG_BLOCK, &usr1, &oldmask);
                myFlag = 0;
                sigprocmask(SIG_SETMASK, &oldmask, NULL);
            }
            fp=fopen("test","a");
            fwrite("parent\n",1,7,fp);
            fclose(fp);
        }
    }
    
    exit(0);
}

void myHandler(int sigNo) {
    myFlag = 1;
    //signal (SIGUSR1, myHandler);
}
