#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TASK_NAME_LENGTH 20
#define SCHED_YES 1
#define SCHED_NO 2
#define SCHED_UNKNOWN 3

typedef struct task {
    char name[TASK_NAME_LENGTH];
    int period;
    int WCET;
    int priority;
    int deadline;
    struct task* next;
} task;

/* Greate tasks. Begins as 0 */ 
task* first_task = NULL;
task* last_task = NULL;

/* Allocate a task and add it to the end of the linked list */
void add_task(char* name, int period, int WCET, int priority, int deadline)
{
    task* new_task = (task*)malloc(sizeof(task));
    if (strlen(name) < TASK_NAME_LENGTH) {
        strcpy(new_task->name, name);
    }
    else {
        printf("The task name is too long.\n");
        exit(-1); // Exit with an error code
    }

    new_task->period = period;
    new_task->WCET = WCET;
    new_task->priority = priority;
    new_task->deadline = deadline;

    new_task->next = NULL;
    if (first_task == NULL && last_task == NULL) { // If this is the first task to be added
        first_task = new_task;
        last_task = new_task;
        return;
    }
    last_task->next = new_task;
    last_task = new_task;
}

/* Remove first task in the list */
void remove_first_task()
{
    task* temp = first_task;

    if (first_task == NULL) {
        printf("The task list is empty\n");
        return;
    }
    if (first_task == last_task) { // Exactly one task in the list
        first_task = NULL;
        last_task = NULL;
    }
    else { // Set first_task to be the second task in the list
        first_task = first_task->next;
    }
    free(first_task); // Deallocate the memory for the first task
}

/* Remove all tasks by iteratively removing the first task until the list is
   empty
*/
void remove_all_tasks()
{
    while (first_task != NULL) {
        remove_first_task();
    }
}

/* This function should return the number of tasks added to the system */
int nbr_of_tasks()
{
    // UPPGIFT 2 a)
    /* What we want to do here ir to itterate through the tasks in the list and 
    count how many number of tasks there is

    First we declare a point variable since we want curr_task to have the same
    structure as task
    */
    task* curr_task = fist_task; // Begin to count the first task

    /* Declare current task */
    int number_tasks = 0; // int since want to count whole numbers. Want to start at 0.
    
    // Todo: Implement this function. You should not change anything outside this
    // function.
    /* While current_task is not 0 */
    while (curr_task != NULL) {
        number_tasks = number_tasks + 1; // Add 1 task
        curr_task = curr_task->next; // Take the next task
    }
    return number_tasks;
    
}

/* Print information about all tasks in the list
   Notice, how to iterate through all tasks in the list
*/
void print_tasks()
{
    task* curr_task = first_task;

    while (curr_task != NULL) {
        printf("Name: %s, period: %d, WCET: %d, priority %d, deadline: %d\n",
            curr_task->name, curr_task->period, curr_task->WCET,
            curr_task->priority, curr_task->deadline);
        curr_task = curr_task->next;
    }
}

/* UPPGIFT 2 c)
Test if the tasks is schedulable according to the critera by Liu and Layland.
Reference: C. L. Liu and J. Layland. Scheduling algorithms for multiprogramming
in a hard real-time environment, Journal of the ACM, 20(1), 1973, pp. 46-61.
Return:
   1: Yes, the tasks are schedulable (according to the criteria)
   2: No, the tasks are not schedulable
   3: The criteria cannot determine if the tasks are schedulable or not

Assumptions: Priorities are unique, the list of tasks contains at least one
task.
*/
int schedulable_Liu_Layland()
{
    // Todo: Implement this function
    /* Want to calculate U = sum of(execution time (C) / Period time (T)) 
    If U < N*(2^(1/N) - 1) then it is schedulable
    */
    int schedulable = SCHED_UNKNOWN;  // Pre define that the variable schedulable is unknown

    /* Start by calculate U */
    double U = 0; // Declare U as a double

    /* Now yse a while loop to calculate U */
    while (curr_task != NULL) {
        U = (double) U + (curr_task->WCET/ curr_task->period); // WCET = worst case execution time. period = period time
        curr_task = curr_task -> next; // Take the next task, The last task is N 
    }

    /* Next up is to compare U with the bound to see if it's schedulable */
    double Liu_Layland_Bound = N * (pow(2,1/N) - 1); // Bound
    if (U < Liu_Layland_Bound) {
        printf("Yes it is schedulable \n");
        int schedulable = SCHED_YES;
    }
    return schedulable;

}
/*
Test if the tasks is schedulable according to the exact response time analysisnd
Layland.
Return:
   1: Yes, the tasks are schedulable (according to the criteria)
   2: No, the tasks are not schedulable
   3: The criteria cannot determine if the tasks are schedulable or not

Assumptions: Priorities are unique, the list of tasks contains at least one
task.
*/

int schedulable_response_time_analysis() 

    /*  
        Check if we have time to execute our task
        We want to calculate the response task for all our tasks and then compare it to the 
        deadline of the current task and see if  the response time is less than the deadlien.
    */

{   // C = WCET = worst case execution time
    // I = is the interference 
    // Todo: Implement this function
    // The C library provides a ceiling function that returns the smallest integer
    // value greater than or equal to x: double ceil(double x)
    int schedulable = SCHED_YES;  
    /* 
        Want it to be yes ast the begining as a initial
        This will change later if it's not true
    */

    task* curr_task = first_task; // Assign curr_task as first one (i)
    while (curr_task != NULL) {  // Not NULL
        /* Initiate what we already know W_n and W_n_plus_one */
        double W_n = 0; 
        double W_n_plus_one = curr_task->WCET; // Initial value on the next is WCET
    
        while (W_n != W_n_plus_one){
            W_n = W_n_plus_one;
            W_n_plus_one = curr_task->WCET;

            /*  
                We want too calculate the recurance using the highest priority tasks period time
                We want to loop through our tasks and takte the one with highest prio and
                calcutate using the highest priority task
            */

            task* highest_prior_task = first_task; // Assign curr_task as first one (i)
            // Loop through highest_prior_task (j)
            while (highest_prior_task != NULL){
                if (highest_prior_task->priority > curr_task->priority){
                   W_n_plus_one  = W_n_plus_one + ceil((double)W_n / highest_prior_task->period) * highest_prior_task->WCET;
                }
                highest_prior_task = highest_prior_task->next; // Want the next task
            }
        }
        /*
            Now we have the response time W_n_plus_one (R), whish is the time it takes
            to do our necessary tasks.
            We said that it was schedulable and if deadline > R => not schedulable
        */
        if (curr_task->deadline > W_n_plus_one){
            int schedulable = SCHED_NO; 
        }
       curr_task = curr_task->next; 
    }
    return schedulable;
}

int check_schedulable(int correct_response_Liu_Layland,
    int correct_response_response_time_analysis)
{
    int nbr_of_failed_tests = 0;
    if (schedulable_Liu_Layland() != correct_response_Liu_Layland) {
        nbr_of_failed_tests++;
        printf("\n=== Schedulable test failed: Liu-Layland\n");
        print_tasks();
        printf("===\n");
    }

    if (schedulable_response_time_analysis() != correct_response_response_time_analysis) {
        nbr_of_failed_tests++;
        printf("\n=== Schedulable test failed: Response-Time Analysis\n");
        print_tasks();
        printf("===\n");
    }
    return nbr_of_failed_tests;
}

/* Do not change or remove any code in this function.
   If you like you can add additional test cases.
   TODO:
   UPPGIFT 2 b)
   To this function call two arguments are pro- vided, the first 
   is the correct result for schedulability analysis using the Liu-Layland criteria 
   and the second is the correct result for the response time analysis.
   Analyze the examples for the five test-cases and fill in the correct answer
*/
int check_tests()
{
    int nbr_of_failed_tests = 0;

    remove_all_tasks();

    // Add test
    // add_task(name, period, WCET, priority, deadline)
    add_task("T1", 25, 10, 5, 25);
    add_task("T2", 25, 8, 4, 25);
    add_task("T3", 50, 5, 3, 50);
    add_task("T4", 50, 4, 2, 50);
    add_task("T5", 100, 2, 1, 100);

    nbr_of_failed_tests += check_schedulable(SCHED_UNKNOWN, SCHED_UNKNOWN);

    remove_all_tasks();
    // Add test
    add_task("T1", 50, 12, 1, 50);
    add_task("T2", 40, 10, 2, 40);
    add_task("T3", 30, 10, 3, 50);

    nbr_of_failed_tests += check_schedulable(SCHED_UNKNOWN, SCHED_UNKNOWN);

    remove_all_tasks();
    // Add test
    add_task("T1", 80, 12, 1, 50);
    add_task("T2", 40, 10, 2, 40);
    add_task("T3", 20, 10, 3, 50);

    nbr_of_failed_tests += check_schedulable(SCHED_UNKNOWN, SCHED_UNKNOWN);

    remove_all_tasks();
    // Add test
    add_task("T1", 7, 3, 3, 7);
    add_task("T2", 12, 3, 2, 12);
    add_task("T3", 20, 5, 1, 20);

    nbr_of_failed_tests += check_schedulable(SCHED_UNKNOWN, SCHED_UNKNOWN);

    remove_all_tasks();
    // Add test
    add_task("T1", 7, 3, 3, 3);
    add_task("T2", 12, 3, 2, 6);
    add_task("T3", 20, 5, 1, 20);

    nbr_of_failed_tests += check_schedulable(SCHED_UNKNOWN, SCHED_UNKNOWN);

    return nbr_of_failed_tests;
}

int main()
{
    int nbr_of_failed_tests = check_tests();
    printf("\n=== Total number of failed tests: %d\n", nbr_of_failed_tests);
 //   fflush(stdout); // Flush print buffer before terminating
    return 0;
}
