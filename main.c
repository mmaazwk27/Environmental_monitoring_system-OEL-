#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for sleep
#include <curl/curl.h>
#include "cJSON.h"
#include "header/data.h"
// #define API_KEY "e5e24f6fafa622984f7505534f34942b"
// #define API_URL "http://api.openweathermap.org/data/2.5/weather"
// #define LATITUDE "24.860966"
// #define LONGITUDE "66.990501"
// #define RAW_DATA_FILE "raw_data.json"
// #define ORGANIZED_DATA_FILE "organize_data.txt"//raw from json
// #define PROCESSED_DATA_FILE "processed_data.txt"//from organize 

// #define MAX_WEATHER_ENTRIES 3
// #define MAX_LINE_LENGTH 20000



// int main() {
    
    
//                 struct MemoryStruct responseData;
//                 retrieveWeatherData(&responseData);
//                 saveOrganizedDataToFile(responseData.memory);
//                 free(responseData.memory);
            
//                 processAndSaveData();

                

// }

int main() {
    struct MemoryStruct responseData;

    while (1) {
        // Step 1: Retrieve weather data
        retrieveWeatherData(&responseData);

        // Step 2: Save organized data
        saveOrganizedDataToFile(responseData.memory);
        free(responseData.memory);

        // Step 3: Process data and notify if conditions are met
        processAndSaveData();

        // Wait for 10 minutes
        sleep(60); // 600 seconds = 10 minutes
    }

    return 0;
}
