import tkinter as tk
import psycopg2
from tkinter import messagebox

# Параметры подключения к базе данных PostgreSQL
conn_params = {
    "host": "localhost",
    "port": "5432",
    "database": "postgres",
    "user": "postgres",
    "password": ""
}

class DelWindow:
    def __init__(self, entity):
        self.entity = entity
        self.delete_window = tk.Toplevel()
        self.delete_window.title("Выберите запись для удаления")

        try:
            # Получаем доступные ID записей из базы данных
            conn = psycopg2.connect(**conn_params)
            cur = conn.cursor()

            # Формируем название столбца ID в соответствии с таблицей
            id_column = f"id_{entity.lower()}а"
            cur.execute(f"SELECT {id_column} FROM {entity}")
            record_ids = [row[0] for row in cur.fetchall()]
            conn.close()

            if not record_ids:
                messagebox.showerror("Ошибка", "Нет доступных записей для удаления.")
                self.delete_window.destroy()
                return

            # Создаем метку с инструкцией
            label = tk.Label(self.delete_window, text="Выберите ID записи для удаления:")
            label.pack()

            # Устанавливаем начальное значение для выбранного ID
            self.selected_id = tk.StringVar(self.delete_window)
            self.selected_id.set(record_ids[0])

            # Создаем выпадающее меню для выбора ID
            id_menu = tk.OptionMenu(self.delete_window, self.selected_id, *record_ids)
            id_menu.pack()

            # Создаем кнопку для удаления записи
            delete_button = tk.Button(self.delete_window, text="Удалить", command=self.delete_selected_record)
            delete_button.pack()
        except Exception as e:
            messagebox.showerror("Ошибка", f"Ошибка при выполнении запроса: {e}")

    def delete_selected_record(self):
        try:
            selected_record_id = self.selected_id.get()
            if not selected_record_id:
                raise ValueError("Не выбран ID записи")

            # Удаляем запись из базы данных
            conn = psycopg2.connect(**conn_params)
            cur = conn.cursor()
            id_column = f"id_{self.entity.lower()}а"
            cur.execute(f"DELETE FROM {self.entity} WHERE {id_column} = %s", (selected_record_id,))
            conn.commit()
            conn.close()

            messagebox.showinfo("Успех", "Запись успешно удалена из базы данных!")
            self.delete_window.destroy()
        except ValueError as ve:
            messagebox.showerror("Ошибка", str(ve))
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось удалить запись: {e}")
