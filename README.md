# my-project — Утилита для сжатия и распаковки файлов с поддержкой прерывания

Простое кроссплатформенное консольное приложение на C++ с использованием `zlib`, которое позволяет:
- 📦 Сжимать файлы в формате `.gz`
- 📂 Распаковывать `.gz` файлы
- 🛑 Корректно обрабатывать сигнал прерывания `Ctrl+C` (SIGINT)
- 💾 Обрабатывать большие файлы (10+ ГБ)

# Сборка проекта

Linux / Ubuntu
```
$ git clone https://github.com/briefl/COPM-IMS-test.git
$ cd my-project
$ mkdir build && cd build
$ cmake ..
$ make
```

Windows (с vcpkg)

```
git clone https://github.com/briefl/COPM-IMS-test.git
cd my-project
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build .
```

# Использование

```
./my-project <режим> <входной_файл> <выходной_файл>
```
**Режимы:**

a — архивировать (compress)

e — извлечь (decompress)

**Примеры**

```
./my-project a myfile.txt myfile.txt.gz      # Сжать файл
./my-project e myfile.txt.gz myfile.txt      # Распаковать файл
```
**Прерывание:**

Нажмите Ctrl+C во время сжатия/распаковки, чтобы безопасно остановить выполнение.


**Возможные ошибки**


1. Ошибка открытия входного файла
2. Ошибка открытия выходного файла
3. Ошибка записи
4. Прерывание пользователем
5. Файл пустой или повреждён
6. Неверный формат gzip





