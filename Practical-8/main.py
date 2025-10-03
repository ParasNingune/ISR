import requests

def get_weather(city, api_key):
    base_url = "http://api.openweathermap.org/data/2.5/weather"
    
    url = f"{base_url}?q={city}&APPID={api_key}&units=metric"
    
    try:
        response = requests.get(url, timeout=10)
        data = response.json()

        if data["cod"] != "404":
            main = data["main"]
            wind = data["wind"]
            weather = data["weather"][0]

            temperature = main["temp"]
            feels_like = main["feels_like"]
            humidity = main["humidity"]
            pressure = main["pressure"]
            wind_speed = wind["speed"]
            wind_direction = wind.get("deg", "N/A")
            description = weather["description"]
            weather_type = weather["main"]

            print(f"\n{'='*50}")
            print(f"WEATHER REPORT FOR {city.upper()}")
            print(f"{'='*50}")
            print(f"Temperature: {temperature}°C (Feels like {feels_like}°C)")
            print(f"Wind Speed: {wind_speed} m/s")
            if wind_direction != "N/A":
                print(f"Wind Direction: {wind_direction}°")
            print(f"Humidity: {humidity}%")
            print(f"Pressure: {pressure} hPa")
            print(f"Weather: {weather_type}")
            print(f"Description: {description.title()}")
            print(f"{'='*50}")
        else:
            print(f"City '{city}' not found! Please check the spelling and try again.")
    
    except requests.exceptions.RequestException as e:
        print(f"Error fetching weather data: {e}")
    except KeyError as e:
        print(f"Error parsing weather data: Missing key {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

def main():
    print("Live Weather Report Application")
    print("="*50)

    
    api_key = input("\nEnter your OpenWeatherMap API key: ").strip()
    
    if not api_key:
        print("API key is required to fetch live weather data!")
        return
    
    while True:
        city_name = input("\nEnter city name (or 'quit' to exit): ").strip()
        
        if city_name.lower() == 'quit':
            print("\nThank you for using Weather App!")
            break
        
        if not city_name:
            print("Please enter a valid city name!")
            continue
        
        print(f"\nFetching weather data for {city_name}...")
        get_weather(city_name, api_key)
        
        continue_choice = input("\n🔄 Check another city? (y/n): ").strip().lower()
        if continue_choice != 'y':
            print("\n👋 Thank you for using Weather App!")
            break

if __name__ == "__main__":
    main()
