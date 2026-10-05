/*
 * Linux Process Monitoring and Control System
 * --------------------------------------------
 * Features:
 * 1. Create Process
 * 2. List All Processes
 * 3. View Process Details
 * 4. Stop Process
 * 5. Continue Process
 * 6. Terminate Process
 * 7. Change Process Priority
 * 8. Refresh Dashboard
 * 9. Process Tree
 * 10. Show Action Log
 * 11. Exit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <errno.h>
#include <time.h>

#define MAX_LOGS 200
#define MAX_LOG_LENGTH 256

char actionLog[MAX_LOGS][MAX_LOG_LENGTH];
int logCount = 0;

/* ---------------------------------------------------------
   ACTION LOG
   --------------------------------------------------------- */

void addLog(const char *message)
{
    if (logCount < MAX_LOGS)
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        snprintf(actionLog[logCount],
                 MAX_LOG_LENGTH,
                 "[%02d:%02d:%02d] %s",
                 t->tm_hour,
                 t->tm_min,
                 t->tm_sec,
                 message);

        logCount++;
    }
}

/* ---------------------------------------------------------
   CLEAR SCREEN
   --------------------------------------------------------- */

void clearScreen()
{
    printf("\033[H\033[J");
}

/* ---------------------------------------------------------
   CHECK IF STRING IS NUMBER
   --------------------------------------------------------- */

int isNumber(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    while (*str)
    {
        if (!isdigit((unsigned char)*str))
            return 0;

        str++;
    }

    return 1;
}

/* ---------------------------------------------------------
   GET PROCESS NAME
   --------------------------------------------------------- */

int getProcessName(pid_t pid, char *name, size_t size)
{
    char path[64];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%d/comm", pid);

    file = fopen(path, "r");

    if (!file)
        return 0;

    if (fgets(name, size, file) == NULL)
    {
        fclose(file);
        return 0;
    }

    name[strcspn(name, "\n")] = '\0';

    fclose(file);

    return 1;
}

/* ---------------------------------------------------------
   CREATE PROCESS
   --------------------------------------------------------- */

void createProcess()
{
    char processName[100];
    char command[200];

    printf("\nEnter process name: ");
    scanf(" %99[^\n]", processName);

    printf("Enter command to run: ");
    scanf(" %199[^\n]", command);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);

        perror("exec");
        exit(EXIT_FAILURE);
    }

    printf("\n============================================\n");
    printf("           PROCESS CREATED\n");
    printf("============================================\n");

    printf("Process Name : %s\n", processName);
    printf("PID           : %d\n", pid);
    printf("Command       : %s\n", command);

    char logMessage[256];

    snprintf(logMessage,
             sizeof(logMessage),
             "Created process '%s' with PID %d",
             processName,
             pid);

    addLog(logMessage);

    printf("\nProcess started successfully.\n");
}

/* ---------------------------------------------------------
   LIST ALL PROCESSES
   --------------------------------------------------------- */

void listAllProcesses()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");

    if (!dir)
    {
        perror("opendir");
        return;
    }

    printf("\n============================================================\n");
    printf("                    ALL PROCESSES\n");
    printf("============================================================\n");

    printf("%-10s %-25s %-15s\n",
           "PID",
           "PROCESS NAME",
           "STATE");

    printf("------------------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL)
    {
        if (!isNumber(entry->d_name))
            continue;

        pid_t pid = atoi(entry->d_name);

        char path[100];
        char name[100] = "Unknown";
        char state = '?';

        snprintf(path,
                 sizeof(path),
                 "/proc/%d/status",
                 pid);

        FILE *file = fopen(path, "r");

        if (!file)
            continue;

        char line[256];

        while (fgets(line, sizeof(line), file))
        {
            if (strncmp(line, "Name:", 5) == 0)
            {
                sscanf(line, "Name:\t%99[^\n]", name);
            }
            else if (strncmp(line, "State:", 6) == 0)
            {
                sscanf(line, "State:\t%c", &state);
            }
        }

        fclose(file);

        printf("%-10d %-25s %-15c\n",
               pid,
               name,
               state);
    }

    closedir(dir);

    printf("============================================================\n");
}

/* ---------------------------------------------------------
   VIEW PROCESS DETAILS
   --------------------------------------------------------- */

void viewProcessDetails()
{
    int pid;

    printf("\nEnter PID: ");
    scanf("%d", &pid);

    char path[100];

    snprintf(path,
             sizeof(path),
             "/proc/%d/status",
             pid);

    FILE *file = fopen(path, "r");

    if (!file)
    {
        printf("\nProcess with PID %d not found.\n", pid);
        return;
    }

    char line[256];

    printf("\n============================================\n");
    printf("             PROCESS DETAILS\n");
    printf("============================================\n");

    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "Name:", 5) == 0 ||
            strncmp(line, "State:", 6) == 0 ||
            strncmp(line, "Pid:", 4) == 0 ||
            strncmp(line, "PPid:", 5) == 0 ||
            strncmp(line, "VmRSS:", 6) == 0)
        {
            printf("%s", line);
        }
    }

    fclose(file);
}

/* ---------------------------------------------------------
   STOP PROCESS
   --------------------------------------------------------- */

void stopProcess()
{
    int pid;

    printf("\nEnter PID to stop: ");
    scanf("%d", &pid);

    if (kill(pid, SIGSTOP) == 0)
    {
        printf("\nProcess %d stopped successfully.\n", pid);

        char logMessage[200];

        snprintf(logMessage,
                 sizeof(logMessage),
                 "Stopped process PID %d",
                 pid);

        addLog(logMessage);
    }
    else
    {
        perror("Unable to stop process");
    }
}

/* ---------------------------------------------------------
   CONTINUE PROCESS
   --------------------------------------------------------- */

void continueProcess()
{
    int pid;

    printf("\nEnter PID to continue: ");
    scanf("%d", &pid);

    if (kill(pid, SIGCONT) == 0)
    {
        printf("\nProcess %d continued successfully.\n", pid);

        char logMessage[200];

        snprintf(logMessage,
                 sizeof(logMessage),
                 "Continued process PID %d",
                 pid);

        addLog(logMessage);
    }
    else
    {
        perror("Unable to continue process");
    }
}

/* ---------------------------------------------------------
   TERMINATE PROCESS
   --------------------------------------------------------- */

void terminateProcess()
{
    int pid;

    printf("\nEnter PID to terminate: ");
    scanf("%d", &pid);

    if (kill(pid, SIGTERM) == 0)
    {
        printf("\nTermination signal sent to PID %d.\n", pid);

        char logMessage[200];

        snprintf(logMessage,
                 sizeof(logMessage),
                 "Terminated process PID %d",
                 pid);

        addLog(logMessage);
    }
    else
    {
        perror("Unable to terminate process");
    }
}

/* ---------------------------------------------------------
   CHANGE PROCESS PRIORITY
   --------------------------------------------------------- */

void changeProcessPriority()
{
    int pid;
    int priority;

    printf("\nEnter PID: ");
    scanf("%d", &pid);

    errno = 0;

    int oldPriority = getpriority(PRIO_PROCESS, pid);

    if (errno != 0)
    {
        perror("Unable to get process priority");
        return;
    }

    printf("Current priority (nice value): %d\n", oldPriority);

    printf("Enter new priority (-20 to 19): ");
    scanf("%d", &priority);

    if (priority < -20 || priority > 19)
    {
        printf("\nInvalid priority.\n");
        printf("Priority must be between -20 and 19.\n");
        return;
    }

    if (setpriority(PRIO_PROCESS, pid, priority) == 0)
    {
        printf("\nPriority changed successfully.\n");
        printf("New priority: %d\n", priority);

        char logMessage[200];

        snprintf(logMessage,
                 sizeof(logMessage),
                 "Changed priority of PID %d to %d",
                 pid,
                 priority);

        addLog(logMessage);
    }
    else
    {
        perror("Unable to change priority");
    }
}

/* ---------------------------------------------------------
   REFRESH DASHBOARD
   --------------------------------------------------------- */

void refreshDashboard()
{
    clearScreen();

    printf("============================================================\n");
    printf("             LINUX PROCESS DASHBOARD\n");
    printf("============================================================\n\n");

    listAllProcesses();

    printf("\nDashboard refreshed successfully.\n");
}

/* ---------------------------------------------------------
   PROCESS TREE
   --------------------------------------------------------- */

void printChildren(int parentPID, int level)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");

    if (!dir)
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!isNumber(entry->d_name))
            continue;

        int pid = atoi(entry->d_name);

        char path[100];

        snprintf(path,
                 sizeof(path),
                 "/proc/%d/status",
                 pid);

        FILE *file = fopen(path, "r");

        if (!file)
            continue;

        int ppid = -1;
        char name[100] = "Unknown";

        char line[256];

        while (fgets(line, sizeof(line), file))
        {
            if (strncmp(line, "PPid:", 5) == 0)
            {
                sscanf(line, "PPid:\t%d", &ppid);
            }
            else if (strncmp(line, "Name:", 5) == 0)
            {
                sscanf(line, "Name:\t%99[^\n]", name);
            }
        }

        fclose(file);

        if (ppid == parentPID)
        {
            for (int i = 0; i < level; i++)
                printf("    ");

            printf("|-- %s (PID: %d)\n",
                   name,
                   pid);

            printChildren(pid, level + 1);
        }
    }

    closedir(dir);
}

void processTree()
{
    printf("\n============================================================\n");
    printf("                    PROCESS TREE\n");
    printf("============================================================\n");

    printf("systemd (PID: 1)\n");

    printChildren(1, 1);
}

/* ---------------------------------------------------------
   ACTION LOG
   --------------------------------------------------------- */

void showActionLog()
{
    printf("\n============================================================\n");
    printf("                    ACTION LOG\n");
    printf("============================================================\n");

    if (logCount == 0)
    {
        printf("\nNo actions recorded yet.\n");
        return;
    }

    for (int i = 0; i < logCount; i++)
    {
        printf("%d. %s\n",
               i + 1,
               actionLog[i]);
    }

    printf("============================================================\n");
}

/* ---------------------------------------------------------
   MAIN MENU
   --------------------------------------------------------- */

void displayMenu()
{
    printf("\n============================================================\n");
    printf("        LINUX PROCESS MONITORING AND CONTROL SYSTEM\n");
    printf("============================================================\n");

    printf("1. Create Process\n");
    printf("2. List All Processes\n");
    printf("3. View Process Details\n");
    printf("4. Stop Process\n");
    printf("5. Continue Process\n");
    printf("6. Terminate Process\n");
    printf("7. Change Process Priority\n");
    printf("8. Refresh Dashboard\n");
    printf("9. Process Tree\n");
    printf("10. Show Action Log\n");
    printf("11. Exit\n");

    printf("============================================================\n");
}

/* ---------------------------------------------------------
   MAIN
   --------------------------------------------------------- */

int main()
{
    int choice;

    addLog("Application started");

    while (1)
    {
        displayMenu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                createProcess();
                break;

            case 2:
                listAllProcesses();
                break;

            case 3:
                viewProcessDetails();
                break;

            case 4:
                stopProcess();
                break;

            case 5:
                continueProcess();
                break;

            case 6:
                terminateProcess();
                break;

            case 7:
                changeProcessPriority();
                break;

            case 8:
                refreshDashboard();
                break;

            case 9:
                processTree();
                break;

            case 10:
                showActionLog();
                break;

            case 11:
                addLog("Application exited");

                printf("\nExiting application...\n");
                printf("Thank you!\n");

                return 0;

            default:
                printf("\nInvalid choice. Please enter 1-11.\n");
        }

        printf("\nPress Enter to continue...");

        getchar();
        getchar();
    }

    return 0;
}