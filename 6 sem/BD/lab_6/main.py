import tkinter as tk
from view_db import ViewWindow
from add_db import AddWindow
from delete_db import DelWindow
from request_db import ReqWindow


def create_operation_window(entity):
    operation_window = tk.Toplevel()
    operation_window.title(entity)
    operation_window.geometry("400x300")
    operation_window.configure(bg="burlywood")

    button_frame = tk.Frame(operation_window, bg="burlywood")
    button_frame.pack(pady=30)

    operations = ["Добавить", "Просмотреть", "Удалить"]
    for operation in operations:
        button = tk.Button(button_frame, text=operation, font=("Arial", 18), width=20, height=1)
        if operation == "Добавить":
            button.config(command=lambda: AddWindow(entity.lower()), bg="green")
        elif operation == "Просмотреть":
            button.config(command=lambda: ViewWindow(entity.lower()), bg="yellow")
        elif operation == "Удалить":
            button.config(command=lambda: DelWindow(entity.lower()), bg="red")
        button.pack(pady=10)


root = tk.Tk()
root.title("Lab_6")
root.geometry("1366x768")
root.configure(bg="burlywood")

label = tk.Label(root, text="DATABASE", font=("Arial", 120), pady=30, bg="burlywood")
label.pack()

button_frame = tk.Frame(root, bg="burlywood")
button_frame.pack()

entities = ["Сотрудник", "Товар", "Покупатель", "Магазин", "Импортер", "Склад", "Выход", "Логистика", "Запрос"]
positions = [(0, 0), (0, 1), (0, 2),
             (1, 0), (1, 1), (1, 2),
             (2, 0), (2, 1), (2, 2)]

for entity, pos in zip(entities, positions):
    if entity == "Выход":
        button = tk.Button(button_frame, text=entity, font=("Arial", 18), width=20, height=2, command=root.quit,
                           bg="red")
    elif entity == "Запрос":
        button = tk.Button(button_frame, text=entity, font=("Arial", 18), width=20, height=2,
                           command=lambda: ReqWindow(), bg="blue")
    else:
        button = tk.Button(button_frame, text=entity, font=("Arial", 18), width=20, height=2,
                           command=lambda e=entity: create_operation_window(e), bg="green")
    button.grid(row=pos[0], column=pos[1], padx=10, pady=10)

root.mainloop()
