#ifndef CONTAINERS_S21_LIST_H
#define CONTAINERS_S21_LIST_H

namespace s21 {
template <typename Type>
class list {
 private:
  class ListNode;

 public:
  class ListIterator;
  class ListIteratorConst;
  using value_type = Type;
  using reference = Type &;
  using const_reference = const Type &;
  using iterator = ListIterator;
  using const_iterator = ListIteratorConst;
  using size_type = std::size_t;
  using node_type = ListNode;
  using node_pointer = ListNode *;

 private:
  // узел двусвязного списка
  class ListNode {
   public:
    // конструктор по умолчанию
    ListNode() noexcept : next_(this), prev_(this), value_(value_type{}) {}

    // конструкторы с параметрами для создания узлов с заданным значением
    explicit ListNode(const_reference value) noexcept
        : next_(nullptr), prev_(nullptr), value_(value) {}

    explicit ListNode(value_type &&value) noexcept
        : next_(nullptr), prev_(nullptr), value_(std::move(value)) {}

    // метод для обмена значениями между узлами
    void SwapValues(node_type *other_node) noexcept {
      std::swap(value_, other_node->value_);
    }

    // метод для обмена указателями на следующий и предыдущий узлы
    void SwapNextPrev() noexcept { std::swap(next_, prev_); }

    node_type *next_;   // указатель на следующий узел
    node_type *prev_;   // указатель на предыдущий узел
    value_type value_;  // значение текущего узла
  };

 public:
  // вложенный класс ListIterator
  class ListIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = list::value_type;
    using pointer = value_type *;
    using reference = value_type &;

    ListIterator() = delete;

    explicit ListIterator(node_type *node) noexcept : node_(node) {}

    reference operator*() const noexcept { return node_->value_; }

    iterator &operator++() {
      node_ = node_->next_;
      return *this;
    }

    // Переход к следующему узлу списка (постфиксный инкремент)
    iterator operator++(int) {
      iterator it = *this;
      ++(*this);
      return it;
    }

    // Переход к предыдущему узлу списка (префиксный декремент)
    iterator &operator--() {
      node_ = node_->prev_;
      return *this;
    }

    // Переход к предыдущему узлу списка (постфиксный декремент)
    iterator operator--(int) {
      iterator it = *this;
      --(*this);
      return it;
    }

    // Оператор равенства для проверки равенства двух итераторов
    bool operator==(const iterator &other) const {
      return node_ == other.node_;
    }

    // Оператор неравенства для проверки неравенства двух итераторов
    bool operator!=(const iterator &other) const { return !(*this == other); }

    node_type *node_;
  };

  // вложенный класс ListIteratorConst
  class ListIteratorConst {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = list::value_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    ListIteratorConst() : node_(nullptr) {}

    explicit ListIteratorConst(const node_type *node) : node_(node) {}

    ListIteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->value_; }

    const_iterator &operator++() {
      node_ = node_->next_;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() {
      node_ = node_->prev_;
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    friend bool operator==(const const_iterator &it1,
                           const const_iterator &it2) {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator &it1,
                           const const_iterator &it2) {
      return !(it1 == it2);
    }

    const node_type *node_;
  };

  list() : head_(new node_type{}), tail_(new node_type{}), size_(0) {}

  explicit list(size_type n) : list() {
    while (n > 0) {
      push_back(value_type{});
      --n;
    }
  }

  list(std::initializer_list<value_type> const &items) : list() {
    for (auto item : items) {
      push_back(item);
    }
  }

  list(const list &other) : list() {
    for (auto list_element : other) {
      push_back(list_element);
    }
  }

  list(list &&other)
      : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }

  list &operator=(const list &other) {
    if (this != &other) {
      clear();
      for (const auto &item : other) {
        push_back(item);
      }
    }
    return *this;
  }

  list &operator=(list &&other) {
    if (this != &other) {
      clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  ~list() {
    clear();
    delete head_;
    head_ = nullptr;
  }

  reference front() noexcept { return *begin(); }
  const_reference front() const noexcept { return *begin(); }
  reference back() noexcept { return *std::prev(end()); }
  const_reference back() const noexcept { return *std::prev(end()); }
  iterator begin() noexcept { return iterator{head_->next_}; }
  const_iterator begin() const noexcept { return const_iterator{head_->next_}; }
  iterator end() noexcept { return iterator{head_}; }
  const_iterator end() const noexcept { return const_iterator{head_}; }

  bool empty() const noexcept { return size_ == 0; }

  size_type size() const noexcept { return size_; };

  size_type max_size() const noexcept {
    return ((std::numeric_limits<size_type>::max() / 2) / sizeof(node_type)) -
           1;
  }

  // принимает два аргумента: итератор pos на позицию в списке, перед которой
  // нужно вставить новый элемент, и константную ссылку на значение value,
  // которое нужно вставить
  iterator insert(iterator pos, const_reference value) {
    node_pointer new_node = new ListNode(
        value);  // создаем новый узел new_node с переданным значением value
    // выделение памяти под новый узел с помощью оператора new
    node_pointer next_node =
        pos.node_;  // получение указателя на узел, следующий за позицией pos,
                    // чтобы затем связать новый узел с ним
    node_pointer prev_node =
        next_node->prev_;  // получение указателя на узел, предшествующий
                           // позиции pos, чтобы затем связать новый узел с ним.

    prev_node->next_ =
        new_node;  // установка указателя next_ для узла, предшествующего pos,
                   // чтобы он указывал на новый узел new_node
    new_node->prev_ =
        prev_node;  // установка указателя prev_ для нового узла new_node, чтобы
                    // он указывал на узел, предшествующий pos.
    new_node->next_ =
        next_node;  // установка указателя next_ для нового узла new_node, чтобы
                    // он указывал на узел, следующий за pos.
    next_node->prev_ =
        new_node;  // установка указателя prev_ для узла, следующего за pos,
                   // чтобы он указывал на новый узел new_node
    ++size_;  // увеличение размера списка

    return iterator(new_node);  // возвращение итератора на вставленный узел.
                                // Создание нового объекта итератора с
                                // указателем на вставленный узел new_node
  }

  const_iterator insert(const_iterator pos, const_reference value) const {
    return const_cast<list *>(this)->insert(pos, value);
    // используется const_cast, чтобы убрать const квалификатор с this указателя и вызвать неконстантную версию метода insert()
  }

  // добавляет в список элементы из диапазона [first, last], начиная с позиции, на которую указывает итератор pos
  template <typename InputIterator>
  void insert(const_iterator pos, InputIterator first, InputIterator last) { 
    for (auto it = first; it != last; ++it) { // Итератор it инициализируется значением first и продвигается до last
      insert(pos, *it); // добавление элемента в список на позицию, указанную итератором pos
    }
  }

  void erase(iterator pos) noexcept { // передача итератора на удаляемый элемент
    if (pos != end()) { // если это не последний элемент списка
      pos.node_->prev_->next_ = pos.node_->next_; // изменение указателя на следующий элемент у предыдущего элемента, чтобы он указывал на следующий элемент после удаляемого
      pos.node_->next_->prev_ = pos.node_->prev_; // изменение указателя на предыдущий элемент у следующего элемента, чтобы он указывал на предыдущий элемент перед удаляемым
      delete pos.node_; // освобождение памяти, занятой удаляемым элементом
      --size_;
    }
  }

  void clear() noexcept {
    while (size_ > 0) {
      erase(begin());
    }
  }

  void push_back(const_reference value) { insert(end(), value); }

  void pop_back() noexcept { erase(--end()); }

  void push_front(const_reference value) {
    node_pointer new_node = new ListNode(
        value);  // создаем новый узел, который будет добавлен в начало списка
    new_node->next_ = head_->next_;  // обновление указателей нового узла
    new_node->prev_ = head_;  // указатели нового узла устанавливаются таким
                              // образом, что он будет
    // следовать за головным узлом и будет указывать на первый элемент списка
    head_->next_->prev_ = new_node;  // обновление указателей старых узлов
    head_->next_ =
        new_node;  // указатели старых узлов обновляются таким образом,
    // что они будут указывать на новый узел и на следующий после него узел
    ++size_;  // увеличение размера списка
  }

  void pop_front() {
    if (empty()) {  // если список пуст
      return;       // ничего не делаем
    }
    node_pointer node_to_delete =
        head_->next_;  // сохраняем указатель на первый узел списка для
                       // последующего освобождения памяти
    head_->next_ =
        node_to_delete->next_;  // устанавливаем указатель на следующий узел для
                                // головного узла, чтобы он указывал на
                                // следующий после удаленного узла
    node_to_delete->next_->prev_ =
        head_;  // устанавливаем указатель на предыдущий узел для следующего
                // узла после удаляемого узла, чтобы он указывал на головной
                // узел
    delete node_to_delete;  // удаляем узел, на который указывал node_to_delete,
                            // освобождая память
    --size_;  // уменьшаем размер списка
  }

  void swap(list &other) {
    if (this == &other) {
      return;
    }
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }

  void splice(const_iterator pos, list &other) noexcept {
    if (!other.empty()) { // проверка, что список other не пустой
      // создание итератора it_current, указывающего на узел, на который указывает итератор pos
      // const_cast используется, чтобы убрать const-квалификатор у указателя, так как it_current - не const_iterator, а обычный итератор.
      iterator it_current{const_cast<node_type *>(pos.node_)}; 
      iterator it_other = other.end(); // создание итератора it_other, указывающего на последний элемент списка other
      
      // установка связи между узлами. Предпоследний узел списка other указывает на тот же узел, на который указывает предыдущий узел списка it_current
      it_other.node_->next_->prev_ = it_current.node_->prev_;
      it_other.node_->prev_->next_ = it_current.node_; // Последний узел списка other указывает на узел it_current

      // установка связи между узлами. Предыдущий узел it_current указывает на тот же узел, на который указывал предпоследний узел списка other
      it_current.node_->prev_->next_ = it_other.node_->next_;
      it_current.node_->prev_ = it_other.node_->prev_; // Предыдущий узел it_current указывает на предпоследний узел списка other

      size_ += other.size(); // увеличение размера списка this на размер списка other

      other.size_ = 0;
      other.head_->next_ = other.head_; // установка связи между узлами списка other
      other.head_->prev_ = other.head_;
    }
  }

  void merge(list &other) {
    auto it1 = begin(); // итератор, указывающий на начало текущего списка
    auto it2 = other.begin(); // итератор, указывающий на начало списка other

    while (it1 != end() && it2 != other.end()) { // пока не будет достигнут конец текущего списка или списка other
      // если значение, на которое указывает it1, меньше значения, на которое указывает it2, то переходим к следующему элементу списка it1
      if (*it1 < *it2) {
        ++it1;
      } else {
        // вставляем элемент, на который указывает it2, перед элементом, на который указывает it1
        insert(it1, *it2);
        ++it2; //  переходим к следующему элементу списка other с помощью it2
      }
    }

    while (it2 != other.end()) {
      // вставляем в конец текущего списка все оставшиеся элементы списка other
      push_back(*it2);
      ++it2;
    }
    // очищаем список other
    other.clear();
  }

  // объявление функции reverse без параметров и гарантией безопасности (noexcept)
  void reverse() noexcept {
    auto begin_iterator = begin(); // иттератор на начало списка
    auto end_iterator = end(); // иттератор на конец списка
    // цикл будет выполняться до тех пор, пока не будут достигнуты begin и end
    while (begin_iterator != end_iterator) {
      // меняем местами указатели на следующий и предыдущий элементы для текущего элемента, на который указывает begin_iterator
      begin_iterator.node_->SwapNextPrev();
      --begin_iterator; // перемещаем итератор на предыдущий элемент списка
    }
    head_->SwapNextPrev(); // меняем местами указатели на следующий и предыдущий элементы у head
  }

  // удаляет последовательные повторяющиеся элементы
  void unique() {
    iterator begin_it = begin(); //  создание итератора, указывающего на начало списка
    iterator end_it = end(); // создание итератора, указывающего на конец списка
    iterator prev_it = begin_it; // создание итератора, указывающего на начало списка
    ++begin_it; //  перемещаем итератор на следующий элемент списка
    for (; begin_it != end_it; ++begin_it) { // цикл будет выполняться от начала до конца списка
      if (*begin_it == *prev_it) { //  проверяем, равны ли значения текущего и предыдущего элементов списка
        erase(begin_it); // удаляем текущий элемент из списка
        begin_it = prev_it; // устанавливаем begin_it на предыдущий элемент списка
      } else {
        // если значения не равны, перемещаем prev_it на следующий элемент списка
        ++prev_it;
      }
    }
  }

  // сортировка вставками
  void sort() {
    // если в списке меньше двух элементов, то сортировать не нужно
    if (size() < 2) {
      return;
    }
    auto it = begin(); // создание итератора, указывающего на начало списка
    auto end_it = end(); // создание итератора, указывающего на конец списка
    while (it != end_it) { // цикл будет выполняться от начала до конца списка
      auto current = *it; // создание переменной, содержащей значение текущего элемента списка
      auto next_it = std::next(it); // создание итератора, указывающего на следующий элемент списка
      auto insert_it = it; // создание итератора, указывающего на текущий элемент списка
      // перемещаем insert_it к началу списка до тех пор, пока значение предыдущего элемента списка больше текущего
      while (insert_it != begin() && *std::prev(insert_it) > current) {
        --insert_it;
      }
      if (insert_it == it) { // если текущий элемент находится на своем месте
        it = next_it; // перемещаем it на следующий элемент списка и продолжаем цикл
        continue;
      }
      erase(it); // удаляем текущий элемент списка
      insert(insert_it, current); // вставляем текущий элемент списка на новое место
      it = next_it; // перемещаем it на следующий элемент списка
    }
  }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PART 3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // emplace создает новый узел со значением, переданным в качестве аргументов Args, и вставляет его в позицию, указанную в качестве pos
  // принимает переменное число аргументов Args
  template <typename... Args>
  iterator emplace(const_iterator pos, Args &&...args) noexcept {
    node_type *new_node; // создание указателя на новый узел
    iterator it_{const_cast<node_type *>(pos.node_)}; // создание итератора, который указывает на узел, переданный в качестве pos
    for (auto item : {std::forward<Args>(args)...}) { // цикл, который перебирает каждый элемент из списка переданных аргументов
      new_node = new node_type(std::move(item)); // создание нового узла с использованием конструктора перемещения
      new_node->next_ = it_.node_; // установка указателя следующего узла нового узла на узел, на который указывает итератор
      new_node->prev_ = it_.node_->prev_; // установка указателя предыдущего узла нового узла на узел, который предшествует узлу, на который указывает итератор
      it_.node_->prev_->next_ = new_node; // обновление указателя следующего узла для предыдущего узла, который предшествует узлу, на который указывает итератор, чтобы он указывал на новый узел
      it_.node_->prev_ = new_node; // обновление указателя предыдущего узла для узла, чтобы он указывал на новый узел
      ++size_; // увеличение размера списка
    }
    return iterator(new_node); // возврат итератора, указывающего на созданный узел
  }

  template <typename... Args>
  void emplace_back(Args &&...args) {
    emplace(end(), std::forward<Args>(args)...); 
    // std::forward используется для передачи аргументов по ссылке 
    // Она используется в шаблонном коде для того, чтобы сохранить rvalue-свойства аргумента (например, перемещение) 
    // при передаче его дальше по цепочке вызовов, когда мы не знаем, какой тип аргумента и как он будет использоваться 
    // внутри вызываемой функции. Это позволяет избежать ненужных копирований и перемещений объектов
  }

  template <typename... Args>
  void emplace_front(Args &&...args) {
    emplace(begin(), std::forward<Args>(args)...);
  }

 private:
  node_type *head_;
  node_type *tail_;
  size_type size_;
};

}  // namespace s21

#endif  // CONTAINERS_S21_LIST_H