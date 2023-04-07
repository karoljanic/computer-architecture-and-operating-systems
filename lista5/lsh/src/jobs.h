#ifndef JOBS_H
#define JOBS_H

#include "defines.h"

int get_job_id_by_pid(struct JobStructsStoreStruct* jobs_store, int pid);
struct JobStruct* get_job_by_id(struct JobStructsStoreStruct* jobs_store, int id);
int get_pgid_by_job_id(struct JobStructsStoreStruct* jobs_store, int id);
int get_processes_number(struct JobStructsStoreStruct* jobs_store, int id, int filter);
int get_next_job_id(struct JobStructsStoreStruct* jobs_store);
int print_processes_of_job(struct JobStructsStoreStruct* jobs_store, int id);
int print_job_status(struct JobStructsStoreStruct* jobs_store, int id);
int insert_job(struct JobStructsStoreStruct* jobs_store, struct JobStruct *job);
int remove_job(struct JobStructsStoreStruct* jobs_store, int id);
int is_job_completed(struct JobStructsStoreStruct* jobs_store, int id);
int set_process_status(struct JobStructsStoreStruct* jobs_store, int pid, int status);
int set_job_status(struct JobStructsStoreStruct* jobs_store, int id, int status);
int wait_for_pid(struct JobStructsStoreStruct* jobs_store, int pid);
int wait_for_job(struct JobStructsStoreStruct* jobs_store, int id);

#endif // JOBS_H
