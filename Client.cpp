#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

struct StudentRequest
{
    char name[25];     // имя 
    int count;         // кол-во оценок
    // Оценки будут передаваться отдельно
};

int main() {
    setlocale(LC_ALL, "rus");

    string nameR = "C:/Users/sachi/source/repos/CompSetServer/CompSetServer/REQUEST.bin";
    string nameA = "C:/Users/sachi/source/repos/CompSetServer/CompSetServer/ANSWER.bin";

    ofstream f_REQ;
    ifstream f_ANS;
    long pred_size;
    int answer;

    while (true)
    {
        // передача данных от клиента серверу
        cout << "Введите запрос: Фамилия Кол-во оценок, Оценки" << endl;

        StudentRequest request;
        cin >> request.name >> request.count;

        int* marks = new int[request.count];
        for (int i = 0; i < request.count; i++) {
            cin >> marks[i];
        }

        cout << request.name << " " << request.count << " ";
        for (int i = 0; i < request.count; i++) {
            cout << marks[i] << " ";
        }
        cout << endl;

        // запись в файл
        f_REQ.open(nameR, ios::app | ios::binary);
        f_REQ.write((char*)&request, sizeof(request)); // записываем структуру
        f_REQ.write((char*)marks, sizeof(int) * request.count); // записываем оценки
        f_REQ.close();

        delete[] marks; // освобождаем память

        // ожидание ответа от сервера
        f_ANS.open(nameA, ios::binary);
        f_ANS.seekg(0, ios::end);
        pred_size = f_ANS.tellg();

        while (pred_size >= f_ANS.tellg())
        {
            Sleep(100);
            f_ANS.seekg(0, ios::end);
        }

        // получение ответа от сервера    
        f_ANS.seekg(pred_size, ios::beg);
        f_ANS.read((char*)&answer, sizeof(answer));
        f_ANS.close();

        // расшифровка ответа
        switch (answer) {
        case 1: { cout << "Повышенная стипендия\n"; break; }
        case 2: { cout << "Нет стипендии\n"; break; }
        case 3: { cout << "Обычная стипендия\n"; break; }
        default: { cout << "Неизвестный ответ: " << answer << "\n"; break; }
        }
    }
}
