#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for sleep
#include <curl/curl.h>
#include "cJSON.h"
#include "header/data.h"


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
