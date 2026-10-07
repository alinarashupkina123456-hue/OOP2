#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <windows.h> // Потрібно для налаштування кодування консолі

// ============================================================================
// ПУНКТ 1 & 2: Клас Book (8 полів: 3 public, 5 private)
// ============================================================================
class Book {
public:
    // Пункт 2: 3 загальні (public) елементи
    std::string title;
    std::string author;
    int year;

    // Конструктори
    Book()
        : title("Невідомо"),
        author("Невідомо"),
        year(0),
        pages_count_(0),
        isbn_("000-0"),
        price_(0.0),
        is_available_(true),
        available_copies_(0) {
    }

    Book(std::string t, std::string a, int y, int pages, std::string isbn,
        double price, bool available, int copies)
        : title(t),
        author(a),
        year(y),
        pages_count_(pages),
        isbn_(isbn),
        price_(price),
        is_available_(available),
        available_copies_(copies) {
    }

    // Пункт 2 & 4: Відображення використання усіх полів у методах
    void displayInfo() const {
        std::cout << "Книга: " << title << " | Автор: " << author << " | Рік: " << year
            << " | Стор: " << pages_count_ << " | ISBN: " << isbn_
            << " | Ціна: " << price_ << " грн | Доступність: "
            << (is_available_ ? "Так" : "Ні")
            << " | Копій: " << available_copies_ << "\n";
    }

    void markAsBorrowed() {
        if (available_copies_ > 0) {
            available_copies_--;
            if (available_copies_ == 0) {
                is_available_ = false;
            }
        }
    }

    void markAsReturned() {
        available_copies_++;
        is_available_ = true;
    }

    // Пункт 4: Перевантажений метод, який приймає об'єкт класу
    bool isSameAuthor(const Book& other) const {
        return this->author == other.author;
    }

    // Пункт 4: Перевантажений метод, який повертає тип класу
    Book createUpdatedEdition(int new_year) const {
        Book updated = *this;
        updated.year = new_year;
        return updated;
    }

    // Пункт 5: Методи запису/читання з файлу
    void saveToFile(std::ofstream& fout) const {
        fout << title << "\n"
            << author << "\n"
            << year << "\n"
            << pages_count_ << "\n"
            << isbn_ << "\n"
            << price_ << "\n"
            << is_available_ << "\n"
            << available_copies_ << "\n";
    }

    void loadFromFile(std::ifstream& fin) {
        fin >> title >> author >> year >> pages_count_ >> isbn_ >> price_
            >> is_available_ >> available_copies_;
    }

    // Пункт 12: Додатковий метод (динамічна пам'ять + сортування)
    void processRatings() {
        int size = rand() % 5 + 5;     // Випадковий розмір (5-9)
        int* ratings = new int[size];  // Виділення динамічної пам'яті

        for (int i = 0; i < size; ++i) {
            ratings[i] = rand() % 100;
        }

        std::sort(ratings, ratings + size);  // Сортування масиву

        std::cout << "Відсортовані рейтинги книги '" << title << "': ";
        for (int i = 0; i < size; ++i) {
            std::cout << ratings[i] << " ";
        }
        std::cout << "\n";

        delete[] ratings;  // Звільнення пам'яті
    }

    // Геттери
    bool isAvailable() const { return is_available_; }

private:
    // Пункт 2: 5 приватних (private) елементів
    int pages_count_;
    std::string isbn_;
    double price_;
    bool is_available_;
    int available_copies_;
};

// ============================================================================
// ПУНКТ 1, 2 & 3: Клас Reader (містить об'єкт Book без дружніх функцій)
// ============================================================================
class Reader {
public:
    // Пункт 2: 3 загальні (public) елементи
    std::string full_name;
    int ticket_number;
    std::string register_date;

    // Пункт 3: Зв'язок двох об'єктів шляхом розміщення об'єкта Book в Reader
    Book borrowed_book;

    // Конструктори
    Reader()
        : full_name("Невідомо"),
        ticket_number(0),
        register_date("01.01.2024"),
        age_(18),
        phone_number_("000"),
        is_blacklisted_(false),
        borrowed_books_count_(0),
        violation_count_(0),
        borrowed_book() {
    }

    Reader(std::string name, int ticket, std::string reg_date, int age,
        std::string phone, bool in_black_list, int borrowed_count,
        Book current_book)
        : full_name(name),
        ticket_number(ticket),
        register_date(reg_date),
        borrowed_book(current_book),
        age_(age),
        phone_number_(phone),
        is_blacklisted_(in_black_list),
        borrowed_books_count_(borrowed_count),
        violation_count_(0) {
    }

    // Пункт 2 & 4: Методи опрацювання даних
    void displayReaderInfo() const {
        std::cout << "Читач: " << full_name << " | Квиток №: " << ticket_number
            << " | Дата: " << register_date << " | Вік: " << age_
            << " | Тел: " << phone_number_ << " | В чорному списку: "
            << (is_blacklisted_ ? "Так" : "Ні")
            << " | Взято книг: " << borrowed_books_count_
            << " | Порушень: " << violation_count_ << "\n";
    }

    // Пункт 10: Сценарій взаємодії двох об'єктів (Читач бере Книгу)
    void borrowBook(Book& book) {
        if (is_blacklisted_) {
            std::cout << "-> Помилка: Читач " << full_name
                << " у чорному списку! Книгу видати неможливо.\n";
            return;
        }
        if (book.isAvailable()) {
            borrowed_book = book;
            book.markAsBorrowed();
            borrowed_books_count_++;
            std::cout << "-> Успіх: Читач " << full_name
                << " взяв книгу '" << book.title << "'\n";
        }
        else {
            std::cout << "-> Помилка: Книга '" << book.title << "' недоступна.\n";
        }
    }

    void returnBook() {
        if (borrowed_books_count_ > 0) {
            borrowed_book.markAsReturned();
            std::cout << "-> Читач " << full_name << " повернув книгу.\n";
        }
    }

    // Пункт 4: Перевантажений метод (приймає об'єкт класу)
    void syncStatusWith(const Reader& other) {
        this->is_blacklisted_ = other.is_blacklisted_;
    }

    // Пункт 4: Перевантажений метод (повертає тип класу)
    Reader cloneWithNewTicket(int new_ticket) const {
        Reader copy = *this;
        copy.ticket_number = new_ticket;
        return copy;
    }

    // Пункт 5: Запис і читання з файлу
    void saveToFile(std::ofstream& fout) const {
        fout << full_name << "\n"
            << ticket_number << "\n"
            << register_date << "\n"
            << age_ << "\n"
            << phone_number_ << "\n"
            << is_blacklisted_ << "\n"
            << borrowed_books_count_ << "\n"
            << violation_count_ << "\n";
    }

    void loadFromFile(std::ifstream& fin) {
        fin >> full_name >> ticket_number >> register_date >> age_ >> phone_number_
            >> is_blacklisted_ >> borrowed_books_count_ >> violation_count_;
    }

private:
    // Пункт 2: 5 приватних (private) елементів
    int age_;
    std::string phone_number_;
    bool is_blacklisted_;
    int borrowed_books_count_;
    int violation_count_;
};

// ============================================================================
// ГОЛОВНА ПРОГРАМА
// ============================================================================
int main() {
    // Налаштування українізації виводу у консолі Windows (CP1251 або Windows-1251)
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::srand(12345);

    std::cout << "========================================================\n";
    std::cout << "Пункт 6: 5 об'єктів у статичній та 5 у динамічній пам'яті\n";
    std::cout << "========================================================\n";

    // Пункт 6: 5 об'єктів у статичній пам'яті
    Book static_books[5] = {
        Book("Кобзар", "Т.Шевченко", 1840, 350, "978-1", 250.0, true, 3),
        Book("Тіні забутих предків", "М.Коцюбинський", 1911, 200, "978-2", 180.0, true, 2),
        Book("Місто", "В.Підмогильний", 1928, 300, "978-3", 210.0, true, 5),
        Book("Захар Беркут", "І.Франко", 1883, 240, "978-4", 190.0, true, 1),
        Book("Кайдашева сім'я", "І.Нечуй-Левицький", 1878, 180, "978-5", 150.0, true, 4)
    };

    // Пункт 6: 5 об'єктів у динамічній пам'яті
    Book* dynamic_books[5];
    for (int i = 0; i < 5; ++i) {
        dynamic_books[i] = new Book("Динамічна_Книга_" + std::to_string(i + 1),
            "Автор", 2020 + i, 100 + i * 10, "978-D",
            100.0 + i, true, 2);
    }

    std::cout << "\n========================================================\n";
    std::cout << "Пункт 7 & 8: 2 масиви об'єктів та демонстрація їх роботи\n";
    std::cout << "========================================================\n";

    // Пункт 7: Оголошення 2 масивів об'єктів класу Reader
    Reader readers_array_1[3] = {
        Reader("Іван_Петренко", 101, "01.09.2023", 19, "0971112233", false, 0, static_books[0]),
        Reader("Олена_Бойко", 102, "15.10.2023", 20, "0632223344", false, 0, static_books[1]),
        Reader("Максим_Коваль", 103, "01.02.2024", 18, "0503334455", true, 0, static_books[2])
    };

    Reader readers_array_2[2] = {
        Reader("Анна_Сидоренко", 104, "10.03.2024", 21, "0984445566", false, 0, static_books[3]),
        Reader("Денис_Ткаченко", 105, "05.04.2024", 22, "0735556677", false, 0, static_books[4])
    };

    // Пункт 8: Демонстрація роботи з об'єктами в масивах
    std::cout << "Перший масив читачів:\n";
    for (int i = 0; i < 3; ++i) {
        readers_array_1[i].displayReaderInfo();
    }

    std::cout << "\nДругий масив читачів:\n";
    for (int i = 0; i < 2; ++i) {
        readers_array_2[i].displayReaderInfo();
    }

    std::cout << "\n========================================================\n";
    std::cout << "Пункт 9: Демонстрація роботи усіх методів та перевантажень\n";
    std::cout << "========================================================\n";

    static_books[0].displayInfo();

    // Перевірка перевантажених методів класу Book
    std::cout << "Чи один автор? "
        << (static_books[0].isSameAuthor(static_books[1]) ? "Так" : "Ні") << "\n";

    Book updated = static_books[0].createUpdatedEdition(2026);
    std::cout << "Створено нове видання з роком 2026: ";
    updated.displayInfo();

    std::cout << "\n========================================================\n";
    std::cout << "Пункт 10: Сценарій взаємодії (Читач замовляє/бере Книгу)\n";
    std::cout << "========================================================\n";

    // Сценарій 1: Успішна видача книги
    readers_array_1[0].borrowBook(static_books[0]);

    // Сценарій 2: Спроба взяти книгу читачем з чорного списку
    readers_array_1[2].borrowBook(static_books[1]);

    std::cout << "\n========================================================\n";
    std::cout << "Пункт 11: Використання покажчика на екземпляр класу\n";
    std::cout << "========================================================\n";

    Book* book_ptr = &static_books[2];  // Покажчик на екземпляр
    book_ptr->displayInfo();            // Виклик через стрілочний оператор

    std::cout << "\n========================================================\n";
    std::cout << "Пункт 12: Метод з динамічною пам'яттю та сортуванням\n";
    std::cout << "========================================================\n";

    static_books[0].processRatings();

    std::cout << "\n========================================================\n";
    std::cout << "Пункт 5: Запис у файл та читання з файлу\n";
    std::cout << "========================================================\n";

    // Запис у файл
    std::ofstream fout("book_data.txt");
    if (fout.is_open()) {
        static_books[0].saveToFile(fout);
        fout.close();
        std::cout << "Книгу '" << static_books[0].title << "' успішно збережено у файл.\n";
    }

    // Читання з файлу
    std::ifstream fin("book_data.txt");
    if (fin.is_open()) {
        Book loaded_book;
        loaded_book.loadFromFile(fin);
        fin.close();
        std::cout << "Прочитані дані з файлу: ";
        loaded_book.displayInfo();
    }

    // Очищення динамічної пам'яті для 5 об'єктів з пункту 6
    for (int i = 0; i < 5; ++i) {
        delete dynamic_books[i];
    }

    return 0;
}