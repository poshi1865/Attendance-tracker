#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void takeAttendance();
void displayAttendanceTable();
void viewStudentDetails(char usntofind[13]);
void createNewAttendanceFiles();
void downloadDataFromRecordsToStructure();
void updateTotalClassesFile();
void updateAttendanceFile();
void printHelp();
void printMenu();

typedef struct Student
{
    char name[17];
    char usn[12];

    int classesAttended;
}Student;

Student s[100];
int totalStrength;
int semTotalClasses;
int percNeeded;
int totalClassesHeld = 0;

int main()
{
    int ch;
    char case1choice, case3choice, case4choice, case6choice;
    char usn[13];
    char term;

    bool running = true;

    while(running)
    {
        fflush(stdin);
        system("cls");
        printMenu();

        //Input choice
        printf("Enter your choice : ");
        if(scanf("%d%c", &ch, &term) != 2 || term != '\n') goto default_case;

        switch(ch)
        {
        case 1:
            downloadDataFromRecordsToStructure();
            if(totalClassesHeld < semTotalClasses)
            {
            case_one:
             fflush(stdin);
             printf("\nTake Attendance for today (Class %d)? (Y/N) ", totalClassesHeld+1);
             scanf("%c", &case1choice);
             getchar();
             if(case1choice == 'Y') {
                    takeAttendance();
                    printf("\n\nAttendance taken, press enter to continue");
                    getchar();
             }
             else if(case1choice == 'N') break;
             else {
                    printf("Invalid, please enter again:  ");
                    goto case_one;
                }
            }
            else {
              printf("\nCannot take attendance\nEither the total classes for this semester have been completed, or\nyou don't have a record for your class yet.\n");
              printf("\nPress Enter to go back to the menu");
              getchar();
              break;
            }

        case 2:
             displayAttendanceTable();
             printf("\n\n\nPress enter to continue\n\n");
             getchar();
             break;
        case 3:
        case_three:
            printf("\nEnter student's USN: ");
            scanf("%s", usn);
            getchar();
            viewStudentDetails(usn);

            printf("\n\n\n\tPress S to search for another student or any other button\n\tto go back to the menu\n\t");
            scanf("%c", &case3choice);
            getchar();
            if(case3choice == 'S') goto case_three;
            else break;

        case 4:
        case_four:
             fflush(stdin);
             printf("\nAre you sure you want to continue? This will overwrite all previous\nattendance data, if you have any. (Y/N)  ");
             scanf("%c", &case4choice);
             getchar();
             if(case4choice == 'Y')
             {
                 createNewAttendanceFiles();
                 printf("\n\nThe names and usn have been saved, press Enter to go back to the menu\n\n");
                 getchar();
                 break;
             }
             else if(case4choice == 'N') break;
             else {
                    printf("\nInvalid, please enter again:  \n");
                    goto case_four;
             }
        case 5:
             system("cls");
             printHelp();
             break;
        case 6:
            printf("\n\nAre you sure you want to exit? (Press Y if Yes, or press any other\nbutton for No) : ");
            scanf("%c", &case6choice);
            getchar();
            if(case6choice == 'Y') exit(1);
            else break;
        default:
        default_case:
            printf("\n\n\tInvalid Input, please try again\n\n");
            break;
        }
    }
    return 0;
}

//Function to create and store initial data in name, usn, classesattended , atn% files
void createNewAttendanceFiles()
{

    FILE *percneededfile, *semtotalclassfile, *tstrengthfile, *tclassesheldfile, *namesFile, *usnFile, *atnFile; //pointers for various attendance files
    char term;

    //Input the total number of classes in the semester
    sem_total_class_input:
    printf("\nEnter total number of classes to be held this semester : ");

    if(scanf("%d%c", &semTotalClasses, &term) != 2 || term != '\n')
    {
        printf("\nInvalid input, please try again\n");
        fflush(stdin);
        goto sem_total_class_input;
    }

    //Input the total strength
    total_strength_input:
    printf("\nEnter total strength of the class: ");
    if(scanf("%d%c", &totalStrength, &term) != 2 || term != '\n')
    {
        printf("\nInvalid input, please try again\n");
        fflush(stdin);
        goto total_strength_input;
    }

    //Input the attendance % a student has to have by the end of the sem
    perc_needed_input:
    printf("\nEnter the attendance percentage a student has to have\nby the end of the sem (eg: 75): ");
    if(scanf("%d%c", &percNeeded, &term) != 2 || term != '\n')
    {
        printf("\n\nInvalid input, please try again\n\n");
        fflush(stdin);
        goto perc_needed_input;
    }

    //reset the total classes held
    totalClassesHeld = 0;

    //Creating total strength file
    tstrengthfile = fopen("TotalStrength.txt", "w");
    fprintf(tstrengthfile, "%d", totalStrength);
    fclose(tstrengthfile);

    //Creating SemesterTotalClasses file
    semtotalclassfile = fopen("SemesterTotalClasses.txt", "w");
    fprintf(semtotalclassfile, "%d", semTotalClasses);
    fclose(semtotalclassfile);

    //Creating PercentageNeeded file
    percneededfile = fopen("PercentageNeeded.txt", "w");
    fprintf(percneededfile, "%d", percNeeded);
    fclose(percneededfile);

    //Updating total classes file
    tclassesheldfile = fopen("TotalClasses.txt", "w");
    fprintf(tclassesheldfile, "%d", totalClassesHeld);
    fclose(tclassesheldfile);

    //Opening Files
    namesFile = fopen("Names.txt", "w");
    usnFile = fopen("USN.txt", "w");
    atnFile = fopen("Attendance.txt", "w");

    system("cls");
    for(int i = 0; i < totalStrength; i++)
    {
        printf("\n\nFOR ROLL NUMBER %d  \n\n", i+1);

        //Recording Name
        printf("\tEnter Name : ");
        fgets(s[i].name, 17, stdin);
        fprintf(namesFile, "%s\n", strtok(s[i].name, "\n"));
        fflush(stdin);

        //Recording USN
        printf("\tEnter USN  : ");
        fgets(s[i].usn, 12, stdin);
        fprintf(usnFile, "%s\n", strtok(s[i].usn, "\n"));
        fflush(stdin);

        //Attendance File Initialization with 0s
        fprintf(atnFile, "%d\n", 0);

    }
    //Closing Files
    fclose(atnFile);
    fclose(usnFile);
    fclose(namesFile);
}

//Function for taking attendance
void takeAttendance()
{
    downloadDataFromRecordsToStructure();
    system("cls");
    char ch;
    printf("\n\n\t\tTaking Attendance for Class no. %d\n\n", (totalClassesHeld+1));
    totalClassesHeld++;
    updateTotalClassesFile();

    for(int i = 0; i < totalStrength; i++)
    {
        fflush(stdin);
        printf("Roll no %d   %s", i+1, s[i].name); printf("          (P/A) : ");
        scanf("%c", &ch);
        getchar();

        if((ch == 'p') || (ch == 'P')) s[i].classesAttended++;
        else if(ch != 'A')
        {
            printf("\nInvalid Input! Enter again\n\n");
            i--;
            continue;
        }
        printf("\n");
    }
    updateAttendanceFile();

}

//Updating the attendance file after taking attendance
void updateAttendanceFile()
{
    FILE *atnfile;

    atnfile = fopen("Attendance.txt", "w");

    for(int i = 0; i < totalStrength; i++)
    {
        fprintf(atnfile, "%d\n", s[i].classesAttended);
    }
    fclose(atnfile);
}

//Updating the Total Classes file
void updateTotalClassesFile()
{
    FILE *tcls;

    tcls = fopen("TotalClasses.txt", "w");
    fprintf(tcls, "%d", totalClassesHeld);
    fclose(tcls);
}

//Function to put data from records to the Student Structure
void downloadDataFromRecordsToStructure()
{
    FILE *percneededfile, *semtotalclassfile, *tstrengthfile, *tclassesfile, *namefile, *usnfile, *atnfile;

    //Downloading Total Classes data from file
    tclassesfile = fopen("TotalClasses.txt", "r");
    fscanf(tclassesfile, "%d", &totalClassesHeld);
    fclose(tclassesfile);

    //Downloading Percentage Needed data from file
    percneededfile = fopen("PercentageNeeded.txt", "r");
    fscanf(percneededfile, "%d", &percNeeded);
    fclose(percneededfile);

    //Downloading Total Semester Classes from file
    semtotalclassfile = fopen("SemesterTotalClasses.txt", "r");
    fscanf(semtotalclassfile, "%d", &semTotalClasses);
    fclose(semtotalclassfile);

    //Downloading Total Strength data from file
    tstrengthfile = fopen("TotalStrength.txt", "r");
    fscanf(tstrengthfile, "%d", &totalStrength);
    fclose(tstrengthfile);

    //Opening remaining files
    namefile = fopen("Names.txt", "r");
    usnfile = fopen("USN.txt", "r");
    atnfile = fopen("Attendance.txt", "r");

    char attenstr[6];

    for(int i = 0; i < totalStrength; i++)
    {
        //NAME
        fgets(s[i].name, 30, namefile);

        //USN
        fgets(s[i].usn, 20, usnfile);

        //ATTENDANCE
        fgets(attenstr, 6, atnfile);
        sscanf(attenstr, "%d", &s[i].classesAttended);
    }
    fclose(namefile);
    fclose(usnfile);
    fclose(atnfile);
}
//Display table
void displayAttendanceTable()
{
    downloadDataFromRecordsToStructure();
    system("cls");
    printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ATTENDANCE TABLE \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n\n");
    float atnperc;
    printf("\n\n%-5s%-19s%-13s%-20s%-10s", "", "|NAME|", "|USN|", "|CLASSES ATTENDED|", "|ATTENDANCE PERCENTAGE|");

    for(int i = 0; i < totalStrength; i++)
    {
        if(totalClassesHeld == 0) atnperc = 0.0;
        else atnperc = (100.0*s[i].classesAttended/totalClassesHeld);
        printf("\n\n%-2d. %-20s%-22s%d/%-15d%.1f%%", (i+1), strtok(s[i].name, "\n"), strtok(s[i].usn, "\n"), s[i].classesAttended, totalClassesHeld, atnperc);
    }
}

void viewStudentDetails(char usntofind[13])
{
    downloadDataFromRecordsToStructure();
    system("cls");
    printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 STUDENT DETAILS \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n");
    
    float perc;
    bool found = false;

    int index = 0;

    for(int i = 0; i < totalStrength; i++)
    {
        if((strcmp((strtok(s[i].usn, "\n")), usntofind)) == 0)
        {
            index = i;
            found = true;
        }
    }

    if(found)
    {
        if(totalClassesHeld > 0)
        {
            perc = 100.0*s[index].classesAttended/totalClassesHeld;
        }
        else perc = 0.0;

    printf("\n\n\tStudent's Name : %s\n\n", strtok(s[index].name, "\n"));
    printf("\tStudent's USN  : %s\n\n", strtok(s[index].usn, "\n"));
    printf("\tClasses Attended : %d/%d\n\n", s[index].classesAttended, totalClassesHeld);
    printf("\tAttendance Percentage : %.1f%%\n\n", perc);

       double classesToAttend, totalClassesToAttend;
        int classesAttended = s[index].classesAttended;;
        int totalClassesLeft = semTotalClasses - totalClassesHeld;
        totalClassesToAttend = (percNeeded*semTotalClasses)/100.0;

        classesToAttend = totalClassesToAttend - classesAttended;

        if(classesToAttend > totalClassesLeft)
            printf("\t%s will not be able to complete %d%% attendance this\n\tsemester as he/she needs to attend %.0f classes, but there are only %d\n\tremaining.\n\n", strtok(s[index].name, "\n"),percNeeded, classesToAttend, totalClassesLeft);
        else if(classesToAttend > 0)
            printf("\t%s needs to attend %.0f more classes out of the %d remaining\n\tto get %d%% attendance by the end of the semester.\n\n", strtok(s[index].name, "\n"), classesToAttend, totalClassesLeft, percNeeded);
        else
            printf("\t%s does not need to attend any more classes for him/her\n\tto have %d%% attendance by the end of the semester.\n\n",  strtok(s[index].name, "\n"), percNeeded);
    }
    else printf("\n\n\tNo student found with that USN\n\n");
}

void printHelp()
{
    printf("\n\n************* HELP ********************\n\n");
    printf("This is a program to track attendance for a class. This can\nbe used by teachers to automate most of the work that comes with\nmonitoring attendance manually.\n");
    printf("\nYou can view your classes'attendance in the form of a table anytime,\nor you can view a particular student's attendance in detail and know\nhow many classes he/she has to attend to get his attendance back on\ntrack.\n");
    printf("\nIf you are using this program for the first time, you can press 4 in\nthe menu to create a new record for your class.\n\n");
    printf("This record will then be used for taking attendance, searching for a\nstudent through their USN, and calculating other relevant details.\n");
    printf("\nPlease note that your previous class record will be overwritten if\nyou decide to create a new record.\n");
    printf("\nAlso note that the total classes which have been held will be updated\neverytime you take attendance.");
    printf("\n\nThank you for using our program!\n\n");
    printf("********************************************************************************");
    printf("\nPress enter to continue to the menu: ");
    getchar();    
}

void printMenu()
{
    printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 ATTENDANCE TRACKER PROGRAM \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n\n");
    printf("\n\n************************************ MENU **************************************\n\n");
    printf("\t1 - Take attendance\n");
    printf("\t2 - View attendance table\n");
    printf("\t3 - View a particular student's attendance details\n");
    printf("\t4 - Create a new attendance record\n");
    printf("\t5 - View Help\n");
    printf("\t6 - Exit");
    printf("\n\n********************************************************************************\n");    
}
