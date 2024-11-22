  
  
  

#ifndef BASHCALLS_H
#define BASHCALLS_H
// bashcalls.h
struct MemoryStruct {
    char* memory;
    size_t size;
};

struct WeatherData {
    double temp;
    double pressure;
    double humidity;
};
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) ;
void saveRawDataToFile(const char* filename, const char* data) ;
void send_poor_visibility_warning_and_threat(const char *recipient, const char *sender, const char *logFilename, double temp_condition, double pressure_condition, double humidity_condition);
void saveOrganizedDataToFile(const char* data);
void saveReportToFile(double avgTemperature, double avgPressure, double avgHumidity);
void generateAndSendReport(double avgTemperature, double avgPressure, double avgHumidity) ;
void processAndSaveData();
void retrieveWeatherData(struct MemoryStruct* responseData) ;


#endif // BASHCALLS_H
