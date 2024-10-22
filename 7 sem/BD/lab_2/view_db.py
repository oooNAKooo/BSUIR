import tkinter as tk
from tkinter import ttk, messagebox
import psycopg2

class ViewWindow(tk.Toplevel):
    def __init__(self, entity):
        super().__init__()
        self.entity = entity
        self.title(f"Просмотр таблицы {entity}")

        self.tree = ttk.Treeview(self)
        self.tree.pack(expand=True, fill=tk.BOTH)

        scrollbar = ttk.Scrollbar(self, orient="vertical", command=self.tree.yview)
        scrollbar.pack(side="right", fill="y")

        self.tree.configure(yscrollcommand=scrollbar.set)

        self.get_data(entity)

        clear_button = tk.Button(self, text="Очистить", command=lambda: self.clear_table(entity))
        clear_button.pack(pady=10)

    def get_data(self, entity):
        try:
            conn = psycopg2.connect(
                host="localhost",
                port="5432",
                database="postgres",
                user="postgres",
                password=""
            )
            cursor = conn.cursor()

            cursor.execute(f"SELECT * FROM {entity}")

            columns = [desc[0] for desc in cursor.description]

            self.tree["columns"] = columns
            self.tree.heading("#0", text=entity)
            for col in columns:
                self.tree.heading(col, text=col)
                self.tree.column(col, anchor=tk.CENTER)

            self.tree.delete(*self.tree.get_children())

            for row in cursor.fetchall():
                self.tree.insert("", "end", values=row)

            cursor.close()
            conn.close()

        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить данные из таблицы: {e}")

    def clear_table(self, entity):
        try:
            conn = psycopg2.connect(
                host="localhost",
                port="5432",
                database="postgres",
                user="postgres",
                password=""
            )
            cursor = conn.cursor()

            cursor.execute(f"DELETE FROM {entity}")

            conn.commit()

            self.tree.delete(*self.tree.get_children())

            messagebox.showinfo("Успех", f"Все записи из таблицы {entity} успешно удалены!")

        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось очистить таблицу: {e}")

        finally:
            cursor.close()
            conn.close()
