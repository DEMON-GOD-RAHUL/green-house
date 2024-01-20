import speech_recognition as sr
import os
import requests
import AppKit
import time
import urllib.request
import tkinter as tk
import threading

root_url = "http://192.168.123.1"
city =  "Dharan"
voice_identifier = 'com.apple.speech.synthesis.voice.karen'
port = 80


def speak(text_to_speak):
        # Create an instance of NSSpeechSynthesizer
        synthesizer = AppKit.NSSpeechSynthesizer.alloc().init()

        # Set the desired voice using the voice identifier
        synthesizer.setVoice_(voice_identifier)

        #displaying the text
        print(f"Assistant: {text_to_speak}")

        # Speak the provided text
        synthesizer.startSpeakingString_(text_to_speak)

        # Wait for the speech to finish
        while synthesizer.isSpeaking():
            time.sleep(0.1)

#================ IOT FUNCTIONS ================#
def sendRequest(url):
    n = urllib.request.urlopen(url)

def turn_on_light():
    sendRequest(root_url+"/ledon")
    speak("Light turned on")

def turn_off_light():
    sendRequest(root_url+"/ledoff")
    speak("Light turned off")

def turn_off_fan():
    sendRequest(root_url+"/fanoff")
    speak("Fan turned off")

def turn_on_fan():
    sendRequest(root_url+"/fanon")
    speak("Fan turned on")

def shutdown():
    speak("Disabiling voice functionality")
    exit()

def current_weather():
        base_url = "http://api.openweathermap.org/data/2.5/weather?"
        api_key = "a53ab8dbec0f35c840d8cc5ae5f1cd3b"
        city = "Dharan"

        url = base_url + "q=" + city + "&units=metric&APPID=" + api_key
        try:
            response = requests.get(url)
            data = response.json()

            if response.status_code == 200:
                # Extract relevant information from the API response
                temperature = data['main']['temp']
                description = data['weather'][0]['description']

                #print(f"Weather in {city}:")
                #print(f"Temperature: {temperature}°C")
                #print(f"Description: {description}")
                speak(f"{city} has a temperature of {temperature} degree celcius and it is {description}")
            else:
                if 'message' in data:
                    print(f"Error: {data['message']}")
                else:
                    print(f"Error: {response.text}")

        except requests.exceptions.RequestException as e:
            print(f"An error occurred: {e}")

#=============  FOR GUI =============#
class GreenHouseGUI:
    def __init__(self, master):
        self.master = master
        self.master.title("Green House Monitoring and Automation System")

        # Light Card
        self.light_card = tk.Frame(self.master, padx=20, pady=20)
        self.light_label = tk.Label(self.light_card, text="Light")
        self.light_label.pack()

        self.light_on_button = tk.Button(self.light_card, text="On", command=self.light_on)
        self.light_on_button.pack(side=tk.LEFT, padx=5)
        
        self.light_off_button = tk.Button(self.light_card, text="Off", command=self.light_off)
        self.light_off_button.pack(side=tk.LEFT, padx=5)

        # Fan Card
        self.fan_card = tk.Frame(self.master, padx=20, pady=20)
        self.fan_label = tk.Label(self.fan_card, text="Fan")
        self.fan_label.pack()

        self.fan_on_button = tk.Button(self.fan_card, text="On", command=self.fan_on)
        self.fan_on_button.pack(side=tk.LEFT, padx=5)
        
        self.fan_off_button = tk.Button(self.fan_card, text="Off", command=self.fan_off)
        self.fan_off_button.pack(side=tk.LEFT, padx=5)

        # Humidity Card
        """self.humidity_card = tk.Frame(self.master, padx=20, pady=20)
        self.humidity_label = tk.Label(self.humidity_card, text="Humidity")
        self.humidity_label.pack()

        self.humidity_value = tk.StringVar()
        self.humidity_value_label = tk.Label(self.humidity_card, textvariable=self.humidity_value)
        self.humidity_value_label.pack()"""

        # Temperature Card
        """self.temperature_card = tk.Frame(self.master, padx=20, pady=20)
        self.temperature_label = tk.Label(self.temperature_card, text="Temperature")
        self.temperature_label.pack()

        self.temperature_value = tk.StringVar()
        self.temperature_value_label = tk.Label(self.temperature_card, textvariable=self.temperature_value)
        self.temperature_value_label.pack()"""

        # Voice Control Card
        self.voice_card = tk.Frame(self.master, padx=20, pady=20)
        self.voice_label = tk.Label(self.voice_card, text="Voice Control")
        self.voice_label.pack()

        self.voice_button = tk.Button(self.voice_card, text="Start Voice Control", command=self.start_recognize_thread)
        self.voice_button.pack(side=tk.LEFT, padx=10)


        # Pack the cards
        self.light_card.pack(side=tk.LEFT, padx=10)
        self.fan_card.pack(side=tk.LEFT, padx=10)
        #self.humidity_card.pack(side=tk.LEFT, padx=10)
        #self.temperature_card.pack(side=tk.LEFT, padx=10)
        self.voice_card.pack(side=tk.LEFT, padx=10)



        # Update values initially
        #self.update_values()

    def light_on(self):
        sendRequest(root_url + "/ledon")
        print("Light turned on")

    def light_off(self):
        sendRequest(root_url + "/ledoff")
        print("Light turned off")

    def fan_on(self):
        sendRequest(root_url + "/fanon")
        print("Fan turned on")

    def fan_off(self):
        sendRequest(root_url + "/fanoff")
        print("Fan turned off")

    """def update_values(self):
        try:
            response = requests.get(f"{root_url}/get_data", timeout=10)
            data = response.json()
            temperature_value = data.get("temperature", "N/A")
            humidity_value = data.get("humidity", "N/A")

            self.temperature_value.set(f"Temperature: {temperature_value} °C")
            self.humidity_value.set(f"Humidity: {humidity_value} %")

        except requests.RequestException as e:
            print(f"Error fetching data: {e}")
            if isinstance(e, requests.ConnectionError) and "Connection reset by peer" in str(e):
                # Retry after a brief delay
                print("Retrying...")
                self.master.after(1000, self.update_values)
                return  # Skip scheduling the regular update

            else:
                # Log other exceptions and handle accordingly
                pass

        # Schedule the update every 5000 milliseconds (5 seconds)
        self.master.after(5000, self.update_values)
        """

    def start_recognize_thread(self):
        # Start a new thread for continuous speech recognition
        threading.Thread(target=self.continuous_recognize_speech).start()

    def continuous_recognize_speech(self):
        while True:
            self.recognize_speech()
    
    def recognize_speech(self):
        # Initialize the recognizer
        r = sr.Recognizer()

        # Capture audio from the microphone
        with sr.Microphone() as source:
            r.energy_threshold = 700  #use 700
            r.dynamic_energy_threshold = True
            r.adjust_for_ambient_noise(source, duration=0.5)
            r.pause_threshold = 0.5
            print("Say something:")
            audio = r.listen(source)
            try:
                # Recognize speech using Google Web Speech API
                text = r.recognize_google(audio)
                print(f"You: {text}")
                #self.execute_command(text)
                if "Jarvis" in text or "Hey buddy" in text:
                    speak("Oh! Hello sir")
                if "turn on light" in text or "turn on the light" in text or "light turn on" in text or "turn on light" in text:
                    turn_on_light()
                elif "turn off light" in text or "turn off the light" in text or "light turn off" in text or "turn off light" in text:
                    turn_off_light()
                elif "turn on fan" in text or "turn on the fan" in text or "fan turn on" in text or "turn on fan" in text:
                    turn_on_fan()
                elif "turn off fan" in text or "turn off the fan" in text or "fan turn off" in text or "turn off fan" in text:
                    turn_off_fan()
                elif "check weather" in text or "what's the weather" in text or "check the weather" in text:
                    current_weather()
                elif "shutdown" in text or "shut down" in text:
                    speak("Disabling voice functionality")
                    exit()
                elif "how are you" in text or "how r u" in text:
                    speak("I am fine Sir")
                elif "thank you" in text or "thanks" in text:
                    speak("It's my pleasure Sir")
            except sr.UnknownValueError:
                pass
                #print("Sorry, I could not understand what you said.")
            except sr.RequestError as e:
                print("Error with the speech recognition service; {0}".format(e))



if __name__ == "__main__":
    root = tk.Tk()
    app = GreenHouseGUI(root)
    root.mainloop()