import threading
import queue
import tkinter as tk
import pygame
import serial
from tkinter import font


class FullscreenNoteDisplay:
    def __init__(self, mode: int):
        self.mode = mode
        self.sound_path = "Sounds/"
        self.modes = ["Piano/", "Bell/"]
        self.notes = ["do", "re", "mi", "fa", "sol", "la", "si"]
        self.sound_extension = ".wav"

        self.queue = queue.Queue()

        # initialize pygame mixer
        pygame.mixer.init()

        # create the GUI
        self.root = tk.Tk()
        self.root.attributes('-fullscreen', True)
        self.root.configure(bg='black')
        self.label = tk.Label(self.root, text='', font=font.Font(size=500, weight='bold'), fg='white', bg='black')
        self.label.pack(expand=True)

        # start a new thread to run start_notes
        self.notes_thread = threading.Thread(target=self.start_notes)
        self.notes_thread.daemon = True
        self.notes_thread.start()

    def start_notes(self):
        arduino = serial.Serial("COM4", 9600)
        while True:
            arduino_data = arduino.readline()
            decoded_value = str(arduino_data.decode("utf-8")).rstrip()

            if decoded_value.isdigit() and int(decoded_value) <= len(self.notes):
                # play the note sound
                
                note_name = self.notes[int(decoded_value) - 1]
                note_path = self.sound_path + self.modes[self.mode] + note_name + self.sound_extension
                note_sound = pygame.mixer.Sound(note_path)
                note_sound.play()

                # update the label text
                self.queue.put(note_name.capitalize())

    def update_gui(self):
        # update the label with the latest note
        while not self.queue.empty():
            note_name = self.queue.get()
            self.label.config(text=note_name)

            # schedule the label to return to blank after 1 second
            self.root.after(1000, lambda: self.label.config(text=''))

        # schedule the next update
        self.root.after(50, self.update_gui)

    def run(self):
        self.update_gui()
        self.root.mainloop()


if __name__ == "__main__":
    gui = FullscreenNoteDisplay(mode=0)
    gui.run()
