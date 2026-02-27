# This is a sample Python script.

# Press Ctrl+F5 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

import tkinter as tk

window = tk.Tk()
window.title("Движущаяся кнопка")
window.geometry("620x420")

current_x = 50
current_y = 50

n_x = 1
n_y = 1
def move_button():
    global current_x, current_y, n_x, n_y

    current_x += 10 * n_x
    current_y += 10 * n_y

    if current_x + 100 >= 600:
        n_x = -1
        current_x = 600 - 100

    if current_x <= 0:
        n_x = 1
        current_x = 0

    if current_y + 40 >= 400:
        n_y = -1
        current_y = 400 - 40

    if current_y <= 0:
        n_y = 1
        current_y = 0
    button.place(x=current_x, y=current_y)
    print(f"X: {current_x}, Y: {current_y}, Направление: {'вправо' if n_x == 1 else 'влево'}")


button = tk.Button(
    window,
    text="BUTTON",
    command=move_button,
    font=("Arial", 14),
    bg="yellow",
    width=10,
    height=2
)

button.place(x=current_x, y=current_y)

window.mainloop()