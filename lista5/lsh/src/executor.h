#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "defines.h"
#include "built-in-functions.h"
#include "jobs.h"

int execute_job(struct JobStructsStoreStruct* jobs_store, struct JobStruct *job);
int execute_process(struct JobStructsStoreStruct* jobs_store, struct JobStruct *job, struct ProcessStruct *process, int in_fd, int out_fd,  int err_fd, int run_in_foreground);
void fight_with_zombies(struct JobStructsStoreStruct* jobs_store);

#endif // EXECUTOR_H
