#ifndef CONTAINERS_S21_ARRAY_H
#define CONTAINERS_S21_ARRAY_H

#include <initializer_list>

// Первая строка определяет пространство имен (namespace) с именем "s21"
namespace s21 {
template <typename T, std::size_t V>
// Далее объявляется шаблонный класс с именем "array". Он принимает два 
// параметра - тип данных элементов массива (T) и некоторый размер (V), который должен быть
// представлен как константное выражение std::size_t
class array {
 public:
 // Затем объявлены несколько типов, используемых в классе: value_type, 
 // reference, const_reference, iterator, const_iterator и size_type. 
 // Эти типы определены через используемые типы (using), чтобы указать на тип 
 // данных элементов, ссылки на элементы, указатели на элементы и размер
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = std::size_t;

// В приватной секции класса объявляется массив с именем data_, который 
// может хранить V элементов типа T. Здесь "value_type" обозначает тип T, 
// который был указан ранее
 private:
  value_type data_[V];

// В публичной секции класса определен конструктор по умолчанию, который 
// вызывается, если пользователь не предоставляет аргументов во время создания 
// экземпляра класса. Он помечен как noexcept, что позволяет компилятору оптимизировать 
// код при работе с этим конструктором. "default" указывает на использование значения по 
// умолчанию для конструктора
 public:
  array() noexcept = default;

  // Ключевое слово `explicit` указывает на явное использование конструктора, то есть он должен быть вызван явно 
  // при создании объекта, а не неявно при передаче аргументов функции
  // Объявление конструктора класса `array`, который принимает список инициализации типа `value_type`
  explicit array(std::initializer_list<value_type> const &items) {
  // проверка, что количество элементов в списке инициализации не превышает размер массива `V`. 
  // Если превышает, то выбрасывается исключение типа `std::logic_error` с сообщением "too many initializers"
    if (items.size() > V) {
      throw std::logic_error("too many initializers");
    }
  // Заполнение массива `data_` элементами из списка инициализации `items`. 
  // Используется цикл от 0 до `V`, который поочередно присваивает элементам массива 
  // значения из списка инициализации, используя оператор индексации `[]`
    for (size_type i = 0; i < V; ++i) {
      data_[i] = items.begin()[i];
    }
  }
  
  // Данный фрагмент кода определяет явный конструктор копирования класса `array`, 
  // который принимает в качестве параметра другой объект класса `array`
  // Ключевое слово `explicit` указывает на явное использование конструктора, то 
  // есть он должен быть вызван явно при создании объекта, а не неявно при передаче аргументов 
  // функции
  explicit array(const array &a) {
  // Цикл используется для копирования элементов массива `a` в массив `data_` текущего объекта.
  // Цикл идет до `V`, что гарантирует, что все элементы массива будут скопированы. Копирование 
  // происходит по индексу `i`, используя оператор доступа по индексу `[]`
    for (size_type i = 0; i < V; ++i) {
      data_[i] = a[i];
    }
  }

  explicit array(array &&a) noexcept {
    auto iter = data_;
    for (auto &item : a) {
      *(iter++) = std::move(item);
    }
  }

  array &operator=(array &&a) noexcept {
    for (size_type i = 0; i < V; ++i) {
      data_[i] = std::move(a[i]);
    }
    return *this;
  };

  ~array() = default;

  reference at(size_type pos) {
    if (pos >= V) {
      throw std::out_of_range("Out of range");
    }
    return data_[pos];
  }

  constexpr reference operator[](size_type pos) { return data_[pos]; }

  constexpr const_reference operator[](size_type pos) const {
    return data_[pos];
  }

  constexpr const_reference front() const { return *data_; };

  constexpr const_reference back() const { return data_[V - 1]; };

  constexpr iterator data() noexcept { return data_; };

  constexpr iterator begin() noexcept { return data_; };

  constexpr iterator end() noexcept { return data_ + V; };

  constexpr bool empty() const noexcept { return V == 0; };

  constexpr size_type size() const noexcept { return V; };

  constexpr size_type max_size() const noexcept { return V; };

  constexpr void swap(array &other) noexcept {
    for (size_type i = 0; i < V; ++i) {
      std::swap(data_[i], other.data_[i]);
    }
  }

  void fill(const_reference value) {
    for (auto &item : data_) {
      item = value;
    }
  }
};

}  // namespace s21

#endif  // CONTAINERS_S21_ARRAY_H