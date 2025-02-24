#include "app_WeatherPage.h"
#include "ui_WeatherPage.h"

void ui_weatherpage_test()
{
    LocationInfo location = {"", ""};
    if(get_location_info(&location) != 0) {
        printf("Failed to get location info.\n");
        return -1;
    }

    printf("City: %s, Adcode: %s\n", location.city, location.adcode);

    WeatherInfo weather_info = { "", "", "", "" };
    if(get_weather_info_by_adcode(location.adcode, &weather_info) != 0) {
        printf("Failed to get weather info.\n");
    } else {
        printf("Weather: %s, Temperature: %s, Humidity: %s, Wind Power: %s\n",
               weather_info.weather, weather_info.temperature, weather_info.humidity, weather_info.windpower);
    }

}