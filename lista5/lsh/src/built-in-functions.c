#include "built-in-functions.h"

void run_exit() {
    printf(CLEAR_SCREEN);
    exit(0);
}

int run_cd(char* dir) {
    if(chdir(dir) == 0) {
        return 0;
    } 
    else {
        printf("Error executing \"cd\" :(\n");
        return 0;
    }
}

int run_jobs(struct JobStructsStoreStruct* jobs_store) {
    for(int i = 0; i < MAX_JOBS_NUMBER; i++) {
        if(jobs_store->jobs[i] != NULL) {
            print_job_status(jobs_store, i);
        }
    }

    return 0;
}

int run_fg(struct JobStructsStoreStruct* jobs_store, int argc, char** args) {
    if(argc < 2) {
        printf("Not enough arguments :(\n");
    }
    
    pid_t pid;
    int job_id = -1;

    if(strcmp(*args, "%") == 0) {
        if(argc < 3) {
            printf("Not enough arguments :(\n");
            return -1;
        }

        job_id = atoi(*(args+1));
        pid = get_pgid_by_job_id(jobs_store, job_id);
        if(pid < 0) {
            printf("Error executing \"fg\" :(\n");
            return -1;
        }
    } 
    else {
        pid = atoi(*args);
    }

    if(kill(pid, SIGCONT) < 0) {
        printf("Error executing \"fg\" :(\n");
        return -1;
    }

    tcsetpgrp(0, pid);

    if(job_id > 0) {
        set_job_status(jobs_store, job_id, STATUS_CONTINUED);
        print_job_status(jobs_store, job_id);
        if(wait_for_job(jobs_store, job_id) >= 0) {
            remove_job(jobs_store, job_id);
        }
    } 
    else {
        wait_for_pid(jobs_store, pid);
    }

    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, getpid());
    signal(SIGTTOU, SIG_DFL);

    return 0;
}

int run_bg(struct JobStructsStoreStruct* jobs_store, int argc, char** args) {
    if(argc < 2) {
        printf("Not enough arguments :(\n");
        return -1;
    }

    pid_t pid;
    int job_id = -1;

     if(strcmp(*args, "%") == 0) {
        if(argc < 3) {
            printf("Not enough arguments :(\n");
            return -1;
        }

        job_id = atoi(*(args+1));
        pid = get_pgid_by_job_id(jobs_store, job_id);
        if(pid < 0) {
            printf("Error executing \"bg\" :(\n");
            return -1;
        }
    } 
    else {
        pid = atoi(*args);
    }

    if(kill(pid, SIGCONT) < 0) {
        printf("Error executing \"bg\" :(\n");
        return -1;
    }

    if(job_id > 0) {
        set_job_status(jobs_store, job_id, STATUS_CONTINUED);
        print_job_status(jobs_store, job_id);
    }

    return 0;
}

int run_kill(struct JobStructsStoreStruct* jobs_store, int argc, char** args) {
    if(argc < 2) {
        printf("Not enough arguments :(\n");
        return -1;
    }

    pid_t pid;
    int job_id = -1;

    if(strcmp(*args, "%") == 0) {
         if(argc < 3) {
            printf("Not enough arguments :(\n");
            return -1;
        }
        job_id = atoi(*(args+1));
        pid = get_pgid_by_job_id(jobs_store, job_id);
        if(pid < 0) {
            printf("Error executing \"kill\" :(\n");
            return -1;
        }
    } 
    else {
        printf("Killing by PID is unavailable :(\n");
        return -1;
    }

    if(kill(pid, SIGTERM) < 0) {
        printf("Error executing \"kill\" :(\n");
        return 0;
    }


    if(job_id > 0) {
        set_job_status(jobs_store, job_id, STATUS_TERMINATED);
        print_job_status(jobs_store, job_id);
        if(wait_for_job(jobs_store, job_id) >= 0) {
            remove_job(jobs_store, job_id);
        }
    }

    return 1;
}
