#include <iostream>
#include <vector>
#include <fstream>

const char *in_fname = "in.txt";
const char *out_fname = "out.txt";

int get_size()
{
    int a = 0;
    std::ifstream fin;
    fin.open(in_fname);
    if (fin)
    {
        std::string buff;
        getline(fin, buff);
        if (buff.length() <= 1)
        {
            std::cout << "Пустой файл ввода, заполните его и запустите снова" << std::endl;
            return 0;
        }
        for (int i = 1; i < buff.length() - 1; i++)
            if (buff[i] == ' ' && buff[i + 1] != '\n')
            {
                a++;
                if (buff[i - 1] == ' ' || buff[i + 1] == ' ')
                {
                    std::cout << "В входных данных есть лишние пробелы, исправьте входные данные и перезапустите программу" << std::endl;
                    return 0;
                }
            }
    }
    else
    {
        std::ofstream fout;
        fout.open(in_fname);
        fout.close();
        std::cout << "Файл ввода не найден или повреждён создан новый файл для ввода " << in_fname << ".\n"
                  << std::endl;
    }
    fin.close();
    return a;
}

std::vector<float> input()
{
    std::vector<float> in;
    std::ifstream fin;
    fin.open(in_fname);
    if (fin)
    {
        while (!fin.eof())
        {
            std::string buff;
            float i = 0;
            getline(fin, buff, ' ');
            try
            {
                int a = buff.find('\n');
                if (a > 0)
                {
                    std::string buff2;
                    for (int i = a; i < buff.length(); i++)
                    {
                        buff2.push_back(buff[i]);
                    }
                    i = std::stof(buff);
                    in.push_back(i);
                    i = std::stof(buff2);
                    in.push_back(i);
                }
                else
                {
                    i = std::stof(buff);
                    in.push_back(i);
                }
            }
            catch (std::invalid_argument &e)
            {
                {
                    std::cout << "Некоректные данные \"" << buff << "\"\n";
                    in.clear();
                    return in;
                }
            }
        }
    }
    else
    {
        std::ofstream fout;
        fout.open(in_fname);
        fout.close();
        std::cout << "Файл ввода не найден или повреждён создан новый файл для ввода " << in_fname << ".\n"
                  << std::endl;
    }
    fin.close();
    return in;
}

void print_input(float *a[], float *b, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << "X" << i + 1 << "\t";
    }
    std::cout << "D" << std::endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            std::cout << a[i][j] << "\t";
        }
        std::cout << b[i];
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

bool check(float *a[], int size)
{
    for (int i = 1; i < size - 1; i++)
    {
        if (abs(a[i][i]) < abs(a[i][i - 1]) + abs(a[i][i + 1]))
        {
            std::cout << "Не выполнены условия достаточности" << std::endl;
            return false;
        }
        if (a[i][i] == 0)
        {
            std::cout << "Нулевые элементы на главной диагонали" << std::endl;
            return false;
        }
    }

    if ((abs(a[0][0]) < abs(a[0][1])) || (abs(a[size - 1][size - 1]) < abs(a[size - 1][size - 2])))
    {
        std::cout << "Не выполнены условия достаточности" << std::endl;
        return false;
    }
    return true;
}

float *solution(float *a[], float *b, int size)
{
    float *x = new float[size], u[size], v[size];

    v[0] = a[0][1] / (-a[0][0]);
    u[0] = (-b[0]) / (-a[0][0]);

    for (int i = 1; i < size - 1; i++)
    {
        v[i] = a[i][i + 1] / (-a[i][i] - a[i][i - 1] * v[i - 1]);
        u[i] = (a[i][i - 1] * u[i - 1] - b[i]) / (-a[i][i] - a[i][i - 1] * v[i - 1]);
    }

    v[size - 1] = 0;
    u[size - 1] = (a[size - 1][size - 2] * u[size - 2] - b[size - 1]) / (-a[size - 1][size - 1] - a[size - 1][size - 2] * v[size - 2]);

    std::cout << "Прогоночные коэффициенты V:" << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cout << "V" << i + 1 << "=" << v[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Прогоночные коэффициенты U:" << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cout << "U" << i + 1 << "=" << u[i] << std::endl;
    }
    std::cout << std::endl;

    x[size - 1] = u[size - 1];
    for (int i = size - 1; i > 0; --i)
    {
        x[i - 1] = v[i - 1] * x[i] + u[i - 1];
    }
    return x;
}

void output(int size, float *x)
{
    std::ofstream fout;
    fout.open(out_fname, std::ios_base::trunc);
    std::cout << "Решение:" << std::endl;
    for (int i = 0; i < size; i++)
    {
        fout << "X" << i + 1 << "=" << x[i] << '\n';
        std::cout << "X" << i + 1 << "=" << x[i] << std::endl;
    }
    std::cout << std::endl;
    fout.close();
}

int main()
{
    // Проверка на правильность данных и получение их из файла
    int size = get_size();
    if (size == 0)
        return 0;
    std::vector<float> in = input();
    if (in.empty())
        return 0;

    // блок для превращения вектора в матрицу
    float b[size], a[size][size], *matrix[size], *x = new float[size];
    int c = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            a[i][j] = in[c];
            c++;
        }
        b[i] = in[c];
        c++;
        matrix[i] = a[i];
    }
    in.clear();

    print_input(matrix, b, size);

    // Основной блок в котором проверяется возможность решения и решается СЛАУ
    if (check(matrix, size))
    {
        x = solution(matrix, b, size);
        output(size, x);
    }
    return 0;
}