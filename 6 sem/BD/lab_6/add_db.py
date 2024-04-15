import tkinter as tk
import psycopg2
from tkinter import messagebox

conn_params = {
    "host": "localhost",
    "port": "5432",
    "database": "postgres",
    "user": "postgres",
    "password": ""
}

class AddWindow:
    def __init__(self, entity):
        self.entity = entity
        self.window = tk.Toplevel()
        self.window.title(f"Добавление записи в {entity}")
        self.fields = {}
        self.create_input_fields()

    def create_input_fields(self):
        connection = psycopg2.connect(**conn_params)
        cursor = connection.cursor()
        cursor.execute("SELECT column_name FROM information_schema.columns WHERE table_name = %s", (self.entity,))
        columns_info = cursor.fetchall()
        connection.close()

        fields_info = {info[0]: info[0] for info in columns_info if "id" not in info[0].lower()}
        for field, label_text in fields_info.items():
            label = tk.Label(self.window, text=label_text)
            label.pack()
            entry = tk.Entry(self.window)
            entry.pack()
            self.fields[field] = entry

        button = tk.Button(self.window, text="Добавить запись", command=self.add_record_to_database)
        button.pack()

    def add_record_to_database(self):
        values = {field: entry.get() for field, entry in self.fields.items()}
        try:
            connection = psycopg2.connect(**conn_params)
            cursor = connection.cursor()
            cursor.execute(f"SELECT MAX(id_{self.entity}а) FROM {self.entity}")
            max_id = cursor.fetchone()[0]
            next_id = max_id + 1 if max_id is not None else 1
            values[f"id_{self.entity}а"] = next_id
            columns = ', '.join(values.keys())
            placeholders = ', '.join(['%s' for _ in values])
            sql = f"INSERT INTO {self.entity} ({columns}) VALUES ({placeholders})"
            cursor.execute(sql, tuple(values.values()))
            connection.commit()
            connection.close()
            messagebox.showinfo("Успех", "Запись успешно добавлена в базу данных!")
            self.window.destroy()
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось добавить запись: {e}")
