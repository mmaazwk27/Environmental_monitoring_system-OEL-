#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "header/data.h"
#define API_KEY "fa0be0ab1b53195ad3b1a999ba5ac8c9"
#define API_URL "http://api.openweathermap.org/data/2.5/weather"
#define LATITUDE "24.860966"
#define LONGITUDE "66.990501"
#define RAW_DATA_FILE "raw_data.json"
#define ORGANIZED_DATA_FILE "organize_data.txt"//raw from json
#define PROCESSED_DATA_FILE "processed_data.txt"//from organize 

#define MAX_WEATHER_ENTRIES 3
#define MAX_LINE_LENGTH 20000

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        // Out of memory!
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
void sendDesktopNotification(const char *title, const char *message) {
    // Print the notification to the terminal
    printf("Notification: %s: %s\n", title, message);

    // Send the notification to the desktop
    char command[512];
    snprintf(command, sizeof(command), "notify-send \"%s\" \"%s\"", title, message);
    int result = system(command);

    if (result != 0) {
        fprintf(stderr, "Failed to send desktop notification. Command: %s\n", command);
    } else {
        printf("Desktop notification sent successfully.\n");
    }
}

// void sendDesktopNotification(const char *title, const char *message) {
//     // Print the notification to the terminal
//     printf("%s: %s\n", title, message);

//     // Send the notification to the desktop
//     char command[512];
//     snprintf(command, sizeof(command), "notify-send \"%s\" \"%s\"", title, message);
//     int result = system(command);

//     if (result != 0) {
//         fprintf(stderr, "Failed to send desktop notification.\n");
//     }
// }

// void sendDesktopNotification(const char *title, const char *message) {
//     char command[512];
//     snprintf(command, sizeof(command), "notify-send \"%s\" \"%s\"", title, message);
//     int result = system(command);

//     if (result != 0) {
//         fprintf(stderr, "Failed to send desktop notification.\n");
//     }
// }


void saveRawDataToFile(const char* filename, const char* data) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s", data);
        fclose(file);
        printf("Raw data saved to %s\n", filename);
    } else {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
    }
}
// void send_poor_visibility_warning_and_threat(const char *recipient, const char *sender, const char *logFilename, double temp_condition, double pressure_condition, double humidity_condition) {
//     // Check if the visibility conditions are met
//     if (temp_condition > 300 || pressure_condition > 1200 || humidity_condition > 20) {
//         // Open the log file for appending
//         FILE *logFile = fopen(logFilename, "a");
//         if (logFile == NULL) {
//             fprintf(stderr, "Failed to open log file %s for writing\n", logFilename);
//             perror("fopen");
//             return;
//         }



//         // Get the current timestamp
//         time_t current_time;
//         struct tm *timeinfo;
//         char timestamp[64];
//         time(&current_time);
//         timeinfo = localtime(&current_time);
//         strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

//         // Append temperature, pressure, and humidity information to the log file
//         fprintf(logFile, "Timestamp: %s\n", timestamp);
//         fprintf(logFile, "Temperature: %.2f degree\n", temp_condition);
//         fprintf(logFile, "Pressure: %.2f\n", pressure_condition);
//         fprintf(logFile, "Humidity: %.2f\n", humidity_condition);

//         // Close the log file
//         fclose(logFile);}

//         // Prepare the command to execute the threat email script
//         const char *threat_email_script_path = "./send_email_script.sh";

//         char *command = (char *)malloc(512);  // Adjust the size accordingly

//         if (command == NULL) {
//             fprintf(stderr, "Failed to allocate memory for the command\n");
//         return;
// }

//         snprintf(command, 512, "%s '%s' '%s' '%s'", threat_email_script_path, recipient, sender,logFilename);
//         printf("Command: %s\n", command);  // Print the generated command

// // Execute the threat email script
//         int result = system(command);

// // Check the result of the system command
//         if (result == -1) {
//             fprintf(stderr, "Failed to execute the threat email script\n");
//             perror("system");
//             free(command);  // Free allocated memory
//             return;
// }       else if (WIFEXITED(result)) {
//             int exit_status = WEXITSTATUS(result);
//             printf("Email script exited with status: %d\n", exit_status);

// }

#include <unistd.h> // for sleep

void send_poor_visibility_warning_and_threat(const char *recipient, const char *sender, const char *logFilename, double temp_condition, double pressure_condition, double humidity_condition) {
    if (temp_condition > 300 || pressure_condition > 1200 || humidity_condition > 20) {
        char notification[512];
        snprintf(notification, sizeof(notification),
                 "notify-send 'Weather Alert' 'Temperature: %.2f°C, Pressure: %.2f, Humidity: %.2f'",
                 temp_condition, pressure_condition, humidity_condition);

        printf("Executing notification: %s\n", notification);
        int result = system(notification);

        if (result != 0) {
            fprintf(stderr, "Failed to send notification\n");
        } else {
            printf("Notification sent successfully\n");
        }
    }
}


// // Free allocated memory
// free(command);
//     }
void saveOrganizedDataToFile(const char* data) {
    FILE* file = fopen(ORGANIZED_DATA_FILE, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", data);  // Save to a new line
        fclose(file);
        printf("Organized data saved to %s\n", ORGANIZED_DATA_FILE);
    } else {
        fprintf(stderr, "Error opening file for writing: %s\n", ORGANIZED_DATA_FILE);
    }
}

char datetime[20];

void saveReportToFile(double avgTemperature, double avgPressure, double avgHumidity) {
    FILE* file = fopen("report.txt", "a");
    if (file != NULL) {
        time_t t = time(NULL);
        struct tm tm_info = *localtime(&t);

        char datetime[20];
        strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", &tm_info);

        fprintf(file, "Moderate Weather Report:\nSerial Number: %d\nDate and Time: %s\n"
                      "Latest Average Temperature: %.2lf\nLatest Average Pressure: %.2lf\nLatest Average Humidity: %.2lf\n\n",
                rand(), datetime, avgTemperature, avgPressure, avgHumidity);

        fclose(file);
        printf("Report saved to report.txt\n");
    } else {
        fprintf(stderr, "Error opening file for writing: report.txt\n");
    }
}

// void generateAndSendReport(double avgTemperature, double avgPressure, double avgHumidity) {
//     saveReportToFile(avgTemperature, avgPressure, avgHumidity);
//     printf("Moderate Weather Report:\n"
//                "Serial Number: %d\n"
//                "Date and Time: %s\n"
//                "Latest Average Temperature: %.2lf\nLatest Average Pressure: %.2lf\nLatest Average Humidity: %.2lf\n",
//                rand(), datetime, avgTemperature, avgPressure, avgHumidity);
// }

void generateAndSendReport(double avgTemperature, double avgPressure, double avgHumidity) {
    // Get the current date and time
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    char datetime[64];
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", &tm_info);

    // Save the report to the file
    saveReportToFile(avgTemperature, avgPressure, avgHumidity);

    // Print the report to the console
    printf("Moderate Weather Report:\n"
           "Serial Number: %d\n"
           "Date and Time: %s\n"
           "Latest Average Temperature: %.2lf\n"
           "Latest Average Pressure: %.2lf\n"
           "Latest Average Humidity: %.2lf\n",
           rand(), datetime, avgTemperature, avgPressure, avgHumidity);
}



// void processAndSaveData() {
//     FILE* file = fopen(ORGANIZED_DATA_FILE, "r");
//     if (file == NULL) {
//         fprintf(stderr, "Error opening file for reading: %s\n", ORGANIZED_DATA_FILE);
//         return;
//     }

//     struct WeatherData data[MAX_WEATHER_ENTRIES] = {0};
//     char line[MAX_LINE_LENGTH];

//     fseek(file, 0, SEEK_END);
//     long totalSize = ftell(file);
//     long lineStart = 0;
//     int linesRead = 0;

//     while (linesRead < MAX_WEATHER_ENTRIES && totalSize > 0) {
//         fseek(file, --totalSize, SEEK_SET);

//         if (fgetc(file) == '\n') {
//             fgets(line, sizeof(line), file);

//             cJSON* root = cJSON_Parse(line);
//             if (root != NULL) {
//                 cJSON* mainObj = cJSON_GetObjectItemCaseSensitive(root, "main");
//                 if (mainObj != NULL) {
//                     data[linesRead].temp = cJSON_GetObjectItemCaseSensitive(mainObj, "temp")->valuedouble;
//                     data[linesRead].pressure = cJSON_GetObjectItemCaseSensitive(mainObj, "pressure")->valuedouble;
//                     data[linesRead].humidity = cJSON_GetObjectItemCaseSensitive(mainObj, "humidity")->valuedouble;
//                     linesRead++;
//                 }

//                 cJSON_Delete(root);
//             }
//         }
//     }

//     double avg_temp = 0.0, avg_pressure = 0.0, avg_humidity = 0.0;

//     for (int i = 0; i < linesRead; i++) {
//         avg_temp += data[i].temp;
//         avg_pressure += data[i].pressure;
//         avg_humidity += data[i].humidity;
//     }

//     avg_temp /= linesRead;
//     avg_pressure /= linesRead;
//     avg_humidity /= linesRead;

//     FILE* output_file = fopen("processed_data.txt", "a");
//     if (output_file != NULL) {
//     fprintf(output_file, "Avg Temp: %.2lf, Avg Pressure: %.2lf, Avg Humidity: %.2lf\n",
//             avg_temp, avg_pressure, avg_humidity);

//     fflush(output_file);  // Flush the file buffer

//     fclose(output_file);
//     printf("Processed data saved to processed_data.txt\n");

//     generateAndSendReport(avg_temp, avg_pressure, avg_humidity);
// } else {
//     perror("fopen");
//     fprintf(stderr, "Error opening file for writing: processed_data.txt\n");
// }

//        if (avg_temp > 298 || avg_pressure > 1015 || avg_humidity > 20) {
//             const char *warning_file = "warning_log.txt";
//             const char *recipient_email = "mmaazwalik@gmail.com";
//             const char *sender_email = "khan4602328@cloud.neduet.edu.pk";
//             send_poor_visibility_warning_and_threat(recipient_email, sender_email, warning_file, avg_temp,avg_pressure,avg_humidity);
        
//     }

//     fclose(file);
// }
// void processAndSaveData() {
//     FILE* file = fopen(ORGANIZED_DATA_FILE, "r");
//     if (file == NULL) {
//         fprintf(stderr, "Error opening file for reading: %s\n", ORGANIZED_DATA_FILE);
//         return;
//     }

//     struct WeatherData data[MAX_WEATHER_ENTRIES] = {0};
//     char line[MAX_LINE_LENGTH];
//     int linesRead = 0;

//     // Read up to MAX_WEATHER_ENTRIES from the file
//     while (linesRead < MAX_WEATHER_ENTRIES && fgets(line, sizeof(line), file)) {
//         cJSON* root = cJSON_Parse(line);
//         if (root != NULL) {
//             cJSON* mainObj = cJSON_GetObjectItemCaseSensitive(root, "main");
//             if (mainObj != NULL) {
//                 cJSON* temp = cJSON_GetObjectItemCaseSensitive(mainObj, "temp");
//                 cJSON* pressure = cJSON_GetObjectItemCaseSensitive(mainObj, "pressure");
//                 cJSON* humidity = cJSON_GetObjectItemCaseSensitive(mainObj, "humidity");

//                 if (temp && pressure && humidity) {
//                     data[linesRead].temp = temp->valuedouble;
//                     data[linesRead].pressure = pressure->valuedouble;
//                     data[linesRead].humidity = humidity->valuedouble;
//                     linesRead++;
//                 }
//             }
//             cJSON_Delete(root);
//         }
//     }
//     fclose(file);

//     // Avoid division by zero
//     if (linesRead == 0) {
//         fprintf(stderr, "No valid weather data found in %s\n", ORGANIZED_DATA_FILE);
//         return;
//     }

//     // Calculate averages
//     double avg_temp = 0.0, avg_pressure = 0.0, avg_humidity = 0.0;
//     for (int i = 0; i < linesRead; i++) {
//         avg_temp += data[i].temp;
//         avg_pressure += data[i].pressure;
//         avg_humidity += data[i].humidity;
//     }

//     avg_temp /= linesRead;
//     avg_pressure /= linesRead;
//     avg_humidity /= linesRead;

//     // Save processed data to a file
//     FILE* output_file = fopen("processed_data.txt", "a");
//     if (output_file != NULL) {
//         fprintf(output_file, "Avg Temp: %.2lf, Avg Pressure: %.2lf, Avg Humidity: %.2lf\n",
//                 avg_temp, avg_pressure, avg_humidity);
//         fclose(output_file);
//         printf("Processed data saved to processed_data.txt\n");

//         // Generate and send the report
//         generateAndSendReport(avg_temp, avg_pressure, avg_humidity);
//     } else {
//         perror("fopen");
//         fprintf(stderr, "Error opening file for writing: processed_data.txt\n");
//     }
// }
void processAndSaveData() {
    FILE* file = fopen(ORGANIZED_DATA_FILE, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for reading: %s\n", ORGANIZED_DATA_FILE);
        return;
    }

    struct WeatherData data[MAX_WEATHER_ENTRIES] = {0};
    char line[MAX_LINE_LENGTH];
    int linesRead = 0;

    while (linesRead < MAX_WEATHER_ENTRIES && fgets(line, sizeof(line), file)) {
        cJSON* root = cJSON_Parse(line);
        if (root != NULL) {
            cJSON* mainObj = cJSON_GetObjectItemCaseSensitive(root, "main");
            if (mainObj != NULL) {
                cJSON* temp = cJSON_GetObjectItemCaseSensitive(mainObj, "temp");
                cJSON* pressure = cJSON_GetObjectItemCaseSensitive(mainObj, "pressure");
                cJSON* humidity = cJSON_GetObjectItemCaseSensitive(mainObj, "humidity");

                if (temp && pressure && humidity) {
                    printf("Raw Temperature (Kelvin): %.2f\n", temp->valuedouble);
                    data[linesRead].temp = temp->valuedouble - 273.15; // Convert to Celsius
                    printf("Converted Temperature (Celsius): %.2f\n", data[linesRead].temp);
                    data[linesRead].pressure = pressure->valuedouble;
                    data[linesRead].humidity = humidity->valuedouble;
                    linesRead++;
                }
            }
            cJSON_Delete(root);
        }
    }
    fclose(file);

    if (linesRead == 0) {
        fprintf(stderr, "No valid weather data found in %s\n", ORGANIZED_DATA_FILE);
        return;
    }

    double avg_temp = 0.0, avg_pressure = 0.0, avg_humidity = 0.0;
    for (int i = 0; i < linesRead; i++) {
        avg_temp += data[i].temp;
        avg_pressure += data[i].pressure;
        avg_humidity += data[i].humidity;
    }

    avg_temp /= linesRead;
    avg_pressure /= linesRead;
    avg_humidity /= linesRead;

    // Notify and log alerts for exceeded thresholds
    if (avg_temp > 27.0) {
        char message[256];
        snprintf(message, sizeof(message), "Temperature exceeded 27°C! Current: %.2f°C", avg_temp);
        printf("High Temperature Alert: %s\n", message); // Terminal output
        sendDesktopNotification("High Temperature Alert", message); // Desktop notification
    }
    if (avg_pressure > 1200.0) {
        char message[256];
        snprintf(message, sizeof(message), "Pressure exceeded 1200 hPa! Current: %.2f hPa", avg_pressure);
        printf("High Pressure Alert: %s\n", message); // Terminal output
        sendDesktopNotification("High Pressure Alert", message); // Desktop notification
    }
    if (avg_humidity > 20.0) {
        char message[256];
        snprintf(message, sizeof(message), "Humidity exceeded 20%%! Current: %.2f%%", avg_humidity);
        printf("High Humidity Alert: %s\n", message); // Terminal output
        sendDesktopNotification("High Humidity Alert", message); // Desktop notification
    }

    // Save processed data to a file
    FILE* output_file = fopen("processed_data.txt", "a");
    if (output_file != NULL) {
        fprintf(output_file, "Avg Temp: %.2lf°C, Avg Pressure: %.2lf, Avg Humidity: %.2lf%%\n",
                avg_temp, avg_pressure, avg_humidity);
        fclose(output_file);
        printf("Processed data saved to processed_data.txt\n");
    } else {
        perror("fopen");
        fprintf(stderr, "Error opening file for writing: processed_data.txt\n");
    }
}

// void processAndSaveData() {
//     FILE* file = fopen(ORGANIZED_DATA_FILE, "r");
//     if (file == NULL) {
//         fprintf(stderr, "Error opening file for reading: %s\n", ORGANIZED_DATA_FILE);
//         return;
//     }

//     struct WeatherData data[MAX_WEATHER_ENTRIES] = {0};
//     char line[MAX_LINE_LENGTH];
//     int linesRead = 0;

//     while (linesRead < MAX_WEATHER_ENTRIES && fgets(line, sizeof(line), file)) {
//         cJSON* root = cJSON_Parse(line);
//         if (root != NULL) {
//             cJSON* mainObj = cJSON_GetObjectItemCaseSensitive(root, "main");
//             if (mainObj != NULL) {
//                 cJSON* temp = cJSON_GetObjectItemCaseSensitive(mainObj, "temp");
//                 cJSON* pressure = cJSON_GetObjectItemCaseSensitive(mainObj, "pressure");
//                 cJSON* humidity = cJSON_GetObjectItemCaseSensitive(mainObj, "humidity");

//                 if (temp && pressure && humidity) {
//                     data[linesRead].temp = temp->valuedouble - 273.15;
//                     data[linesRead].pressure = pressure->valuedouble;
//                     data[linesRead].humidity = humidity->valuedouble;
//                     linesRead++;
//                 }
//             }
//             cJSON_Delete(root);
//         }
//     }
//     fclose(file);

//     if (linesRead == 0) {
//         fprintf(stderr, "No valid weather data found in %s\n", ORGANIZED_DATA_FILE);
//         return;
//     }

//     double avg_temp = 0.0, avg_pressure = 0.0, avg_humidity = 0.0;
//     for (int i = 0; i < linesRead; i++) {
//         avg_temp += data[i].temp;
//         avg_pressure += data[i].pressure;
//         avg_humidity += data[i].humidity;
//     }

//     avg_temp /= linesRead;
//     avg_pressure /= linesRead;
//     avg_humidity /= linesRead;

//     // Notify if thresholds are exceeded
//     if (avg_temp > 27.0) { // Adjusted threshold for Celsius
//         char message[256];
//         snprintf(message, sizeof(message), "Temperature exceeded 27°C! Current: %.2f°C", avg_temp);
//         printf("High Temperature Alert: %s\n", message); // Print to terminal
//         sendDesktopNotification("High Temperature Alert", message); // Send desktop notification
//     }
//     if (avg_pressure > 1200.0) {
//         char message[256];
//         snprintf(message, sizeof(message), "Pressure exceeded 1200 hPa! Current: %.2f hPa", avg_pressure);
//         printf("High Pressure Alert: %s\n", message); // Print to terminal
//         sendDesktopNotification("High Pressure Alert", message); // Send desktop notification
//     }
//     if (avg_humidity > 20.0) {
//         char message[256];
//         snprintf(message, sizeof(message), "Humidity exceeded 20%%! Current: %.2f%%", avg_humidity);
//         printf("High Humidity Alert: %s\n", message); // Print to terminal
//         sendDesktopNotification("High Humidity Alert", message); // Send desktop notification
//     }

//     FILE* output_file = fopen("processed_data.txt", "a");
//     if (output_file != NULL) {
//         fprintf(output_file, "Avg Temp: %.2lf, Avg Pressure: %.2lf, Avg Humidity: %.2lf\n",
//                 avg_temp, avg_pressure, avg_humidity);
//         fclose(output_file);
//         printf("Processed data saved to processed_data.txt\n");
//     } else {
//         perror("fopen");
//         fprintf(stderr, "Error opening file for writing: processed_data.txt\n");
//     }
// }



void retrieveWeatherData(struct MemoryStruct* responseData) {
    CURL* curl;
    CURLcode res;

    responseData->memory = malloc(1);
    responseData->size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char apiUrlWithParams[256];
        sprintf(apiUrlWithParams, "%s?lat=%s&lon=%s&appid=%s", API_URL, LATITUDE, LONGITUDE, API_KEY);

        curl_easy_setopt(curl, CURLOPT_URL, apiUrlWithParams);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)responseData);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
