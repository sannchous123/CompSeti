#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

struct StudentRequest
{
    char name[25];     // имя 
    int count;         // кол-во оценок
};

int answer;
long size_pred;

int main() {
    ifstream fR;
    ofstream fA;
    setlocale(LC_ALL, "rus");

    string nameR = "C:/Users/sachi/source/repos/CompSetServer/CompSetServer/REQUEST.bin";
    string nameA = "C:/Users/sachi/source/repos/CompSetServer/CompSetServer/ANSWER.bin";

    cout << "server is working" << endl;

    // начальные установки
    fR.open(nameR, ios::binary);
    fR.seekg(0, ios::end);
    size_pred = fR.tellg();
    fR.close();

    while (true)
    {
        fR.open(nameR, ios::binary);
        fR.seekg(0, ios::end);

        // ожидание новых запросов
        while (size_pred >= fR.tellg())
        {
            Sleep(100);
            fR.seekg(0, ios::end);
        }

        // обработка нового запроса
        fR.seekg(size_pred, ios::beg);

        // считываем структуру
        StudentRequest request;
        fR.read((char*)&request, sizeof(request));

        // считываем оценки
        int* marks = new int[request.count];
        fR.read((char*)marks, sizeof(int) * request.count);

        size_pred = fR.tellg();
        fR.close();

        // определение стипендии (ИСПРАВЛЕННАЯ ЛОГИКА)
        answer = 1; // по умолчанию - повышенная
        bool hasFour = false;

        for (int i = 0; i < request.count; i++)
        {
            cout << "Оценка " << i << ": " << marks[i] << endl; // отладочный вывод

            if (marks[i] == 2 || marks[i] == 3) {
                answer = 2; // нет стипендии
                break;
            }
            else if (marks[i] == 4) {
                hasFour = true;
                answer = 3; // обычная стипендия
                // НЕ break - продолжаем проверять на наличие 2/3
            }
        }

        // Если нашли 4, но не нашли 2/3 - оставляем answer = 3
        // Если не нашли ни 4, ни 2/3 - answer остается 1

        delete[] marks; // освобождаем память

        cout << "Ответ сервера: " << answer << endl; // отладочный вывод

        // передача ответа клиенту
        fA.open(nameA, ios::binary | ios::app);
        fA.write((char*)&answer, sizeof(answer));
        fA.close();
    }
    return 0;
}
