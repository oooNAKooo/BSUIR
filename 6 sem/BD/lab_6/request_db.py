import tkinter as tk
from tkinter import scrolledtext
import psycopg2
from tkinter import messagebox

conn_params = {
    "host": "localhost",
    "port": "5432",
    "database": "postgres",
    "user": "postgres",
    "password": ""
}

class ReqWindow:
    def __init__(self):
        self.window = tk.Toplevel()
        self.window.title("Запрос")

        self.create_widgets()

    def create_widgets(self):
        label = tk.Label(self.window, text="Введите запрос:")
        label.pack()

        self.query_entry = scrolledtext.ScrolledText(self.window, width=50, height=2)
        self.query_entry.pack()

        execute_button = tk.Button(self.window, text="Выполнить запрос", command=self.execute_query)
        execute_button.pack()

    def execute_query(self):
        query = self.query_entry.get("1.0", tk.END)
        try:
            connection = psycopg2.connect(**conn_params)
            cursor = connection.cursor()
            cursor.execute(query)
            records = cursor.fetchall()
            connection.close()

            self.show_result_window(records)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Ошибка при выполнении запроса: {e}")

    def show_result_window(self, records):
        result_window = tk.Toplevel()
        result_window.title("Результат запроса")

        result_text = scrolledtext.ScrolledText(result_window, width=100, height=10)
        result_text.pack()

        for record in records:
            result_text.insert(tk.END, str(record) + "\n")
