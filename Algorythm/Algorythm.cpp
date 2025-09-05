#include <iostream>
#include <string>

void types_numbers(short Short1, int Int1, long Long1, long long LLong1, float Float1, double Double1) {
    std::cout << "\nЧисленные типы:\n1 - short Short1\n2 - int Int1\n3 - long Long1\n4 - long long LLong1\n5 - float Float1\n6 - double Double1" << std::endl;
    std::cout << "Примеры операций с ними" << "\n";
    Short1 = Short1 * 3;
    std::cout << "Short1 = Short1 * 3;" << Short1 << "\n";
    Int1 = Int1 / 2;
    std::cout << "Int1 = Int1 / 2;" << Int1 << "\n";
    Long1 = Long1 % 3;
    std::cout << "Long1 = Long1 % 3;" << Long1 << "\n";
    LLong1 = LLong1 + 1;
    std::cout << "LLong1 = LLong1 + 1;" << LLong1 << "\n";
    Float1 = Float1 - 1.1;
    std::cout << "Float1 = Float1 - 1.1;" << Float1 << "\n";
    Double1 = Double1 * 3.1;
    std::cout << "Double1 = Double1 * 3.1;" << Double1 << "\n";
}

void types_letters(char Char1, signed char SChar1, unsigned char UChar1) {
    Char1 = 'B';
    std::cout << "\nChar1 = 'B';" << Char1 << "\n";
    SChar1 = SChar1 - 11;
    std::cout << "SChar1 = SChar1 - 11;" << SChar1 << "\n";
    UChar1 = UChar1 + 10;
    std::cout << "UChar1 = UChar1 + 10;" << UChar1 << "\n";
}

void types_others(bool Boolean1, int myArray1[], int* ptrArray1) {
    int arraySize = 3;
    Boolean1 = false;
    std::cout << "int myArray[] = {10,20,30};\n" << "\nBoolean1 = false;" << Boolean1 << "\n";
    for (int i = 0; i < 3; i++) {
        myArray1[i] + 1;
        std::cout << myArray1[i] << "\n";
    }
    std::cout << "int* ptrArray = new int[arraySize];\n" << "myArray1[i] + 1;" << Boolean1 << "\n\n";
    for (int i = 0; i < arraySize; i++) {
        ptrArray1[i] = i * 20;
        std::cout << ptrArray1[i] << "\n";
    }
}

class Car {
public:
    std::string brand;
};

int main()
{
    setlocale(0, "");
    Car myCar;
    myCar.brand = "Bebra";
    std::cout << "Машина: " << myCar.brand << std::endl;

    int arraySize{};
    bool Boolean = true;
    char Char = 'F';
    signed char SChar = -22;
    unsigned char UChar = 'A';
    short Short = 32767;
    int Int = 2147483647;
    long Long = 4294967295;
    long long LLong = 9223372036854775807;
    float Float = 10.1;
    double Double = 10.11;
    int myArray[] = { 10,20,30 };


    int* ptrArray = new int[arraySize];




    bool IsTrue = true;
    short Check = 0;


    while (IsTrue == true) {
        std::cout << "\nВведите тип:\n1 - numbers\n2 - letters\n3 - others" << std::endl;
        std::cin >> Check;

        if (Check == 1) {
            types_numbers(Short, Int, Long, LLong, Float, Double);
        }
        else if (Check == 2) {
            types_letters(Char, SChar, UChar);
            delete[] ptrArray;
            ptrArray = nullptr;
        }
        else if (Check == 3) {
            types_others(Boolean, myArray, ptrArray);
        }
        else {
            std::cout << "Укажите верное значение" << std::endl;
            IsTrue = false;
        }
    }
}


/*
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Converter
{
    public partial class filePathLabel : Form
    {
        private string selectedFilePath;

        public filePathLabel()
        {
            InitializeComponent();
            //LoadUnits();
        }

        //private void LoadUnits()
        //{
        //    string[] units = { "Цельсий", "Фаренгейт", "Кельвин" };
        //    comboBox1.Items.AddRange(units);
        //    comboBox2.Items.AddRange(units);
        //    comboBox1.SelectedIndex = 0;
        //    comboBox2.SelectedIndex = 1;
        //}

        private void button1_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(selectedFilePath))
            {
                MessageBox.Show("Пожалуйста, сначала выберите файл с данными.", "Файл не выбран", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // 2. Читаем числа из файла
            int[] numbers;
            try
            {
                string fileContent = File.ReadAllText(selectedFilePath);
                // Разделяем строку на части по пробелам, табуляции и переносам строк,
                // убираем пустые элементы и преобразуем каждую часть в число.
                numbers = fileContent.Split(new char[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries)
                                     .Select(int.Parse)
                                     .ToArray();
            }
            catch (FileNotFoundException)
            {
                MessageBox.Show("Файл не найден. Возможно, он был перемещен или удален.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (FormatException)
            {
                MessageBox.Show("Файл содержит некорректные данные. Убедитесь, что в файле только числа, разделенные пробелами.", "Ошибка формата", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Произошла ошибка при чтении файла: " + ex.Message, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Если в файле нет чисел, выходим
            if (numbers.Length == 0)
            {
                resultRichTextBox.Text = "Файл пуст или не содержит чисел.";
                return;
            }

            // 3. Запускаем сортировку и получаем подробный лог
            StringBuilder log = new StringBuilder();
            log.AppendLine("Начальный массив: " + string.Join(", ", numbers));
            log.AppendLine("------------------------------------");
            log.AppendLine("Начинаем сортировку методом простого выбора:");

            SelectionSort(numbers, log); // Вызываем метод сортировки

            log.AppendLine("------------------------------------");
            log.AppendLine("Сортировка завершена!");
            log.AppendLine("Отсортированный массив: " + string.Join(", ", numbers));

            // 4. Выводим результат в RichTextBox
            resultRichTextBox.Text = log.ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*";
            openFileDialog1.Title = "Выберите текстовый файл с числами";

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                // Сохраняем путь к файлу в нашу переменную
                selectedFilePath = openFileDialog1.FileName;

                // Показываем пользователю, какой файл он выбрал
                label3.Text = "Выбранный файл: " + Path.GetFileName(selectedFilePath);
                resultRichTextBox.Text = "Файл успешно выбран. Нажмите 'Сортировать' для начала.";

                this.Text = "Сортировка файла: " + Path.GetFileName(selectedFilePath);
            }
        }

        private void SelectionSort(int[] array, StringBuilder log)
        {
            int n = array.Length;
            for (int i = 0; i < n - 1; i++)
            {
                log.AppendLine($"\n--- Итерация {i + 1} (ищем элемент для позиции {i}) ---");
                log.AppendLine("Текущий массив: " + string.Join(", ", array));

                // Находим индекс минимального элемента в оставшейся части массива
                int minIndex = i;
                for (int j = i + 1; j < n; j++)
                {
                    if (array[j] < array[minIndex])
                    {
                        minIndex = j;
                    }
                }

                log.AppendLine($"Найден минимальный элемент {array[minIndex]} на позиции {minIndex}.");

                // Если минимальный элемент не на своем месте, меняем его с текущим
                if (minIndex != i)
                {
                    log.AppendLine($"Меняем местами {array[i]} (на позиции {i}) и {array[minIndex]} (на позиции {minIndex}).");
                    int temp = array[minIndex];
                    array[minIndex] = array[i];
                    array[i] = temp;
                }
                else
                {
                    log.AppendLine("Минимальный элемент уже на своем месте.");
                }

                log.AppendLine("Массив после итерации: " + string.Join(", ", array));
            }
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
1
*/