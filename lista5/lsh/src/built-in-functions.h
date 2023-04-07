#ifndef BUILT_IN_FUNCTIONS_H
#define BUILT_IN_FUNCTIONS_H

#include "defines.h"
#include "jobs.h"

void run_exit();
int run_cd(char* dir);
int run_jobs(struct JobStructsStoreStruct* jobs_store);
int run_fg(struct JobStructsStoreStruct* jobs_store, int argc, char** args);
int run_bg(struct JobStructsStoreStruct* jobs_store, int argc, char** args);
int run_kill(struct JobStructsStoreStruct* jobs_store, int argc, char** args);

#endif // BUILT_IN_FUNCTIONS_H
