

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <Windows.h>
using namespace std;

// Класс Product представляет товар, доступный в магазине;

class Product 
{
private:
    string name; // Название товара
    double price;     // Цена товара
    int stock;        // Количество товара на складе

public:
    // Конструктор для инициализации товара
    Product(const string& name, double price, int stock) : name(name), price(price), stock(stock) {}

    // Метод для увеличения количества на складе
    void increaseStock(int amount)
    {
        stock += amount;
    }

    // Метод для уменьшения количества на складе
    void decreaseStock(int amount) 
    {
        if (amount <= stock)
            stock -= amount;
    }

    // Метод для отображения информации о товаре
    void displayInfo() const 
    {
        cout << "Товар: " << name << ", Цена: " << price << " руб., Остаток: " << stock << " шт." << endl;
    }

    // Перегрузка оператора == для сравнения товаров по названию
    bool operator==(const Product& other) const 
    {
        return name == other.name;
    }

    // Перегрузка оператора += для увеличения количества на складе
    Product& operator+=(int amount) 
    {
        increaseStock(amount);
        return *this;
    }

    // Перегрузка оператора -= для уменьшения количества на складе
    Product& operator-=(int amount) 
    {
        decreaseStock(amount);
        return *this;
    }

    // Геттеры для получения данных о товаре
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    const string& getName() const { return name; }
};

// Предварительное объявление класса Store для использования в классе Customer
class Store;

// Класс Customer представляет клиента магазина
class Customer 
{
private:
    string name;   // Имя клиента
    double balance; // Баланс клиента

public:
    // Конструктор для инициализации клиента
    Customer(const string& name, double balance) : name(name), balance(balance) {}

    // Метод для пополнения счета клиента
    void addFunds(double amount) 
    {
        balance += amount;
    }

    // Метод для покупки товара; возвращает true, если покупка успешна
    bool buyProduct(Product& product, int quantity)
    {
        double cost = product.getPrice() * quantity; // Стоимость покупки
        if (balance >= cost && product.getStock() >= quantity) 
        {
            balance -= cost;     // Списываем деньги с баланса клиента
            product -= quantity; // Уменьшаем количество товара
            return true;
        }
        return false;
    }

    // Геттеры для доступа к имени и балансу клиента
    double getBalance() const { return balance; }
    const string& getName() const { return name; }

    // Дружественный класс Store может использовать закрытые данные Customer
    friend class Store;
};

// Класс Store представляет магазин
class Store 
{
private:
    vector<Product> products;    // Список товаров в магазине
    vector<Customer> customers;  // Список клиентов магазина
    static int productCount;     // Статическое поле для подсчета количества товаров

public:
    // Конструктор по умолчанию
    Store() {}

    // Метод для добавления товара в магазин
    void addProduct(const Product& product) 
    {
        products.push_back(product);
        productCount++;
    }

    // Метод для добавления клиента в магазин
    void addCustomer(const Customer& customer) 
    {
        customers.push_back(customer);
    }

    // Метод для обработки покупки товара клиентом
    void purchaseProduct(const string& customerName, const string& productName, int quantity) 
    {
        for (auto& customer : customers)// Ищем клиента по имени
        { 
            if (customer.getName() == customerName) 
            {
                for (auto& product : products) // Ищем товар по названию
                { 
                    if (product.getName() == productName) 
                    {
                        if (customer.buyProduct(product, quantity)) 
                        {
                            cout << "Покупка успешна!" << endl;
                        }
                        else 
                        {
                            cout << "Покупка не удалась." << endl;
                        }
                        return;
                    }
                }
            }
        }
    }

    // Шаблонный метод для вывода информации о товарах или клиентах
    template <typename T>
    void displayInfo() const 
    {
        if (typeid(T) == typeid(Product)) // Если тип - Product, выводим информацию о товарах
        { 
            for (const auto& product : products)
                product.displayInfo();
        }
        else if (typeid(T) == typeid(Customer))// Если тип - Customer, выводим информацию о клиентах 
        { 
            for (const auto& customer : customers) 
            {
                cout << "Клиент: " << customer.getName() << ", Баланс: " << customer.getBalance() << " руб." << endl;
            }
        }
    }

    // Статический метод для получения количества товаров в магазине
    static int getProductCount() 
    {
        return productCount;
    }
};

// Инициализация статического поля productCount
int Store::productCount = 0;

// Шаблонный класс Employee для представления сотрудника
template <typename T>
class Employee 
{
private:
    T id;           // Идентификатор сотрудника (шаблонный тип)
    string name;    // Имя сотрудника
    double salary;  // Зарплата сотрудника
    bool isActive;  // Статус активности сотрудника

public:
    // Конструктор для инициализации сотрудника
    Employee(T id, const string& name, double salary) : id(id), name(name), salary(salary), isActive(true) {}

    // Метод для отображения информации о сотруднике
    void displayInfo() const 
    {
        cout << "Сотрудник ID: " << id << ", Имя: " << name << ", Зарплата: " << salary << ", Статус: " << (isActive ? "Активен" : "Неактивен") << endl;
    }

    // Методы для изменения статуса и зарплаты сотрудника
    void deactivate() { isActive = false; }
    void activate() { isActive = true; }
    void setSalary(double newSalary) { salary = newSalary; }
};

int main()
{
    // Установка кодировки консоли для корректного вывода текста на русском языке
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Создаем магазин
    Store store;

    // Добавляем товары в магазин
    store.addProduct(Product("Хлеб", 50, 100));
    store.addProduct(Product("Молоко", 60, 100));
    store.addProduct(Product("Мясо", 150, 50));
    store.addProduct(Product("Яйца", 110, 31));
    store.addProduct(Product("Рис", 78, 50));
    store.addProduct(Product("Макароны", 69, 53));
    store.addProduct(Product("Тушенка", 350, 87));
    store.addProduct(Product("Кабачковая икра", 135, 48));
    store.addProduct(Product("Фасоль в томатном соусе", 169, 31));

    // Создаем клиента
    Customer customer("Иван Иванов", 1000);
    store.addCustomer(customer);

    // Клиент совершает покупку товара
    store.purchaseProduct("Иван Иванов", "Хлеб", 1);
    store.purchaseProduct("Иван Иванов", "Макароны", 3);
    store.purchaseProduct("Иван Иванов", "Фасоль в томатном соусе", 2);
    store.purchaseProduct("Иван Иванов", "Молоко", 2);

    // Вывод информации о товарах
    cout << "Информация о товарах:" << endl;
    store.displayInfo<Product>();

    cout << endl << endl << endl << endl;

    // Вывод информации о клиентах
    cout << "Информация о клиентах:" << endl;
    store.displayInfo<Customer>();

    cout << endl << endl << endl << endl;

    // Пример использования класса Employee для сотрудников
    Employee<int> employee1(1, "Анна Петрова", 50000);
    employee1.displayInfo();
    employee1.deactivate(); // Изменение статуса сотрудника на "Неактивен"
    employee1.displayInfo();

    return 0;
}