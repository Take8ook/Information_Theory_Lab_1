#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

using namespace std;

const string FORMAT = "bcaa1098";
const string VERSION = "0.0.1";
const string CODES = "000000";

//const string HEADER = FORMAT + VERSION + CODES + BYTES + AMOUNT_FILES
// формат, версия, коды алгоритмов, кол-во байтов в файле (для доп заданий), кол-во файлов в архиве

/*
* + проверка сигнатуры
* + проверка кодов
* + узнать размер файла
* + меню для выбора кодера/декодера
* + кодер (шапка + текст)
* + декодер (выудить шапку, проверить ее и оставить только текст)
* 
* - сделать возможность сохранения нескольких файлов в одном архиве
    записывать в архив название + содержимое, чтобы можно было восстановить пофайлово
* 
* - сделать возможность сохранения и восстановления вложенных папок 
    заходить внутрь директории, записывать иерархию, файл из первой директории записывается первым
    цикл кончается, когда заканчиваются папки
    (записывать сразу название (путь + имя) файла + содержимое)  
*/
 
const string FILE_NAME_PRECODE = "txt_files/1.txt";     // исходный файл
const string FILE_NAME_CODE = "txt_files/2.txt";        // кодированный файл 
const string FILE_NAME_DECODE = "txt_files/3.txt";      // декодированный файл 

bool check_format(string* header)
{
    int len = FORMAT.length(), start_pos = 0;
    int is_different = header->compare(start_pos, len, FORMAT);
    return is_different;
}

bool check_codes(string* header)
{
    int len = CODES.length(), start_pos = FORMAT.length() + VERSION.length();
    int is_different = header->compare(start_pos, len, CODES);
    return is_different;
}

int read_from(const string file_name, string* text, int* file_size, string* header=nullptr)
{
    ifstream fin(file_name);

    if (!fin.is_open()){
        cout << " Cannot open! \n\n";
        return 1;
    }

    string temp;
    bool flag = false;

    if (!fin.eof())
    {
        fin >> temp;
        if (header != nullptr)
            *header = temp;
        else text->append(temp);
    }

    while (!fin.eof())
    {
        if (flag)
            text->append("\n");
        fin >> temp;
        text->append(temp);
        flag = true;
    }

    fin.seekg(0, ios::end);
    *file_size = fin.tellg();

    fin.close();

    return 0;
}

int write_to(const string file_name, string* text)
{
    ofstream fout(file_name);

    if (!fout.is_open()) {
        cout << " Cannot create new file! \n\n";
        return 2;
    }

    fout << *text;
    fout.close();

    return 0;
}

int coder(string* message)
{
    string text = "", str;
    int file_size = 0;
    int is_wrong_reading = read_from(FILE_NAME_PRECODE, &str, &file_size);

    if (is_wrong_reading)
    {
        *message = "Wrong reading!.\n\n";
        return is_wrong_reading;
    }
        
    text.append(FORMAT + VERSION + CODES + "bytes-" + to_string(file_size) + "\n" + str);

    int is_wrong_writing = write_to(FILE_NAME_CODE, &text);

    if (is_wrong_writing)
    {
        *message = "Wrong writing!\n\n";
        return is_wrong_writing;
    }

    *message = "Coding completed";

    return 0;
}

int decoder(string* message)
{
    string str, source_text, header;
    bool is_first = true;
    int file_size = 0, is_wrong_writing = 0;

    int is_wrong_reading = read_from(FILE_NAME_CODE, &str, &file_size, &header);

    if (is_wrong_reading)
    {
        *message = "Wrong reading!.\n\n";
        return is_wrong_reading;
    }

    bool is_wrong_format = check_format(&header); // 0 = ок, 1 != ок
    bool is_wrong_codes = check_codes(&header); // 0 = ок, 1 != ок

    string wrong_format_message = "Format error!";
    string wrong_codes_message = "Codes error!";

    source_text.append(str);

    if (!is_wrong_format)
        if (!is_wrong_codes)
        {
            *message = "Decoding completed.\n\n";
            is_wrong_writing = write_to(FILE_NAME_DECODE, &source_text);
        }
        else
        {
            *message = "Wrong codes!\n\n";
            is_wrong_writing = write_to(FILE_NAME_DECODE, &wrong_codes_message);
        }
    else
    {
        *message = "Wrong format!\n\n";
        is_wrong_writing = write_to(FILE_NAME_DECODE, &wrong_format_message);
    }

    if (is_wrong_writing)
    {
        *message = "Wrong writing!\n\n";
        return is_wrong_writing;
    }

    return 0;
}

int start()
{
    char choice;
    bool is_end = false;

    cout << "Start.\n\n";

    cout << "NUM - FUNCTION\n";
    cout << "\n";
    cout << " 1  - TO CODE\n";
    cout << " 2  - TO DECODE\n";
    cout << "\n";
    cout << " 0  - EXIT\n\n";

    while (!is_end)
    {
        // system("cls");
        string message;

        cout << "Your choice: ";
        choice = _getch();
        cout << choice << "\n\n";

        int error_code = 0;

        switch (choice)
        {
        case '1':
        {
            error_code = coder(&message);
            if (error_code)
                return error_code;
            break;
        }
        case '2':
        {
            error_code = decoder(&message);
            if (error_code)
                return error_code;
            break;
        }
        case '0':
        {
            message = "Finishing program...\n";
            is_end = true;
            break;
        }
        default:
        {
            message = "Wrong input. Please, choose correct point.\n\n";
        }
        }

        cout << message;
    }

    return 0;
}

int main()
{
    int code = start();

    return code;
}

