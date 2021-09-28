#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_FILE_PATH  "input.txt"
#define OUTPUT_FILE_PATH "output.txt"

#define MIN_TENANT_NAME_LENGTH 2
#define MAX_TENANT_NAME_LENGTH 30
#define MIN_ITEM_NAME_LENGTH 4
#define MAX_ITEM_NAME_LENGTH 15


typedef struct {
    unsigned char seconds;
    unsigned char minutes;
    unsigned char hours;
} Time;


typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned short year;
} Date;


typedef struct {
    char name[MAX_ITEM_NAME_LENGTH + 1];
    float size;
    unsigned char amount;
    char units[6];
} Item;


Item* createItem() {
    Item *new_item = (Item*) calloc(1, sizeof(Item));

    return new_item;
}


typedef struct {
    char tenant_name[MAX_TENANT_NAME_LENGTH + 1];
    Date date;
    Time time;
    Item **items;
    size_t items_count;
} SkiReport;


SkiReport* createSkiReport() {
    SkiReport *new_report = (SkiReport*) calloc(1, sizeof(SkiReport));
    if (new_report != NULL) {
        new_report->items = (Item**) malloc(0);
        new_report->items_count = 0;
    }

    return new_report;
}


typedef struct {
    size_t count;
    SkiReport **items;
} SkiReports;


bool isDateValid(Date *date) {
    bool is_year_leap;

    if (date->year % 4 != 0 || date->year % 400 != 0)
        is_year_leap = false;
    else
        is_year_leap = true;

    unsigned char max_day;

    // Check month and determine maximum day number
    switch (date->month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            max_day = 31;
            break;
        case 2:
            max_day = is_year_leap ? 29 : 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            max_day = 30;
            break;
        default:
            return false;
    }

    if (date->day <= 0 || date->day > max_day)
        return false;

    return true;
}


bool isTimeValid(Time *time) {
    if (!(0 <= time->hours && time->hours <= 23))
        return false;
    if (!(0 <= time->minutes && time->minutes <= 59))
        return false;
    if (!(0 <= time->seconds && time-> seconds <= 59))
        return false;
    return true;
}


void writeSkiReportToFile(SkiReport *report, FILE *file) {
    // Write tenant name
    fprintf(file, "%s has rented ", report->tenant_name);

    // Write items info
    for (size_t i = 0; i < report->items_count; ++i) {
        Item item = *(report->items[i]);
        fprintf(
                file, "%d %s of %sof size %g",
                item.amount, item.units, item.name, item.size
        );

        // Write "and" conjunction if it's pre-last item
        // or comma, if it's the last item
        if (i + 2 == report->items_count)
            fprintf(file, " and ");
        else if (i + 1 < report->items_count)
            fprintf(file, ", ");
    }

    // Write date and time info
    fprintf(
            file, " on %02d/%02d/%04d at %02d:%02d:%02d.\n",
            report->date.day, report->date.month, report->date.year,
            report->time.hours, report->time.minutes, report->time.seconds
    );
}


enum ReadingState {
    kReadingName,
    kReadingDatetime,
    kReadingItems
};


bool readTenantNameFromString(
        char *string, size_t string_length,
        char tenant_name[MAX_TENANT_NAME_LENGTH + 1]
        ) {
    if (string_length > MAX_TENANT_NAME_LENGTH)
        return false;

    if (
            sscanf(string, "%[a-zA-Z ]%*[^\n]", tenant_name) != 1
            || strlen(tenant_name) != string_length
            )
        return false;

    if (strlen(tenant_name) < MIN_TENANT_NAME_LENGTH)
        return false;

    return true;
}


bool readDateAndTimeFromString(
        char *string,
        Date *date,
        Time *time
) {
    if (sscanf(string, "%02hhu/%02hhu/%hu %02hhu:%02hhu:%02hhu",
               &(date->day), &(date->month), &(date->year),
               &(time->hours), &(time->minutes), &(time->seconds)
    ) != 6)
        return false;

    if (!isDateValid(date))
        return false;

    if (!isTimeValid(time))
        return false;

    return true;
}


bool readItemFromString(char *string, Item *item) {
    if (sscanf(string, "%15[ a-zA-Z] %f %2hhu %5[a-z]",
               item->name, &(item->size), &(item->amount), item->units
    ) != 4)
        // Invalid item format
        return false;

    size_t name_length = strlen(item->name);

    if (name_length < MIN_ITEM_NAME_LENGTH)
        // To short item name
        return false;

    if (item->name[name_length-1] != ' ')
        // Should be space between name and size
        return false;

    if (item->size == 0.0 || item->size > 200.0)
        // Item size is not in valid range=
        return false;

    if (item->amount == 0 || item->amount > 30)
        // Item amount is not in valid range
        return false;

    if (strcmp(item->units, "pair") == 0) {
        // Change units from 'pair' to 'pairs' if amount is greater than 1
        if (item->amount > 1)
            strcpy(item->units, "pairs");
    } else if (strcmp(item->units, "pairs") == 0) {
        // Units 'pairs' is incorrect, if amount is less than 2
        if (item->amount < 2) {
            return false;
        }
    } else if (strcmp(item->units, "pcs") != 0) {
        // If units is neither 'pair', neither 'pairs', neither 'pcs', return false
        return false;
    }

    return true;
}


// Reads ski reports from file to *reports* and returns
// true if reports were successfully read, false otherwise.
bool readSkiReportsFromFile(FILE *file, SkiReports *reports) {
    char string[50] = {0};
    enum ReadingState state = kReadingName;
    size_t success_reports_count = 0;
    SkiReport *current_report;

    while ((fgets(string, 49, file)) != NULL) {
        size_t string_length = strlen(string);

        // If string ends with line break, remove it
        if (string[string_length - 1] == '\n')
            string[--string_length] = '\0';

        switch (state) {
            case kReadingName:
                reports->items = (SkiReport**) realloc(reports->items, ++reports->count * sizeof(SkiReport*));
                reports->items[reports->count - 1] = current_report = createSkiReport();

                if (!readTenantNameFromString(
                        string, string_length,
                        reports->items[reports->count - 1]->tenant_name)
                        )
                    // Invalid tenant name
                    return false;

                state = kReadingDatetime;
                break;
            case kReadingDatetime:
                if (!readDateAndTimeFromString(
                        string, &(current_report->date), &(current_report->time)
                        ))
                    // Invalid date or/and time
                    return false;

                state = kReadingItems;
                break;
            case kReadingItems:
                if (string_length == 0) {
                    if (current_report->items_count < 1)
                        // No report items were provided
                        return false;
                    state = kReadingName;
                } else {
                    current_report->items = (Item**) realloc(
                            current_report->items,
                            ++(current_report->items_count) * sizeof(SkiReport*));
                    current_report->items[current_report->items_count - 1] = createItem();
                    if (!readItemFromString(string, current_report->items[current_report->items_count - 1]))
                        // Invalid item
                        return false;
                }
                break;
            default:
                return false;
        }
    }

    if (current_report->items_count < 1)
        // No report items were provided
        return false;

    ++success_reports_count;

    return true;
}


// Free dynamically created items.
void freeItems(Item **items, size_t items_count) {
    for (size_t i = 0; i < items_count; ++i)
        free(items[i]);
}


// Free dynamically created ski reports.
void freeSkiReports(SkiReports reports) {
    for (size_t i = 0; i < reports.count; ++i) {
        freeItems(reports.items[i]->items, reports.items[i]->items_count);
        free(reports.items[i]);
    }
}


int main() {
    FILE *fin, *fout;
    if ((fin = fopen(INPUT_FILE_PATH, "r")) == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }

    if ((fout = fopen(OUTPUT_FILE_PATH, "w")) == NULL) {
        fclose(fin);
        printf("Failed to open output file.\n");
        return 1;
    }

    SkiReports reports;
    reports.count = 0;
    reports.items = (SkiReport**) malloc(0);

    if (!readSkiReportsFromFile(fin, &reports))
        fprintf(fout, "Invalid input!\n");
    else
        for (size_t i = 0; i < reports.count; ++i)
            writeSkiReportToFile(reports.items[i], fout);

    freeSkiReports(reports);

    fclose(fin);
    fclose(fout);

    return 0;
}
