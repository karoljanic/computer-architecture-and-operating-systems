#include "jobs.h"


int get_job_id_by_pid(struct JobStructsStoreStruct* jobs_store, int pid) {
    struct ProcessStruct *process;
    for(int i = 1; i <= MAX_JOBS_NUMBER; i++) {
        if(jobs_store->jobs[i] != NULL) {
            for(process = jobs_store->jobs[i]->root; process != NULL; process = process->next) {
                if(process->pid == pid) {
                    return i;
                }
            }
        }
    }

    return -1;
}

struct JobStruct* get_job_by_id(struct JobStructsStoreStruct* jobs_store, int id) {
    if(id > MAX_JOBS_NUMBER) {
        return NULL;
    }

    return jobs_store->jobs[id];
}

int get_pgid_by_job_id(struct JobStructsStoreStruct* jobs_store, int id) {
    struct JobStruct *job = get_job_by_id(jobs_store, id);
    if(job == NULL) {
        return -1;
    }

    return job->pgid;
}

int get_processes_number(struct JobStructsStoreStruct* jobs_store, int id, int filter) {
    if(id > MAX_JOBS_NUMBER || jobs_store->jobs[id] == NULL) {
        return -1;
    }

    int count = 0;
    struct ProcessStruct *process;
    for(process = jobs_store->jobs[id]->root; process != NULL; process = process->next) {
        if(filter == GET_ALL ||
            (filter == GET_DONE && process->status == STATUS_DONE) ||
            (filter == GET_NOT_DONE && process->status != STATUS_DONE)) {
            count++;
        }
    }

    return count;
}

int get_next_job_id(struct JobStructsStoreStruct* jobs_store) {
    for(int i = 1; i <= MAX_JOBS_NUMBER; i++) {
        if(jobs_store->jobs[i] == NULL) {
            return i;
        }
    }

    return -1;
}

int print_processes_of_job(struct JobStructsStoreStruct* jobs_store, int id) {
    if(id > MAX_JOBS_NUMBER || jobs_store->jobs[id] == NULL) {
        return -1;
    }

    printf("[%d]", id);

    struct ProcessStruct *process;
    for(process = jobs_store->jobs[id]->root; process != NULL; process = process->next) {
        printf(" %d", process->pid);
    }
    printf("\n");

    return 0;
}

int print_job_status(struct JobStructsStoreStruct* jobs_store, int id) {
    if(id > MAX_JOBS_NUMBER || jobs_store->jobs[id] == NULL) {
        return -1;
    }

    printf("[%d]", id);

    struct ProcessStruct *process;
    for(process = jobs_store->jobs[id]->root; process != NULL; process = process->next) {
        if(process->status == STATUS_RUNNING) {
            printf("\t%d\tRUNNING\t%s", process->pid, process->command[0]);
        }
        else if(process->status == STATUS_DONE) {
            printf("\t%d\tDONE\t%s", process->pid, process->command[0]);
        }
        else if(process->status == STATUS_SUSPENDED) {
            printf("\t%d\tSUSPENDED\t%s", process->pid, process->command[0]);
        }
        else if(process->status == STATUS_CONTINUED) {
            printf("\t%d\tCONTINUED\t%s", process->pid, process->command[0]);
        }
        else if(process->status == STATUS_TERMINATED) {
            printf("\t%d\tTERMINATED\t%s", process->pid, process->command[0]);
        }
        
        if(process->next != NULL) {
            printf(" || \n");
        } 
        else {
            printf("\n");
        }
    }

    return 0;
}

int insert_job(struct JobStructsStoreStruct* jobs_store, struct JobStruct *job) {
    int id = get_next_job_id(jobs_store);
    if(id < 0) {
        return -1;
    }

    job->id = id;
    jobs_store->jobs[id] = job;
    return id;
}

int remove_job(struct JobStructsStoreStruct* jobs_store, int id) {
    if(id > MAX_JOBS_NUMBER || jobs_store->jobs[id] == NULL) {
        return -1;
    }

    jobs_store->jobs[id] = NULL;
    return 0;
}

int is_job_completed(struct JobStructsStoreStruct* jobs_store, int id) {
    if(id > MAX_JOBS_NUMBER || jobs_store->jobs[id] == NULL) {
        return 0;
    }

    struct ProcessStruct *process;
    for(process = jobs_store->jobs[id]->root; process != NULL; process = process->next) {
        if(process->status != STATUS_DONE) {
            return 0;
        }
    }

    return 1;
}

int set_process_status(struct JobStructsStoreStruct* jobs_store, int pid, int status) {
    struct ProcessStruct *process;
    for(int i = 1; i <= MAX_JOBS_NUMBER; i++) {
        if(jobs_store->jobs[i] == NULL) {
            continue;
        }
        for(process = jobs_store->jobs[i]->root; process != NULL; process = process->next) {
            if(process->pid == pid) {
                process->status = status;
                return 0;
            }
        }
    }

    return -1;
}

int set_job_status(struct JobStructsStoreStruct* jobs_store, int id, int status) {
    if(id > MAX_JOBS_NUMBER || jobs_store->jobs[id] == NULL) {
        return -1;
    }

    struct ProcessStruct *process;
    for(process = jobs_store->jobs[id]->root; process != NULL; process = process->next) {
        if(process->status != STATUS_DONE) {
            process->status = status;
        }
    }

    return 0;
}

int wait_for_pid(struct JobStructsStoreStruct* jobs_store, int pid) {
    int status = 0;
    waitpid(pid, &status, WUNTRACED);
    if(WIFEXITED(status)) {
        set_process_status(jobs_store, pid, STATUS_DONE);
    } 
    else if(WIFSIGNALED(status)) {
        set_process_status(jobs_store, pid, STATUS_TERMINATED);
    } 
    else if(WSTOPSIG(status)) {
        status = -1;
        set_process_status(jobs_store, pid, STATUS_SUSPENDED);
    }

    return status;
}

int wait_for_job(struct JobStructsStoreStruct* jobs_store, int id) {
    if(id > MAX_JOBS_NUMBER || jobs_store->jobs[id] == NULL) {
        return -1;
    }

    int proc_count = get_processes_number(jobs_store, id, GET_NOT_DONE);
    int wait_pid = -1, wait_count = 0;
    int status = 0;
    do {
        wait_pid = waitpid(-jobs_store->jobs[id]->pgid, &status, WUNTRACED);
        wait_count++;

        if(WIFEXITED(status)) {
            set_process_status(jobs_store, wait_pid, STATUS_DONE);
        } 
        else if(WIFSIGNALED(status)) {
            set_process_status(jobs_store, wait_pid, STATUS_TERMINATED);
        } 
        else if(WSTOPSIG(status)) {
            status = -1;
            set_process_status(jobs_store, wait_pid, STATUS_SUSPENDED);
            if(wait_count == proc_count) {
                print_job_status(jobs_store, id);
            }
        }
    } 
    while (wait_count < proc_count);

    return status;
}
