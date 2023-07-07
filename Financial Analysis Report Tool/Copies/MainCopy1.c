#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <conio.h>
#include <windows.h>

#define MAX_PATH_LENGTH 260
#define MAX_FILES_PER_COLUMN 10
#define MIN_COLUMNS 3
#define MAX_FILES 100
#define MAX_COLUMN_SIZE 7


typedef struct{
char name[50];
int revenue;
int amount_sold;
int price;
int count;
int cost;
int profit;
float efficiency;
int sales_counter;
}product;

typedef struct{
int products;
int product_count;
int sum;
int difference;
int sold;
int profit;
float average_price;
float efficiency;
}overall;

//Function Prototypes
void main_menu();
void pick_year();

void title_function(char* text);
void exit_animation(char *phrase,int i);

void sales_performance_menu(char* string);
void sales_performance_day(char* string);

float efficiency_calc(product* day,int i);
float efficiency_comparison_calc(product (*day)[], int i, int j);

void printTable(product* day, int product_count);
char* product_listing(FILE* file, char** buffer);
int file_exists(const char *filename);
void clearInputBuffer();
int safe_atoi(const char* str);
void null_checker(FILE* file_pointer);
bool alloc_checker(void *ptr);
double input_verifier_num();
bool folderExists(const char *folderName);
int compareFilenames(const void* a, const void* b);

int main()
{
    main_menu();
    return 0;
}

//General Menu Functions
void main_menu(){
    int user_input;
    title_function("Financial Analytics Report Tool");
    printf("\n1. View sales performance\n");
    printf("2. Sales Comparison\n");
    printf("3. View Analytics Report\n");
    printf("4. Exit Program\n\n");
    printf("Enter number [1-4] : ");
    scanf("%d",&user_input);
    switch(user_input){
    case 1:
        system("cls");
        if(chdir("years") == 0){
            printf("Folder entered");
        }
        system("cls");
        pick_year();
        break;
    case 2:
        system("cls");
        if(chdir("years") == 0){
            printf("Folder entered");
        }
        system("cls");
        year_comparison();
        break;
    case 3:
        break;
    case 4:
        system("cls");
        exit_animation("Exiting Program.",4);
        exit(1);
        break;
    default:
        clearInputBuffer();
        system("cls");
        main_menu();
        break;
    }
}

//Sales Performance Function
void pick_year(){
    char year[6];
    title_function("Year Menu");
    printf("Note: Enter 4 to go back to Main Menu\n\n");
    printf("Available years: \n");
    PrintSubfolders(".");
    printf("\nEnter Year: ");
    scanf("%s", year);
    printf("\nNote: Enter 4 to go back to Main Menu\n\n");

    if (folderExists(year)) {
        if(chdir(year) == 0){
            system("cls");
            printf("Folder entered");
            system("cls");
            sales_performance_menu(year);
        }
        else{
            printf("Failed to enter directory");
            system("cls");
            printf("Press any key to continue...");
            _getch();
            chdir("..");
            main_menu();

        }
    }
    else if(strcmp(year,"4") == 0){
        system("cls");
        chdir("..");
        main_menu();
    }
    else {
        system("cls");
        printf("Could not find '%s' folder.\n", year);

        system("cls");
        pick_year();
    }
}
void sales_performance_menu(char* string){
    char folderName[15];
    char date[15];
    title_function("Sales Performance Menu");
    printf("Note: Enter 4 to go back to Year Menu\n");
    printf("\nAvailable months: ");
    print_month();
    printf("\nEnter month: ");
    scanf("%s", folderName);
    if (folderExists(folderName)) {
        if(chdir(folderName) == 0){
            printf("Folder entered");
            system("cls");
            strcpy(date,string);
            strcat(date,"-");
            strcat(date,folderName);
            clearInputBuffer();
            sales_performance_day(date);
        }
        else{
            printf("\nFailed to enter directory\n");
            system("cls");
            printf("Press any key to continue...");
            _getch();
            chdir("..");
            main_menu();
        }

    }
    else if(strcmp(folderName,"4") == 0){
        system("cls");
        chdir("..");
        pick_year();
    }
    else {
        system("cls");
        printf("Could not find '%s' folder.\n", folderName);
        system("cls");
        sales_performance_menu(string);
    }
}
void sales_performance_day(char* string){
    int file_finder;
    char day[3];
    char file_name[10];
    char date[20];
    int count,error;
    title_function("Pick a day");
    error = file_lister();
    if(error == NULL){
        printf("\nPress any key to go back to main menu....");
        _getch();
        system("cls");
        chdir("..");
        chdir("..");
        main_menu();
    }
    printf("\n\nEnter day in numerical format: ");
    scanf("%s",day);
    strcpy(date, string);
    strcat(date,"-");
    strcat(date,day);
    strcpy(file_name,day);
    strcat(file_name,".txt");

    file_finder = file_exists(file_name);
    switch(file_finder){
    case 0:
        printf("\n\nFile does not exist\n\n");
        printf("Press any key to continue....");
        _getch();
        system("cls");
        sales_performance_day(string);
        break;
    case 1:
        printf("\n\nFile exists!");
        break;
    default:
        printf("Default excecption occurred");
        break;
    }
    count = file_counter(file_name);
    printf("\nNumber of products: %d\n\n",count);
    file_reader(count,file_name,date,string);

}
void file_reader(int product_count,char* fileName,char* string,char* backup){
    system("cls");
    title_function("Data Extracting");
    FILE* txt_file;
    printf("Current Date: %s\n",string);
    txt_file = fopen(fileName,"r");
    int expense,input,sum = 0,difference = 0,total_sold = 0,profit_sum = 0,prompt;
    float efficiency_sum = 0;
    char* buffer;
    product* day;
    day = (product*) malloc(sizeof(product)*product_count);
    for (int i = 0; i < product_count; i++) {
        char* name = product_listing(txt_file,&buffer);
        strcpy(day[i].name,name);
        char* revenueString;
        revenueString = product_listing(txt_file,&buffer);
        day[i].revenue = safe_atoi(revenueString);
        char* priceString = product_listing(txt_file,&buffer);
        day[i].price = atof(priceString);
        char* salesString = product_listing(txt_file,&buffer);
        day[i].amount_sold = safe_atoi(salesString);
        char* costString = product_listing(txt_file,&buffer);
        day[i].cost = safe_atoi(costString);

    }
    printf("\n1. View product\n");
    printf("2. View day summary\n");
    printf("3. Back\n");
    printf("4. Exit\n\n");
    printf("Enter choice: ");
    scanf("%d",&prompt);
    switch(prompt){
    case 1:
        for(int i = 0;i >= 0;i++){
            printf("\nEnter product to view [1-%d:] ",product_count);
            input = input_verifier_num();
            if(input<=product_count){
                break;
            }
            else{
                printf("\nInput range from [1-%d] only\n\n",product_count);
            }
        }
        day[input-1].efficiency = efficiency_calc(day,input-1);
        system("cls");
        title_function(day[input-1].name);
        printf("Product revenue: %d\n",day[input-1].revenue);
        printf("Product price: %d\n",day[input-1].price);
        printf("Product sold: %d\n",day[input-1].amount_sold);
        printf("Product cost: %d\n",day[input-1].cost);
        printf("Product profit: %d\n",day[input-1].cost + day[input-1].revenue);
        printf("Cost-Efficiency: %.2f%%\n\n",day[input-1].efficiency);
        printf("Press any key to continue.....");
        _getch();
        free(day);
        free(buffer);
        fclose(txt_file);
        file_reader(product_count,fileName,string,backup);
        break;
    case 2:
        system("cls");
        title_function(string);
    for(int i = 0; i < product_count; i++){
        sum += day[i].revenue;
        difference += day[i].cost;
        total_sold += day[i].amount_sold;
        day[i].efficiency = efficiency_calc(day,i);
        efficiency_sum += day[i].efficiency;
    }
        printTable(day, product_count);
        printf("\nTotal sales: %d\n",sum);
        printf("Total cost: %d",difference);
        printf("\nToday's Profit: %d",sum + difference);
        printf("\nAverage Cost Efficiency: %.2f%%\n\n",efficiency_sum/product_count);
        printf("Press any key to continue.....");
        _getch();
        clear_extra_space();
        free(day);
        free(buffer);
        fclose(txt_file);
        file_reader(product_count,fileName,string,backup);
        break;
    case 3:
        free(buffer);
        free(day);
        fclose(txt_file);
        system("cls");
        sales_performance_day(backup);
        break;
    case 4:
        free(buffer);
        free(day);
        fclose(txt_file);
        system("cls");
        chdir("..");
        chdir("..");
        main_menu();
        break;
    default:
        free(buffer);
        free(day);
        fclose(txt_file);
        clearInputBuffer();
        file_reader(product_count,fileName,string,backup);
    }
}
//Sales Comparison
void year_comparison(){
    char year[6];
    title_function("Year Menu");
    printf("Note: Enter 4 to go back to Main Menu\n\n");
    printf("Available Years: \n");
    PrintSubfolders(".");
    printf("\nEnter Year: ");
    scanf("%s", year);

    if (folderExists(year)) {
        if(chdir(year) == 0){
            system("cls");
            printf("Folder entered");
            system("cls");
            day_or_month(year);
        }
        else{
            printf("Failed to enter directory");
        }
    }
    else if(strcmp(year,"4") == 0){
        system("cls");
        chdir("..");
        main_menu();
    }
    else {
        system("cls");
        printf("Could not find '%s' folder.\n", year);

        system("cls");
        year_comparison();
    }
}
void day_or_month(char* string){
    char date[15];
    int choice;

    title_function("Comparison Operation Mode");
    printf("\nOptions:\n");
    printf("1. Day-to-Day");
    printf("\n2. Month-to-Month");
    printf("\n3. Back");
    printf("\n4. Exit");
    printf("\n\nEnter choice: ");
    scanf("%d",&choice);
    switch(choice){
    case 1:
        system("cls");
        month_comparison_menu(string);
        break;
    case 2:
        system("cls");
        month_comparison(string);
        break;
    case 3:
        system("cls");
        chdir("..");
        year_comparison();
        break;
    case 4:
        system("cls");
        chdir("..");
        main_menu();
        break;
    default:
        break;
    }
}
void month_comparison_menu(char* string){
    char folderName[15];
    char date[15];
    title_function("Sales Comparison Menu");
    printf("Note: Enter 4 to go back to Year Menu\n\n");
    printf("Available Months:");
    print_month();
    printf("\nEnter month: ");
    scanf("%s", folderName);
    strcpy(date, string);
    strcat(date,"-");
    strcat(date,folderName);
    if (folderExists(folderName)) {
        if(chdir(folderName) == 0){
            printf("Folder entered");
            system("cls");
            clearInputBuffer();
            day_comparison_menu(date);
        }
        else{
            printf("\nFailed to enter directory\n");
            system("cls");
            printf("Press any key to continue...");
            _getch();
            chdir("..");
            main_menu();
        }

    }
    else if(strcmp(folderName,"4") == 0){
        system("cls");
        chdir("..");
        pick_year();
    }
    else {
        system("cls");
        printf("Could not find '%s' folder.\n", folderName);
        system("cls");
        month_comparison_menu(string);
    }
}
void day_comparison_menu(char* string){
    char input_a[10];
    char input_b[10];
    int a_count,b_count,file_finder;
    char day_a[10];
    char day_b[10];
    char date[25];
    char* buffer;

    title_function("Sales Comparison Menu");

    printf("\nPick two days to compare\n");
    printf("\nDay [A]: ");
    scanf("%s",input_a);
    printf("\nDay [B]: ");
    scanf("%s",input_b);

    strcpy(date, string);
    strcat(date,"- ");
    strcat(date,input_a);
    strcat(date," vs. ");
    strcat(date,input_b);

    FILE* fptr_a;
    FILE* fptr_b;
    strcpy(day_a,input_a);
    strcpy(day_b,input_b);
    strcat(day_a,".txt");
    strcat(day_b,".txt");

    file_finder = file_exists(day_a);
    switch(file_finder){
    case 0:
        printf("\n\nFile A does not exist\n");
        _getch();
        system("cls");
        main_menu();
        break;
    case 1:
        printf("\n\nFile A exists!");
        break;
    default:
        printf("Default excecption occurred");
        break;
    }
    file_finder = file_exists(day_b);
    switch(file_finder){
    case 0:
        printf("\n\nFile B does not exist\n");
        _getch();
        system("cls");
        main_menu();
        break;
    case 1:
        printf("\n\nFile B exists!");
        break;
    default:
        printf("Default excecption occurred");
        break;
    }

    a_count = file_counter(day_a);
    b_count = file_counter(day_b);

    product** day = (product**)malloc(sizeof(product*) * 2);
    day[0] = (product*)malloc(sizeof(product) * a_count);
    day[1] = (product*)malloc(sizeof(product) * b_count);

    fptr_a = fopen(day_a,"r");
    fptr_b = fopen(day_b,"r");

    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            for (int j = 0; j < a_count; j++) {
                char* name = product_listing(fptr_a, &buffer);
                strcpy(day[i][j].name, name);

                char* revenueString = product_listing(fptr_a, &buffer);
                day[i][j].revenue = safe_atoi(revenueString);
                char* priceString = product_listing(fptr_a, &buffer);
                day[i][j].price = atof(priceString);
                char* salesString = product_listing(fptr_a, &buffer);
                day[i][j].amount_sold = safe_atoi(salesString);
                char* costString = product_listing(fptr_a,&buffer);
                day[i][j].cost = safe_atoi(costString);
            }
        }
         else {
            for (int j = 0; j < b_count; j++) {
                char* name = product_listing(fptr_b, &buffer);
                strcpy(day[i][j].name, name);
                char* revenueString = product_listing(fptr_b, &buffer);
                day[i][j].revenue = safe_atoi(revenueString);
                char* priceString = product_listing(fptr_b, &buffer);
                day[i][j].price = atof(priceString);
                char* salesString = product_listing(fptr_b, &buffer);
                day[i][j].amount_sold = safe_atoi(salesString);
                char* costString = product_listing(fptr_b,&buffer);
                day[i][j].cost = safe_atoi(costString);
            }
        }
    }

    overall total[2] = {0};
    total[0].products = a_count;
    total[1].products = b_count;
    for(int i = 0; i < 2; i++){
        if(i == 0){
            for(int j = 0; j<a_count;j++){
                total[0].sum += day[i][j].revenue;
                total[0].difference += day[i][j].cost;
                total[0].sold += day[i][j].amount_sold ;
                total[0].average_price += day[i][j].price;
                total[0].efficiency += (float)(day[i][j].revenue + day[i][j].cost) / ((float)day[i][j].amount_sold*day[i][j].price)*100.0;
            }
        }
        else{
                printf("\nProduct B:\n");
            for(int j = 0; j<b_count;j++){
                total[1].sum += day[i][j].revenue;
                total[1].difference += day[i][j].cost;
                total[1].sold += day[i][j].amount_sold ;
                total[1].average_price += day[i][j].price;
                total[1].efficiency += (float)(day[i][j].revenue + day[i][j].cost) / ((float)day[i][j].amount_sold*day[i][j].price)*100.0;
            }
        }
    }
    total[0].profit = total[0].sum + total[0].difference;
    total[1].profit = total[1].sum + total[1].difference;

    system("cls");
    title_function(date);
    printf("\n");
    print_comparison(total);
    printf("\n");
    for (int i = 0; i < 2; i++) {
    free(day[i]);
}
    free(day);
    free(buffer);
    fclose(fptr_a);
    fclose(fptr_b);
    printf("\n\nPress anywhere to continue...");
    _getch();
    system("cls");
    chdir("..");
    chdir("..");
    main_menu();
    }
void month_comparison(char*string){
    char input[10],input1[10];
    int file_count[2];
    overall total[2];
    char title[20];
    memset(total, 0, sizeof(overall) * 2);

    title_function("Month Comparison Menu");
    printf("\nAvailable Months: ");
    print_month();
    printf("\nMonth [A]: ");
    scanf("%s",input);
    printf("Month [B]: ");
    scanf("%s",input1);
    if(strcasecmp(input,input1) == 0 ){
        printf("\n\nYou have input the same month\n");
        printf("Press any key to continue...");
        _getch();
        system("cls");
        month_comparison(string);
        return;
    }
    chdir(input);
    file_count[0] = day_counter(".");
    chdir("..");
    chdir(input1);
    file_count[1] = day_counter(".");
    chdir("..");

    chdir(input);
    open_files(total,file_count[0],0);
    chdir("..");
    chdir(input1);
    open_files(total,file_count[1],1);
    chdir("..");

    system("cls");
    sprintf(title,"%s vs. %s Comparison",input,input1);
    title_function(title);
    calculateEfficiency(total, 2);
    calculate_profit(total);
    printOverall(total);
    printf("\n\nPress any key to continue...");
    _getch();
    chdir("..");
    system("cls");
    main_menu();
}

//Trend Analytics


//Calculation Functions
void calculateEfficiency(overall* data, int size) {
    for (int i = 0; i < size; i++) {
        data[i].efficiency = (data[i].sum + data[i].difference) / (data[i].sold * data[i].average_price)*10000;
    }
}
void calculate_profit(overall* all) {
    for (int i = 0; i < 2; i++) {
        overall* total = &all[i];
        total->profit = total->sum + total->difference;
    }
}
float efficiency_calc(product* day, int i) {
    float efficiency;
    efficiency = (float)(day[i].revenue - (day[i].cost*-1)) / ((float)day[i].amount_sold * day[i].price) * 100.0;
    return efficiency;
}
void modifyAveragePrice(overall data[2]) {
    for (int i = 0; i < 2; i++) {
        if (data[i].product_count != 0) {
            data[i].average_price /= data[i].product_count;
        }
    }
}
//Reusable Functions
void printOverall(const overall data[2]) {
    printf("%-20s%-20s%-20s\n", "Summary", "Product 1", "Product 2");
    printf("%-20s%-20s%-20s\n", "----------", "----------", "----------");
    printf("%-20s%-20d%-20d\n", "Total Earned:", data[0].sum, data[1].sum);
    printf("%-20s%-20.2f%-20.2f\n", "Total Price:", data[0].average_price, data[1].average_price);
    printf("%-20s%-20d%-20d\n", "Total Sold:", data[0].sold, data[1].sold);
    printf("%-20s%-20d%-20d\n", "Total Cost:", data[0].difference, data[1].difference);
    printf("%-20s%-20d%-20d\n", "Total Profit:", data[0].profit, data[1].profit);
    printf("%-20s%.2f%% %-12s %-.2f%% %-17s\n", "Cost Efficiency:", data[0].efficiency," ", data[1].efficiency," ");

}
void print_comparison(overall arr[]) {
    printf("%-25s%-20s%-20s\n", "Product Information:", "Day A", "Day B");
    printf("\n");
    printf("%-25s%-20d%-20d\n", "Total products:", arr[0].products, arr[1].products);
    printf("%-25s%-20d%-20d\n", "Total earned:", arr[0].sum, arr[1].sum);
    printf("%-25s%-20d%-20d\n", "Total cost:", arr[0].difference, arr[1].difference);
    printf("%-25s%-20d%-20d\n", "Total products sold:", arr[0].sold, arr[1].sold);
    printf("%-25s%-20.2f%-20.2f\n", "Average Price:", arr[0].average_price, arr[1].average_price);
    printf("%-25s%-20d%-20d\n", "Total Profit:", arr[0].profit, arr[1].profit);
    printf("%-25s%.2f%% %-13s %.2f%%%-20s\n", "Cost-Efficiency:", arr[0].efficiency / arr[0].products,"", arr[1].efficiency / arr[1].products,"");
}
void printTable(product* day, int product_count) {
    const int maxRows = 10;  // Maximum number of rows to print

    printf("\n%-25s%-25s%-25s%-25s%-25s%-25s%-25s\n\n", "Product No.", "Name", "Revenue", "Price", "Sold", "Cost", "Performance");

    // Print rows up to the 10th row or the total product count
    for (int i = 0; i < product_count && i < maxRows; i++) {
        printf("%-25d%-25s%-25d%-25d%-25d%-25d%.2f%%\n",
               i + 1,
               day[i].name,
               day[i].revenue,
               day[i].price,
               day[i].amount_sold,
               day[i].cost,
               day[i].efficiency);
    }
}
void PrintSubfolders(const char* folderPath) {
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    char searchPath[MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", folderPath);

    hFind = FindFirstFile(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No subfolders found.\n");
        return;
    }

    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                printf("%s\n", findData.cFileName);
            }
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
}
void print_month(){
    char* months[] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };

    int i;
    printf("\n");
    for (i = 0; i < 12; i++) {
        printf("%-15s", months[i]);

        // Print newline after every 4th month
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
}
void open_files(overall* all, int file_num,int num) {
    char fileName[8];
    int product_count;
    char* buffer;

    overall *total = &all[0];
    overall *total1 = &all[1];

    for (int i = 1; i <= file_num; i++) {
        sprintf(fileName, "%d.txt", i);
        FILE* file = fopen(fileName, "r");
        if (file == NULL) {
            printf("\nFailed to open file: %s", fileName);
            return;
        }
        product_count = file_counter(fileName);

        if(num==0){
            total->product_count += product_count;
            for (int j = 0; j < product_count; j++){

                char* trash = product_listing(file,&buffer);
                char* revenueString;
                revenueString = product_listing(file,&buffer);
                total->sum += safe_atoi(revenueString);
                char* priceString = product_listing(file,&buffer);
                total->average_price += atof(priceString);
                char* salesString = product_listing(file,&buffer);
                total->sold += safe_atoi(salesString);
                char* costString = product_listing(file,&buffer);
                total->difference += safe_atoi(costString);
            }
        }
        else{
            total1->product_count += product_count;
            for (int j = 0; j < product_count; j++){
                char* trash = product_listing(file,&buffer);
                char* revenueString;
                revenueString = product_listing(file,&buffer);
                total1->sum += safe_atoi(revenueString);
                char* priceString = product_listing(file,&buffer);
                total1->average_price += safe_atoi(priceString);
                char* salesString = product_listing(file,&buffer);
                total1->sold += safe_atoi(salesString);
                char* costString = product_listing(file,&buffer);
                total1->difference += safe_atoi(costString);
            }

        }
        free(buffer);
        fclose(file);
    }

}
int compareFilenames(const void* a, const void* b) {
    const char* filenameA = *(const char**)a;
    const char* filenameB = *(const char**)b;

    // Extract the numeric part of the filenames
    int numA = atoi(filenameA);
    int numB = atoi(filenameB);

    // Compare the numeric parts
    if (numA < numB) {
        return -1;
    } else if (numA > numB) {
        return 1;
    } else {
        return strcmp(filenameA, filenameB);
    }
}
int file_lister() {
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    char path[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, path);
    strcat(path, "\\*.txt");

    hFind = FindFirstFile(path, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No text files found.\n");
        return;
    }

    printf("\nCurrent text files in the directory:\n");

    char* filenames[MAX_FILES];
    int fileCount = 0;

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            // Remove file extension
            char* dot = strrchr(findData.cFileName, '.');
            if (dot != NULL) {
                *dot = '\0';
            }

            filenames[fileCount] = strdup(findData.cFileName);
            fileCount++;
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);

    if (fileCount == 0) {
        printf("\nNo text files found.\n");
        return NULL;
    }

    // Sort the filenames in ascending order
    qsort(filenames, fileCount, sizeof(char*), compareFilenames);

    // Print the sorted filenames
    print_filenames(filenames, fileCount);
}
void print_filenames(const char* filenames[], int fileCount) {
    int column = 0;

    for (int i = 0; i < fileCount; i++) {
        printf("%-6s", filenames[i]);

        column++;
        if (column == MAX_COLUMN_SIZE) {
            printf("\n");
            column = 0;
        }
    }

    if (column != 0) {
        printf("\n");
    }
}
double input_verifier_num(){
    double num;
    while(scanf("%lf",&num)!= 1){
    clearInputBuffer();
    printf("\nPlease type numerical input only: ");
    }
    return num;
}
char y_n(){
    char yes_no;
    printf("\nEnter another product? (y or n): ");
    scanf("%c",&yes_no);
    switch(yes_no){
        case 'y':
            clearInputBuffer();
            return 0;
        case 'n':
            clearInputBuffer();
            return 1;
        default:
            clearInputBuffer();
            y_n();
        }

}
int file_counter(char* fileName){
    int count = 0;
    char* counter;
    char* buffer;
    FILE* fptr;
    fptr = fopen(fileName,"r");
    while ((counter = product_listing(fptr, &buffer)) != NULL){
            count++;
        }
    count = count/5;
    free(buffer);
    fclose(fptr);
    return count;
}
char* product_listing(FILE* file, char** buffer) {
    int bufferSize = 100;
    int position = 0;
    *buffer = (char*)malloc(bufferSize * sizeof(char));

    if (*buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = fgetc(file)) != EOF && c != '\n') {
        (*buffer)[position++] = (char)c;

        if (position >= bufferSize) {
            bufferSize *= 2;
            *buffer = (char*)realloc(*buffer, bufferSize * sizeof(char));

            if (*buffer == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                return NULL;
            }
        }
    }

    if (position == 0 && c == EOF) {
        free(*buffer);
        *buffer = NULL;
        return NULL;
    }

    (*buffer)[position] = '\0';
    return *buffer;
}
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
    }
void title_function(char* text) {
    int length = strlen(text);
    int i;
    for (i = 0; i < length + 9; i++) {
        printf("*");
    }
    printf("\n");
    printf("  * ");
    printf("%s", text);
    printf(" *\n");
    for (i = 0; i < length + 9; i++) {
        printf("*");
    }
    printf("\n");
}
void exit_animation(char *phrase,int i){
    char storage[20];
    if(i<=5){
        printf("%s",phrase);
        strcpy(storage,phrase);
        strcat(storage,".");
        usleep(500000);
        i++;
        system("cls");
        exit_animation(storage,i);
    }
}
int day_counter(const char* folderPath) {
    int fileCount = 0;
    DIR* directory = opendir(folderPath);
    if (directory == NULL) {
        printf("Error opening the directory.\n");
        return -1;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        struct stat fileStat;
        char filePath[256];
        sprintf(filePath, "%s/%s", folderPath, entry->d_name);

        if (stat(filePath, &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
            fileCount++;
        }
    }

    closedir(directory);
    return fileCount;
}

//Error Checking Functions
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
void clear_extra_space() {
    char buffer[256];
    fgets(buffer, sizeof(buffer), stdin);
}
int safe_atoi(const char* str) {
    if (str == NULL) {
        fprintf(stderr, "Error: String pointer is NULL.\n");
        return 0;
    }

    return atoi(str);
}
void null_checker(FILE* file_pointer){
    if(file_pointer == NULL){
        exit_animation("Error in opening file, returning to main-menu.",0);
        main_menu();
    }
}
bool alloc_checker(void *ptr) {
    if (ptr == NULL) {
        printf("Memory allocation failed.\n");
        return false;
    }
    return true;
}
bool folderExists(const char *folderName) {
    DIR *directory = opendir(folderName);
    if (directory != NULL) {
        closedir(directory);
        return true;
    }
    return false;
}
