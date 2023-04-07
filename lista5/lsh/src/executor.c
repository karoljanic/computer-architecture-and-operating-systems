#include "executor.h"

int execute_job(struct JobStructsStoreStruct* jobs_store, struct JobStruct *job) {
    fight_with_zombies(jobs_store);
    
    struct ProcessStruct *process;
    int status = 0, in_fd = 0, fd[2], job_id = -1;

    if(!job->root->built_in_command) {
        job_id = insert_job(jobs_store, job);
    }

    for(process = job->root; process != NULL; process = process->next) {
        int err_fd = 2;
        if(process->error_path != NULL) {
            err_fd = open(process->error_path, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
            if(err_fd < 0) {
                err_fd = 2;
            }
        }

        if(process == job->root && process->input_path != NULL) {
            in_fd = open(process->input_path, O_RDONLY);
            if(in_fd < 0) {
                printf("Error running new job. File \"%s\" not found :(\n", process->input_path);
                remove_job(jobs_store, job_id);
                return -1;
            }
        }
        if(process->next != NULL) {
            pipe(fd);
            status = execute_process(jobs_store, job, process, in_fd, err_fd, fd[1], 0);
            close(fd[1]);
            in_fd = fd[0];
        } 
        else {
            int out_fd = 1;
            if(process->output_path != NULL) {
                out_fd = open(process->output_path, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
                if(out_fd < 0) {
                    out_fd = 1;
                }
            }
            status = execute_process(jobs_store, job, process, in_fd, out_fd, err_fd, job->run_in_foreground);
        }
    }

    if(!job->root->built_in_command) {
        if(status >= 0 && job->run_in_foreground) {
            remove_job(jobs_store, job_id);
        } 
        else if(!job->run_in_foreground) {
            print_processes_of_job(jobs_store, job_id);
        }
    }

    return status;
}

int execute_process(struct JobStructsStoreStruct* jobs_store, struct JobStruct *job, struct ProcessStruct *process, int in_fd, int out_fd, int err_fd, int run_in_foreground) {
    process->status = STATUS_RUNNING;
    if(strcmp(process->command[0], "exit") == 0) {
        run_exit();
    }
    else if(strcmp(process->command[0], "cd") == 0) {
        if(process->command_size < 2) {
            printf("Not enough arguments :(\n");
            return -1;
        }
        else {
            return run_cd(process->command[1]);
        }
    }
    else if(strcmp(process->command[0], "jobs") == 0) {
        return run_jobs(jobs_store);
    }
    else if(strcmp(process->command[0], "fg") == 0) {
        return run_fg(jobs_store, process->command_size, process->command + 1);
    }
    else if(strcmp(process->command[0], "bg") == 0) {
        return run_bg(jobs_store, process->command_size, process->command + 1);
    }
    else if(strcmp(process->command[0], "kill") == 0) {
        return run_kill(jobs_store, process->command_size, process->command + 1);
    }

    pid_t childpid;
    int status = 0;

    childpid = fork();

    if(childpid < 0) {
        return -1;
    } 
    else if(childpid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);

        process->pid = getpid();
        if(job->pgid > 0) {
            setpgid(0, job->pgid);
        } 
        else {
            job->pgid = process->pid;
            setpgid(0, job->pgid);
        }

        if(in_fd != 0) {
            dup2(in_fd, 0);
            close(in_fd);
        }

        if(out_fd != 1) {
            dup2(out_fd, 1);
            close(out_fd);
        }

        if(err_fd != 2) {
            dup2(err_fd, 2);
            close(err_fd);
        }

        if(execvp(process->command[0], process->command) < 0) {
            printf("Error executing \"%s\" :(\n", process->command[0]);
            exit(0);
        }

        exit(0);
    } 
    else {
        process->pid = childpid;
        if(job->pgid > 0) {
            setpgid(childpid, job->pgid);
        } 
        else {
            job->pgid = process->pid;
            setpgid(childpid, job->pgid);
        }

        if(run_in_foreground) {
            tcsetpgrp(0, job->pgid);
            status = wait_for_job(jobs_store, job->id);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, getpid());
            signal(SIGTTOU, SIG_DFL);
        }
    }

    return status;
}

void fight_with_zombies(struct JobStructsStoreStruct* jobs_store) {
    int status, pid;
    while((pid = waitpid(-1, &status, WNOHANG|WUNTRACED|WCONTINUED)) > 0) {
        if(WIFEXITED(status)) {
            set_process_status(jobs_store, pid, STATUS_DONE);
        } 
        else if(WIFSTOPPED(status)) {
            set_process_status(jobs_store, pid, STATUS_SUSPENDED);
        } 
        else if(WIFCONTINUED(status)) {
            set_process_status(jobs_store, pid, STATUS_CONTINUED);
        }

        int job_id = get_job_id_by_pid(jobs_store, pid);
        if(job_id > 0 && is_job_completed(jobs_store, job_id)) {
            print_job_status(jobs_store, job_id);
            remove_job(jobs_store, job_id);
        }
    }
}
