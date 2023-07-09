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
#define MAX_ELEMENTS 12
#define MAX_HEIGHT 10


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

const char *getMonthName(int month);
const char* convertToMonth(int monthNumber);
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
        system("cls");
        if(chdir("years") == 0){
            printf("Folder entered");
        }
        system("cls");
        trend_year();
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
void sales_performance_menu(char* string) {
    char folderName[15];
    char date[15];
    title_function("Sales Performance Menu");
    printf("\nAvailable months: ");
    print_month();
    printf("\nEnter month (numerical equivalent): ");
    scanf("%s", folderName);

    int month = atoi(folderName);  // Convert the input to an integer

    if (month >= 1 && month <= 12) {
        const char* monthNames[] = {
            "January", "February", "March", "April",
            "May", "June", "July", "August",
            "September", "October", "November", "December"
        };

        const char* monthName = monthNames[month - 1];

        if (folderExists(monthName)) {
            if (chdir(monthName) == 0) {
                printf("Folder entered.\n");
                system("cls");
                strcpy(date, string);
                strcat(date, "-");
                strcat(date, monthName);
                clearInputBuffer();
                sales_performance_day(date);
            }
            else {
                printf("\nFailed to enter directory.\n");
                system("cls");
                printf("Press any key to continue...");
                _getch();
                chdir("..");
                main_menu();
            }
        }
    }
    else {
        system("cls");
        printf("Invalid month input.\n");
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
    int expense,input,sum = 0,difference = 0,total_sold = 0,profit_sum = 0,prompt,avg_price=0;
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
        printProductInfo(day,input);
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
        avg_price += day[i].price;
        total_sold += day[i].amount_sold;
        day[i].efficiency = efficiency_calc(day,i);
        efficiency_sum += day[i].efficiency;
    }
        printTable(day, product_count);
        printf("\n");
        title_function("Day Summary");
        printf("%-30s%d\n", "Total revenue:", sum);
        printf("%-30s%d\n", "Average Price:", avg_price/product_count);
        printf("%-30s%d\n", "Total Product Sold:", total_sold);
        printf("%-30s%d\n", "Total expenditure:", difference);
        printf("%-30s%d\n", "Today's Profit:", sum + difference);
        printf("%-30s%.2f%%\n\n", "Avg. Cost-Efficiency:", efficiency_sum/product_count);
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
    char* folderName;
    char date[15];
    int num;
    title_function("Sales Comparison Menu");
    printf("Available Months:");
    print_month();
    printf("\nEnter month (numerical equivalent): ");
    if(scanf("%d", &num) == 0){
        clearInputBuffer();
        system("cls");
        month_comparison_menu(string);
        if(num >12 || num <= 0){
            system("cls");
            month_comparison_menu(string);
        }
    }
    folderName = convertToMonth(num);
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
    int num_days;
    char* buffer;
    int day_count = day_counter(".");

    title_function("Sales Comparison Menu");

    printf("\nEnter how many days to compare: ");
    if(scanf("%d",&num_days) == 0){
        clearInputBuffer();
        system("cls");
        day_comparison_menu(string);
    }
    if(num_days > day_count){
        clearInputBuffer();
        system("cls");
        day_comparison_menu(string);
    }
    if(num_days <= 0){
        clearInputBuffer();
        system("cls");
        chdir("..");
        chdir("..");
        main_menu();
    }
    int input[num_days];
    int dummy[num_days];
    printf("\n\nEnter days to compare: \n");
    for(int i = 0;i<num_days;i++){
        if(scanf("%d",&input[i]) == 0){
            clearInputBuffer();
            system("cls");
            day_comparison_menu(string);
            if(input[i] > day_count){
                printf("\nYou went past the day count!");
                printf("\n\nPress any key to continue...");
                _getch();
                clearInputBuffer();
                system("cls");
                day_comparison_menu(string);
            }
            if(input[i] <= 0){
                printf("\nInvalid value");
                printf("\n\nPress any key to continue...");
                _getch();
                clearInputBuffer();
                system("cls");
                day_comparison_menu(string);
            }

        }

        for (int j = 0; j < i; j++) {
            if (input[i] == input[j]) {
                printf("\nYou entered a duplicate value!");
                printf("\n\nPress any key to continue...");
                _getch();
                clearInputBuffer();
                system("cls");
                day_comparison_menu(string);
            }
        }
        dummy[i] = input[i];
    }


    FILE* fptr;
    int file_count[num_days];

    char filename[20];  // Variable to hold the filename string

    for (int i = 0; i < num_days; i++) {
        sprintf(filename, "%d.txt", input[i]);  // Generate the filename string
        file_count[i] = file_counter(filename);
    }

    product** day = (product**)malloc(sizeof(product*) * num_days);
    for(int i = 0;i<num_days;i++){
        day[i] = (product*)malloc(sizeof(product)*file_count[i]);
    }
    for (int i = 0; i < num_days; i++) {
        char filename[20];
        sprintf(filename, "%d.txt", input[i]);
        fptr = fopen(filename, "r");
        if (fptr == NULL) {
            // Handle file open error
            printf("Error opening file: %s\n", filename);
            // Additional error handling code...
        } else {
            // File opened successfully, read data
            for (int j = 0; j < file_count[i]; j++) {
                // Read product data from the file and store in day[i][j]
                char* name = product_listing(fptr, &buffer);
                strcpy(day[i][j].name, name);
                char* revenueString = product_listing(fptr, &buffer);
                day[i][j].revenue = safe_atoi(revenueString);
                char* priceString = product_listing(fptr, &buffer);
                day[i][j].price = atof(priceString);
                char* salesString = product_listing(fptr, &buffer);
                day[i][j].amount_sold = safe_atoi(salesString);
                char* costString = product_listing(fptr, &buffer);
                day[i][j].cost = safe_atoi(costString);
            }
            fclose(fptr);
        }
    }
    overall total[num_days];
    memset(total, 0, sizeof(total));

    for (int i = 0; i < num_days; i++) {
        total[i].products = file_count[i];
        for (int i = 0; i < num_days; i++) {
            for (int j = 0; j < file_count[i]; j++) {
                total[i].sum += day[i][j].revenue;
                total[i].difference += day[i][j].cost;
                total[i].sold += day[i][j].amount_sold;
                total[i].average_price += day[i][j].price;
                total[i].efficiency += ((float)(day[i][j].revenue + day[i][j].cost) / ((float)day[i][j].amount_sold * day[i][j].price)) * 100.0;
            }
        }
    }
    for(int i = 0;i<num_days;i++){
        for(int j = 0;j<file_count[i];j++){
        total[i].profit = total[i].sum + total[i].difference;
        }
    }

    system("cls");
    printf("\n");
    print_comparison(total,num_days,&dummy,string);
    printf("\n");
    for (int i = 0; i < num_days; i++) {
        free(day[i]);
    }
    free(day);
    free(buffer);
    fclose(fptr);
    system("cls");
    chdir("..");
    chdir("..");
    main_menu();
}

void month_comparison(char*string){
    int num_month;
    overall* total;
    char title[100];


    title_function("Month Comparison Menu");
    printf("\nAvailable Months: ");
    print_month();
    printf("\n");
    printf("Compare how many months [1-12]: ");

    if(scanf("%d",&num_month) == 0){
        clearInputBuffer();
        system("cls");
        month_comparison(string);
    }
    if(num_month > 12){
        system("cls");
        month_comparison(string);
    }
    total = (overall*)malloc(sizeof(overall)*num_month);
    memset(total, 0, sizeof(overall) * num_month);
    int month[num_month];
    int file_count[num_month];
    char input[num_month][13];

const char* monthNames[] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"
};

char convertedMonths[num_month][20];  // Array to store the converted month names

for (int i = 0; i < num_month; i++) {
    printf("\nMonth [%d]: ", i + 1);
    scanf("%s", input[i]);

    month[i] = atoi(input[i]);  // Convert the input to an integer
    if (month[i] >= 1 && month[i] <= 12) {
        const char* monthName = monthNames[month[i] - 1];

        if (folderExists(monthName)) {
            // Check for same month input
            for (int j = 0; j < i; j++) {
                if (month[i] == month[j]) {
                    printf("\nYou have input the same month.\n");
                    printf("Press any key to continue...");
                    _getch();
                    system("cls");
                    clearInputBuffer();
                    month_comparison(string);
                }
            }

            strcpy(convertedMonths[i], monthName);  // Store the converted month name
            continue;
        }
        else {
            printf("\nThe folder you entered does not exist!");
            printf("\n\n\nPress any key to continue...");
            _getch();
            system("cls");
            clearInputBuffer();
            month_comparison(string);
        }
    }
    else{
        system("cls");
        month_comparison(string);
    }
}
    for(int i = 0;i<num_month;i++){
        chdir(convertedMonths[i]);
        file_count[i] = day_counter(".");
        chdir("..");
    }
    for(int i = 0;i<num_month;i++){
        chdir(convertedMonths[i]);
        open_files(&total[i],file_count[i],i);
        chdir("..");
    }

    system("cls");
    calculateEfficiency(total, num_month);
    calculate_profit(total,num_month);
    printOverall(total,num_month,convertedMonths);
    printf("\n\nPress any key to continue...");
    _getch();
    chdir("..");
    system("cls");
    main_menu();
}

//Trend Analytics
void trend_year() {
    char year[10];
    title_function("Trend Analytics Menu");
    printf("Note: Enter 4 to go back to Main Menu\n\n");
    printf("Available Years: \n");
    PrintSubfolders(".");
    printf("\nEnter Year: ");
    scanf("%s", &year);

    if (folderExists(year)) {
        if(chdir(year) == 0){
            system("cls");
            trend_month(year);
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
        trend_year();
    }
}
void trend_month(char* string) {
    char folderName[15];
    char date[15];
    title_function("Trend Analytics Menu");
    printf("\nAvailable Months:");
    print_month();
    printf("\nEnter month: ");

    int monthNumber;
    scanf("%d", &monthNumber);

    const char* monthName = convertToMonth(monthNumber);
    if (strcmp(monthName, "Invalid month number") == 0) {
        printf("Invalid month number. Please enter a number between 1 and 12.\n");
        trend_month(string);
        return;
    }

    strcpy(folderName, monthName);
    strcpy(date, string);
    strcat(date, "-");
    strcat(date, folderName);

    if (folderExists(folderName)) {
        if (chdir(folderName) == 0) {
            printf("Folder entered");
            system("cls");
            clearInputBuffer();
            trend_analytics(date, string);
        } else {
            printf("\nFailed to enter directory\n");
            system("cls");
            printf("Press any key to continue...");
            _getch();
            chdir("..");
            main_menu();
        }
    } else if (strcmp(folderName, "4") == 0) {
        system("cls");
        chdir("..");
        trend_year();
    } else {
        system("cls");
        printf("Could not find '%s' folder.\n", folderName);
        system("cls");
        trend_month(string);
    }
}
void trend_analytics(char* string,char* backup){
    char title[20];
    strcpy(title,string);
    strcat(title," Trend Analytics");
    title_function(title);

    int input;

    printf("Month Chosen: %s\n\n",string);
    printf("Trend:");
    printf("\n1. Highest earning product");
    printf("\n2. Top selling product");
    printf("\n3. Most Cost-Efficient product");
    printf("\n4. Month's Performance Graph");
    printf("\n5. Back");
    printf("\n6. Exit");
    printf("\n\n\nEnter Choice: ");
    scanf("%d",&input);

    switch(input){
    case 1:
        system("cls");
        trend_revenue(string,backup);
        break;
    case 2:
        system("cls");
        trend_product_sold(string,backup);
        break;
    case 3:
        system("cls");
        trend_cost_efficiency(string,backup);
        break;

    case 4:
        system("cls");
        trend_month_graph(title,backup);
        break;
    case 5:
        system("cls");
        chdir("..");
        trend_month(backup);
        break;
    case 6:
        system("cls");
        chdir("..");
        chdir("..");
        main_menu();
    default:
        system("cls");
        clearInputBuffer();
        trend_analytics(string,backup);
        break;
    }

    return;
}
void trend_revenue(char* string, char* backup){
    product highest_revenue;
    memset(&highest_revenue, 0, sizeof(product));

    int day;
    for (day = 1; day <= 30; day++) {
        char filename[20];
        sprintf(filename, "%d.txt", day);
        FILE* file = fopen(filename, "r");
        if (file != NULL) {
            char* buffer;
            product current_product;
            memset(&current_product, 0, sizeof(product));

            if (product_listing(file, &buffer) != NULL) {
                strcpy(current_product.name, buffer);
                free(buffer);

                fscanf(file, "%d", &current_product.revenue);
                fscanf(file, "%d", &current_product.amount_sold);
                fscanf(file, "%d", &current_product.price);
                fscanf(file, "%d", &current_product.cost);

                if (current_product.revenue > highest_revenue.revenue) {
                    highest_revenue = current_product;
                }
            }

            fclose(file);
        }
    }

    title_function("Highest Selling Product");
    printf("Name: %s\n", highest_revenue.name);
    printf("Revenue: %d\n", highest_revenue.revenue);
    printf("Amount Sold: %d\n", highest_revenue.amount_sold);
    printf("Price: %d\n", highest_revenue.price);
    printf("Cost: %d\n", highest_revenue.cost);
    printf("\n\n\nPress any key to continue...");
    _getch();
    system("cls");
     clearInputBuffer();
    trend_analytics(string,backup);
}
void trend_product_sold(char* string, char* backup){
    product highest_sold;
    memset(&highest_sold, 0, sizeof(product));

    int day;
    for (day = 1; day <= 31; day++) {
        char filename[20];
        sprintf(filename, "%d.txt", day);
        FILE* file = fopen(filename, "r");
        if (file != NULL) {
            char* buffer;
            product current_product;
            memset(&current_product, 0, sizeof(product));

            if (product_listing(file, &buffer) != NULL) {
                strcpy(current_product.name, buffer);
                free(buffer);

                fscanf(file, "%d", &current_product.revenue);
                fscanf(file, "%d", &current_product.amount_sold);
                fscanf(file, "%d", &current_product.price);
                fscanf(file, "%d", &current_product.cost);

                if (current_product.amount_sold > highest_sold.amount_sold) {
                    highest_sold = current_product;
                }
            }

            fclose(file);
        }
    }

    title_function("This month's top seller");
    printf("Name: %s\n", highest_sold.name);
    printf("Revenue: %d\n", highest_sold.revenue);
    printf("Amount Sold: %d\n", highest_sold.amount_sold);
    printf("Price: %d\n", highest_sold.price);
    printf("Cost: %d\n", highest_sold.cost);
    printf("\n\n\nPress any key to continue...");
    _getch();
    system("cls");
     clearInputBuffer();
    trend_analytics(string,backup);
}
void trend_cost_efficiency(char* string, char* backup) {
    product most_efficient;
    memset(&most_efficient, 0, sizeof(product));
    float highest_efficiency = 0.0;

    int day;
    for (day = 1; day <= 31; day++) {
        char filename[20];
        sprintf(filename, "%d.txt", day);
        FILE* file = fopen(filename, "r");
        if (file != NULL) {
            char* buffer;
            product current_product;
            memset(&current_product, 0, sizeof(product));

            if (product_listing(file, &buffer) != NULL) {
                strcpy(current_product.name, buffer);
                free(buffer);


                fscanf(file, "%d", &current_product.revenue);
                fscanf(file, "%d", &current_product.amount_sold);
                fscanf(file, "%d", &current_product.price);
                fscanf(file, "%d", &current_product.cost);
                float efficiency = (float)(current_product.revenue - (current_product.cost * -1)) / ((float)current_product.amount_sold * current_product.price) * 100.0;

                if (efficiency > highest_efficiency) {
                    highest_efficiency = efficiency;
                    most_efficient = current_product;
                }
            }

            fclose(file);
        }
    }

    title_function("Most Cost-Efficient Product");
    printf("Name: %s\n", most_efficient.name);
    printf("Revenue: %d\n", most_efficient.revenue);
    printf("Amount Sold: %d\n", most_efficient.amount_sold);
    printf("Price: %d\n", most_efficient.price);
    printf("Cost: %d\n", most_efficient.cost);
    printf("Cost-Efficiency: %.2f%%\n", highest_efficiency);
    printf("\n\n\nPress any key to continue...");
    _getch();
    system("cls");
    clearInputBuffer();
    trend_analytics(string,backup);
}
void trend_month_graph(char* string, char* backup){
    title_function("   Cost-Efficiency Performance    ");
    int limit = day_counter(".");
    char day_name[8];
    int product_count;
    int num_month = 12;
    int file_count[num_month];
    overall* total;
    FILE* fptr;
    char* buffer;
    char convertedMonths[num_month][20];
    total = (overall*)malloc(sizeof(overall)*num_month);
    memset(total, 0, sizeof(overall)*num_month);
    for(int i = 0;i<num_month;i++){
        strcpy(convertedMonths[i], convertToMonth(i+1));
    }
    for(int i = 0;i<num_month;i++){
        chdir(convertedMonths[i]);
        file_count[i] = day_counter(".");
        chdir("..");
    }
    for(int i = 0;i<num_month;i++){
        chdir(convertedMonths[i]);
        open_files(&total[i],file_count[i],i);
        chdir("..");
    }
      calculateEfficiency2(total,12);
        createGraph(total,&convertedMonths);
    main_menu();
}
void createGraph(overall data[]) {
    int heights[MAX_ELEMENTS];
    int maxVal = 0;
    int i, j;

    // Get the heights from the efficiency field of the data
    for (i = 0; i < MAX_ELEMENTS; i++) {
        heights[i] = (int)(data[i].efficiency * MAX_HEIGHT);

        // Find the maximum height in the array
        if (heights[i] > maxVal) {
            maxVal = heights[i];
        }
    }

    // Cap values exceeding the maximum height
    if (maxVal > MAX_HEIGHT) {
        maxVal = MAX_HEIGHT;
    }

    // Create the graph
    for (i = MAX_HEIGHT; i > 0; i--) {
        for (j = 0; j < MAX_ELEMENTS; j++) {
            if (heights[j] >= i) {
                printf(" *  ");
            } else {
                printf("    ");
            }
        }
        printf("\n");
    }

    // Print the horizontal barrier
    for (i = 0; i < MAX_ELEMENTS; i++) {
        printf("----");
    }
    printf("\n");

    // Print the month headers
    for (i = 0; i < MAX_ELEMENTS; i++) {
        printf("%-4s", getMonthName(i + 1));
    }
    printf("\n");
}
const char *getMonthName(int month) {
    const char *months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    if (month >= 1 && month <= 12) {
        return months[month - 1];
    }

    return "";
}
//Calculation Functions
void calculateEfficiency(overall* data, int size) {
    for (int i = 0; i < size; i++) {
        data[i].efficiency = (data[i].sum + data[i].difference) / (data[i].sold * data[i].average_price)*10000;
    }
}
void calculateEfficiency2(overall* data, int size) {
    for (int i = 0; i < size; i++) {
        data[i].efficiency = (data[i].sum + data[i].difference) / (data[i].sold * data[i].average_price)*100;
    }
}
void calculate_profit(overall* all, int size) {
    for (int i = 0; i < size; i++) {
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
const char* convertToMonth(int monthNumber) {
    const char* months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    if (monthNumber >= 1 && monthNumber <= 12) {
        return months[monthNumber - 1];
    } else {
        return "Invalid month number";
    }
}
void printProductInfo(product day[], int input) {
    printf("%-20s%d\n", "Product revenue:", day[input-1].revenue);
    printf("%-20s%d\n", "Price per Serving:", day[input-1].price);
    printf("%-20s%d\n", "Amount sold:", day[input-1].amount_sold);
    printf("%-20s%d\n", "Expenditure:", day[input-1].cost);
    printf("%-20s%d\n", "Profit Earned:", day[input-1].cost + day[input-1].revenue);
    printf("%-20s%.2f%%\n\n", "Cost-Efficiency:", day[input-1].efficiency);
    printf("Press any key to continue.....");
}
void printOverall(const overall data[], int size, char month[][20]) {
    int start = 0;
    int rowsRemaining = size;

    while (rowsRemaining > 0) {
        title_function("Month-to-Month Comparison");
        printf("\n");
        int rowsToPrint = (rowsRemaining >= 4) ? 4 : rowsRemaining;

        printf("%-25s", "Summary");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20s", month[i]);
        }
        printf("\n");

        printf("%-20s", "------------------");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20s", "--------------------");
        }
        printf("\n");

        printf("%-25s", "Total Revenue:");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20d", data[i].sum);
        }
        printf("\n");

        printf("%-25s", "Total Price:");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20.2f", data[i].average_price);
        }
        printf("\n");

        printf("%-25s", "Total Amount Sold:");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20d", data[i].sold);
        }
        printf("\n");

        printf("%-25s", "Total Expenditure:");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20d", data[i].difference);
        }
        printf("\n");

        printf("%-25s", "Total Profit:");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20d", data[i].profit);
        }
        printf("\n");

        printf("%-25s", "Cost-Efficiency(%) :");
        for (int i = start; i < start + rowsToPrint; i++) {
            printf("%-20.2f", data[i].efficiency);
        }
        printf("\n");

        rowsRemaining -= rowsToPrint;
        start += rowsToPrint;

        if (rowsRemaining > 0) {
            printf("\nPress any key to show the next set of results...");
            _getch();
            system("cls");
        }
    }
}

void print_comparison(overall arr[], int rows, int* user_input,char* string) {
    int cols = rows; // Assuming the number of columns is equal to the number of rows

    int start = 0;
    int end = (rows > 12) ? 12 : rows; // Limit to 12 rows

    while (start < rows) {
        system("cls");
        title_function(string);
        printf("%-25s", "Chosen Day:");

        // Print header for each day
        for (int i = start; i < end; i++) {
            printf("%-10d", user_input[i]);
        }
        printf("\n");

        // Print table data for each field
        printf("%-25s", "Total products:");
        for (int i = start; i < end; i++) {
            printf("%-10d", arr[i].products);
        }
        printf("\n");

        printf("%-25s", "Total earned:");
        for (int i = start; i < end; i++) {
            printf("%-10d", arr[i].sum);
        }
        printf("\n");

        printf("%-25s", "Total products sold:");
        for (int i = start; i < end; i++) {
            printf("%-10d", arr[i].sold);
        }
        printf("\n");

        printf("%-25s", "Average Price:");
        for (int i = start; i < end; i++) {
            printf("%-10.2f", arr[i].average_price/arr[i].products);
        }
        printf("\n");

        printf("%-25s", "Total expenditure:");
        for (int i = start; i < end; i++) {
            printf("%-10d", arr[i].difference);
        }
        printf("\n");

        printf("%-25s", "Total profit:");
        for (int i = start; i < end; i++) {
            printf("%-10d", arr[i].profit);
        }
        printf("\n");

        printf("%-25s", "Cost Efficiency(%):");
        for (int i = start; i < end; i++) {
            printf("%-10.2f", arr[i].efficiency / arr[i].products);
        }
        printf("\n");

        if (end < rows) {
            printf("\nPress any key to show the next set of results...");
            _getch();
            start = end;
            end = (end + 12 < rows) ? end + 12 : rows;
        } else {
            printf("\nPress any key to continue...");
            _getch();
            break;
        }
    }
}

void printTable(product* day, int product_count) {
    const int maxRows = 10;

    printf("%-20s%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n\n", "Product No.", "Name", "Revenue", "Price", "Amnt. Sold", "Expenditure", "Profit", "Cost-Efficiency");

    for (int i = 0; i < product_count && i < maxRows; i++) {
        int profit = day[i].revenue - day[i].cost;
        printf("%-20d%-20s%-20d%-20d%-20d%-20d%-20d%.2f%%\n",
               i + 1,
               day[i].name,
               day[i].revenue,
               day[i].price,
               day[i].amount_sold,
               day[i].cost,
               profit,
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
void print_month() {
    char* months[] = {
        "[01] January", "[02] February", "[03] March", "[04] April",
        "[05] May", "[06] June", "[07] July", "[08] August",
        "[09] September", "[10] October", "[11] November", "[12] December"
    };

    int i;
    printf("\n");
    for (i = 0; i < 12; i++) {
        printf("%-20s", months[i]);

        // Print newline after every 4th month
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
}
void open_files(overall* all, int file_num,int num) {
    char fileName[11];
    int product_count;
    char* buffer;

    overall *total = all;

    for (int i = 1; i <= file_num; i++) {
        sprintf(fileName, "%d.txt", i);
        FILE* file = fopen(fileName, "r");
        if (file == NULL) {
            printf("\nFailed to open file: %s", fileName);
            return;
        }
        product_count = file_counter(fileName);

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
            free(buffer);
            fclose(file);
            }


}

int compareFilenames(const void* a, const void* b) {
    const char* filenameA = *(const char**)a;
    const char* filenameB = *(const char**)b;


    int numA = atoi(filenameA);
    int numB = atoi(filenameB);


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
