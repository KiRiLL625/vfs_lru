#include "gtest/gtest.h"
#include "../VirtualFileSystem.h"
#include "../LRUCache.h"

TEST(AVLTree, Insert) {
    AVLTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    ASSERT_TRUE(tree.contains(1));
    ASSERT_TRUE(tree.contains(2));
    ASSERT_TRUE(tree.contains(3));
    ASSERT_TRUE(tree.contains(4));
    ASSERT_TRUE(tree.contains(5));
}

TEST(AVLTree, Remove) {
    AVLTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.remove(1);
    tree.remove(3);
    tree.remove(5);
    ASSERT_FALSE(tree.contains(1));
    ASSERT_TRUE(tree.contains(2));
    ASSERT_FALSE(tree.contains(3));
    ASSERT_TRUE(tree.contains(4));
    ASSERT_FALSE(tree.contains(5));
}

TEST(AVLTree, Contains) {
    AVLTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    ASSERT_TRUE(tree.contains(1));
    ASSERT_TRUE(tree.contains(2));
    ASSERT_TRUE(tree.contains(3));
    ASSERT_TRUE(tree.contains(4));
    ASSERT_TRUE(tree.contains(5));
    tree.remove(1);
    tree.remove(3);
    tree.remove(5);
    ASSERT_FALSE(tree.contains(1));
    ASSERT_TRUE(tree.contains(2));
    ASSERT_FALSE(tree.contains(3));
    ASSERT_TRUE(tree.contains(4));
    ASSERT_FALSE(tree.contains(5));
}

TEST(AVLTreeIterator, Iterate) {
    AVLTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    std::vector<int> expected = {1, 2, 3, 4, 5};
    std::vector<int> actual;
    for (int key : tree) {
        actual.push_back(key);
    }

    ASSERT_EQ(expected, actual);
}

TEST(Set, Insert) {
    Set<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    set.insert(4);
    set.insert(5);
    ASSERT_TRUE(set.contains(1));
    ASSERT_TRUE(set.contains(2));
    ASSERT_TRUE(set.contains(3));
    ASSERT_TRUE(set.contains(4));
    ASSERT_TRUE(set.contains(5));
}

TEST(Set, Remove) {
    Set<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    set.insert(4);
    set.insert(5);
    set.remove(1);
    set.remove(3);
    set.remove(5);
    ASSERT_FALSE(set.contains(1));
    ASSERT_TRUE(set.contains(2));
    ASSERT_FALSE(set.contains(3));
    ASSERT_TRUE(set.contains(4));
    ASSERT_FALSE(set.contains(5));
}

TEST(Dictionary, Add){
    Dictionary<int, std::string> dict;
    dict.add(1, "one");
    dict.add(2, "two");
    dict.add(3, "three");
    dict.add(4, "four");
    dict.add(5, "five");
    ASSERT_EQ(dict.get(1), "one");
    ASSERT_EQ(dict.get(2), "two");
    ASSERT_EQ(dict.get(3), "three");
    ASSERT_EQ(dict.get(4), "four");
    ASSERT_EQ(dict.get(5), "five");
}

TEST(Dictionary, Remove){
    Dictionary<int, std::string> dict;
    dict.add(1, "one");
    dict.add(2, "two");
    dict.add(3, "three");
    dict.add(4, "four");
    dict.add(5, "five");
    dict.remove(1);
    dict.remove(3);
    dict.remove(5);
    ASSERT_THROW(dict.get(1), std::runtime_error);
    ASSERT_EQ(dict.get(2), "two");
    ASSERT_THROW(dict.get(3), std::runtime_error);
    ASSERT_EQ(dict.get(4), "four");
    ASSERT_THROW(dict.get(5), std::runtime_error);
}

TEST(Dictionary, Contains){
    Dictionary<int, std::string> dict;
    dict.add(1, "one");
    dict.add(2, "two");
    dict.add(3, "three");
    dict.add(4, "four");
    dict.add(5, "five");
    ASSERT_TRUE(dict.contains_key(1));
    ASSERT_TRUE(dict.contains_key(2));
    ASSERT_TRUE(dict.contains_key(3));
    ASSERT_TRUE(dict.contains_key(4));
    ASSERT_TRUE(dict.contains_key(5));
    dict.remove(1);
    dict.remove(3);
    dict.remove(5);
    ASSERT_FALSE(dict.contains_key(1));
    ASSERT_TRUE(dict.contains_key(2));
    ASSERT_FALSE(dict.contains_key(3));
    ASSERT_TRUE(dict.contains_key(4));
    ASSERT_FALSE(dict.contains_key(5));
}

TEST(Dictionary, Get){
    Dictionary<int, std::string> dict;
    dict.add(1, "one");
    dict.add(2, "two");
    dict.add(3, "three");
    dict.add(4, "four");
    dict.add(5, "five");
    ASSERT_EQ(dict.get(1), "one");
    ASSERT_EQ(dict.get(2), "two");
    ASSERT_EQ(dict.get(3), "three");
    ASSERT_EQ(dict.get(4), "four");
    ASSERT_EQ(dict.get(5), "five");
    dict.remove(1);
    dict.remove(3);
    dict.remove(5);
    ASSERT_THROW(dict.get(1), std::runtime_error);
    ASSERT_EQ(dict.get(2), "two");
    ASSERT_THROW(dict.get(3), std::runtime_error);
    ASSERT_EQ(dict.get(4), "four");
    ASSERT_THROW(dict.get(5), std::runtime_error);
}

TEST(Dictionary, Clear){
    Dictionary<int, std::string> dict;
    dict.add(1, "one");
    dict.add(2, "two");
    dict.add(3, "three");
    dict.add(4, "four");
    dict.add(5, "five");
    dict.clear();
    ASSERT_THROW(dict.get(1), std::runtime_error);
    ASSERT_THROW(dict.get(2), std::runtime_error);
    ASSERT_THROW(dict.get(3), std::runtime_error);
    ASSERT_THROW(dict.get(4), std::runtime_error);
    ASSERT_THROW(dict.get(5), std::runtime_error);
}

#include <chrono>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

// Генерация файлов в папке
void generateTestFiles(const std::string &directory, int fileCount) {
    fs::create_directory(directory); // Создание папки test_files
    for (int i = 1; i <= fileCount; ++i) { // Создание файлов file1, file2, ..., file<fileCount>
        std::ofstream file(directory + "/file" + std::to_string(i));
        file.close();
    }
}

// Генерация случайной структуры виртуальной файловой системы
void generateVirtualFileSystem(VirtualFileSystem &vfs, const std::string &basePath, int depth, int width) {
    // Рекурсивная функция для создания структуры
    std::function<void(const std::string &, int)> createStructure = [&](const std::string &path, int level) {
        if (level > depth) return; // Если достигнута максимальная глубина

        for (int i = 1; i <= width; i++) { // Создание директорий и файлов
            // Создание директории с названием dir<level>_<i(width)>
            std::string dirName = "dir" + std::to_string(level) + "_" + std::to_string(i);
            vfs.addDirectory(path, dirName);

            for (int j = 1; j <= width; j++) {
                // Создание файла с названием file<level>_<i(width)>_<j(width)>
                std::string fileName = "file" + std::to_string(level) + "_" + std::to_string(i) + "_" + std::to_string(j);
                vfs.addFile(path + "/" + dirName, basePath + "/file" + std::to_string(rand() % 2000 + 1), fileName);
            }

            createStructure(path + "/" + dirName, level + 1); // Рекурсивный вызов для создания дочерних директорий
        }
    };

    createStructure("/", 1);
}

// Тесты для работы с виртуальной файловой системой
class VirtualFileSystemTest : public ::testing::Test {
protected:
    const std::string testFilesDir = "test_files"; // Папка с тестовыми файлами
    const int fileCount = 2000; // Количество файлов

    // Вызывается перед каждым тестом
    void SetUp() override {
        // Генерация файлов
        generateTestFiles(testFilesDir, fileCount);
    }

    // Вызывается после каждого теста
    void TearDown() override {
        // Удаление тестовых файлов
        fs::remove_all(testFilesDir);
    }
};

// Тест создания и проверки структуры (TEST_F вместо TEST для использования фикстуры - класса VirtualFileSystemTest)
TEST_F(VirtualFileSystemTest, CreateAndValidateStructure) {
    VirtualFileSystem vfs;

    // Генерация случайной виртуальной файловой системы
    const int depth = 3; // Глубина дерева
    const int width = 3; // Количество директорий/файлов на уровень

    auto startTime = std::chrono::high_resolution_clock::now();
    generateVirtualFileSystem(vfs, testFilesDir, depth, width);
    auto endTime = std::chrono::high_resolution_clock::now();

    std::cout << "Time creating vfs: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
              << " ms" << std::endl;

    // Вывод структуры
    vfs.printStructure();

    // Проверка существования директорий и файлов
    ASSERT_NO_THROW(vfs.addDirectory("/", "testDir"));
    ASSERT_NO_THROW(vfs.addFile("/testDir", testFilesDir + "/file1", "testFile"));
    ASSERT_THROW(vfs.addFile("/nonexistentDir", testFilesDir + "/file1", "testFile"), std::runtime_error);
}

// Тест производительности
TEST_F(VirtualFileSystemTest, PerformanceTest) {
    VirtualFileSystem vfs;

    // Добавление множества файлов и директорий
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= 100; i++) {
        vfs.addDirectory("/", "dir" + std::to_string(i));
        for (int j = 1; j <= 10; j++) {
            vfs.addFile("/dir" + std::to_string(i), testFilesDir + "/file" + std::to_string((i - 1) * 10 + j), "file" + std::to_string(j));
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();

    std::cout << "Time to insert 100 directories and 1000 files: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
              << " ms" << std::endl;
}


// Тестовый класс для работы с LRUCache
class LRUCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Выполняется перед каждым тестом
        cache = std::make_unique<LRUCache<int, std::string>>(3); // Кэш с размером 3
    }

    std::unique_ptr<LRUCache<int, std::string>> cache; // Указатель на кэш
};

// Тест метода access и базовых операций
TEST_F(LRUCacheTest, AccessAndBasicOperations) {
    // Добавляем элементы в кэш
    cache->access(1, "one");
    cache->access(2, "two");
    cache->access(3, "three");

    // Проверяем размер кэша
    EXPECT_EQ(cache->size(), 3);

    // Проверяем наличие элементов
    EXPECT_TRUE(cache->contains(1));
    EXPECT_TRUE(cache->contains(2));
    EXPECT_TRUE(cache->contains(3));
    EXPECT_FALSE(cache->contains(4));

    // Проверяем значения
    EXPECT_EQ(cache->get(1), "one");
    EXPECT_EQ(cache->get(2), "two");
    EXPECT_EQ(cache->get(3), "three");

    // Добавляем новый элемент, вытесняя самый старый (1)
    cache->access(4, "four");

    // Теперь элемент 1 должен быть удалён
    EXPECT_FALSE(cache->contains(1));
    EXPECT_TRUE(cache->contains(2));
    EXPECT_TRUE(cache->contains(3));
    EXPECT_TRUE(cache->contains(4));

    // Проверяем значения оставшихся элементов
    EXPECT_EQ(cache->get(2), "two");
    EXPECT_EQ(cache->get(3), "three");
    EXPECT_EQ(cache->get(4), "four");
}

// Тест LRU поведения (последнее использование перемещает элемент в начало)
TEST_F(LRUCacheTest, LRUBehavior) {
    // Добавляем элементы в кэш
    cache->access(1, "one");
    cache->access(2, "two");
    cache->access(3, "three");

    // Используем элемент 1
    EXPECT_EQ(cache->get(1), "one"); // Должен остаться в кэше
    cache->access(1, "one"); // Помечаем как использованный

    // Добавляем новый элемент, вытесняя элемент 2
    cache->access(4, "four");

    // Теперь элемент 2 должен быть удалён
    EXPECT_FALSE(cache->contains(2));
    EXPECT_TRUE(cache->contains(1));
    EXPECT_TRUE(cache->contains(3));
    EXPECT_TRUE(cache->contains(4));

    // Проверяем порядок в кэше
    std::ostringstream cacheOutput;
    cache->print(); // Вывод в консоль для проверки
}

// Тест производительности
TEST_F(LRUCacheTest, PerformanceTest) {
    std::srand(std::time(nullptr));
    int numElements; // От 10000 до 100000 элементов

    for(int j = 0; j < 5; j++) {
        numElements = 10000 + std::rand() % 90000;
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numElements; i++) {
            cache->access(i, "value" + std::to_string(i));
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time to insert " << numElements << " elements: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms" << std::endl;

        cache->clear();
    }
}
