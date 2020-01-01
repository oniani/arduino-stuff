#!/usr/bin/env python3

"""
Authors: David Oniani, Thomas Twiton, Mason Donnohue
Created: May 12, 2019

Dependencies: PyAudio, PySpeech, PySerial
"""

import time

import serial

import speech_recognition as sr


def main() -> None:
    """The main function."""
    # Record Audio
    recorder = sr.Recognizer()

    # Recognized word
    recognized_word = "nothing!"

    with sr.Microphone() as source:
        print("\n")
        print("==============================================================")
        print("Say Something!")
        audio = recorder.listen(source, phrase_time_limit=3)

    # Speech recognition using Google speech recognition
    try:
        # for testing purposes, we're just using the default API key
        # to use another API key, use
        # 'r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")'
        # instead of 'r.recognize_google(audio)`
        recognized_word = recorder.recognize_google(audio)

    except sr.UnknownValueError:
        print("Google Speech Recognition could not understand audio")

    except sr.RequestError as req_e:
        print(f"Cannot get results from GSR service, {req_e}")

    # Additional phrases are provided due to the microphone not being
    # precise in voice recognition
    wake_up_on = {
        "hello raspberry",
        "hella raspberry",
        "heather raspberry",
        "hellraiser",
    }

    if recognized_word == "nothing!":
        print("You said {}".format(recognized_word))

    elif recognized_word in wake_up_on:
        print("Raspberry has woken up!")

        colors = {
            "red": "r",
            "green": "g",
            "blue": "b",
            "yellow": "y",
            "purple": "p",
            "clear": "0",
        }
        color = "nothing!"
        run = True

        while run:
            print("What color would you like?")

            with sr.Microphone() as source:
                color_audio = recorder.listen(source, phrase_time_limit=2.5)

            try:
                color = recorder.recognize_google(color_audio)

            except sr.UnknownValueError:
                print("Google Speech Recognition could not understand audio")

            except sr.RequestError as req_e:
                print(f"Cannot get results from GSR service, {req_e}")

            # If we got the right color, change the color of the bulb
            if color in colors:
                print("The color you said is {}!".format(color))

                serial_port = "/dev/ttyACM0"
                baud_rate = 9600
                ser = serial.Serial(serial_port, baud_rate)
                ser.flushInput()
                time.sleep(3)
                ser.write(str.encode(colors[color]))

            elif color == "stop":
                run = False
                print("The program has stopped running!")

    else:
        print(f"Command {recognized_word} is unknown!")
