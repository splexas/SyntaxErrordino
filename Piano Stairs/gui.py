import threading
import queue
import tkinter as tk
import pygame
import serial

class ArduinoStairNotesGUI:
    def __init__(self, mode: int):
        self.mode = mode
        self.sound_path = "Sounds/"
        self.modes = ["Piano/", "Bell/"]
        self.notes = ["do", "re", "mi", "fa", "sol", "la", "si"]
        self.sound_extension = ".wav"

        self.note_queue = queue.Queue()

        self.create_gui()

        # initialize pygame mixer
        pygame.mixer.init()

        # start a new thread to run start_notes
        self.notes_thread = threading.Thread(target=self.start_notes)
        self.notes_thread.daemon = True
        self.notes_thread.start()

    def create_gui(self):
        self.root = tk.Tk()
        self.root.title("Arduino Stair Notes")

        # create labels for note names
        for note in self.notes:
            note_label = tk.Label(self.root, text=note.capitalize(), foreground='black', font=("Arial", 40))
            note_label.pack()

    def start_notes(self):
        arduino = serial.Serial("COM7", 9600)
        while True:
            arduino_data = arduino.readline()
            decoded_value = str(arduino_data.decode("utf-8")).rstrip()

            if decoded_value.isdigit() and int(decoded_value) <= len(
                self.notes
            ):  # checks to see if incoming notes are appropriate to play
                print("Incoming note index:", decoded_value)
                note_name = self.notes[int(decoded_value) - 1]
                note_path = (
                    self.sound_path
                    + self.modes[self.mode]
                    + note_name
                    + self.sound_extension
                )  # substracting it since notes start from 1
                note_sound = pygame.mixer.Sound(note_path)
                note_sound.play()

                self.note_queue.put(note_name.capitalize())

    def update_gui(self):
        # update the labels with the latest note
        while not self.note_queue.empty():
            note_name = self.note_queue.get()
            note_labels = self.root.pack_slaves()
            for note_label in note_labels:
                if note_label["text"] == note_name:
                    note_label.config(foreground="red", font=("Arial", 50, "bold"))
                    self.root.after(
                        100,
                        lambda: note_label.config(
                            foreground="black", font=("Arial", 40)
                        ),
                    )

        # schedule the next update
        self.root.after(50, self.update_gui)

    def run(self):
        self.update_gui()
        self.root.mainloop()


if __name__ == "__main__":
    gui = ArduinoStairNotesGUI(mode=0)
    gui.run()
