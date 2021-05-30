#include <iostream>
#include <algorithm>
#include "StringBuilderClass.h"
#include "FileManagerClass.h"
#include "ClassMenu.h"
#include "StudentDBClass.h"
#include "StudentClass.h"
#include "ClassEdit.h"
#include "list.hpp"
int main()
{
    using namespace std;
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
    SetConsoleOutputCP(1251);
    std::cout << "Курсовая работа запущена...\n";
    StudentDBClass* sdb = new StudentDBClass();
    StudentDBClass* sdb1 = new StudentDBClass();
    StudentDBClass* sdb2 = new StudentDBClass();
    vector<string> groups;
    vector<string>* groupsptr = &groups;
    vector<int> indexes1;
    vector<int>* indexesptr1 = &indexes1;
    vector<int> indexes2;
    vector<int>* indexesptr2 = &indexes2;
    sdb->FileName = "DB.txt";
    sdb->loadDataFromFile();

        ClassMenu* mainMenu = new ClassMenu();
        mainMenu->addTitleItem("Главное меню");
        mainMenu->addItem("Просмотреть список студетов (удалить или изменить данные)"); //0
        mainMenu->addItem("Добавить данные о студенте в БД"); //1
        mainMenu->addItem("Сохранить БД студентов в файл"); //2
        mainMenu->addItem("Выполнить вариант 80"); //3
        mainMenu->addItem("Выход"); //4
        int resultSelectedItem = 0;
        int exitInt = 4;
        ClassMenu* studentsMenu = new ClassMenu();
        studentsMenu->addTitleItem("Список студентов");
        int resultStudentSelectedItem = 1;
        const int exitIntStudentMenu = 0;
        StudentNode* sn;
        StudentClass* st = new StudentClass();
        ClassMenu* delStudentsMenu = new ClassMenu();
        int curCount;
        ClassEdit* ce = new  ClassEdit();
        int startYear = 0;
        int endYear = 0;
        int year = 0;
        StringBuilderClass* sb = new StringBuilderClass();
        ClassMenu* groupsMenu = new ClassMenu();
        int resultGr = 1, resultSes = 1;
        const int exitGr = 0, exitSes = 0;
        ClassMenu* sesMenu = new ClassMenu();
        while (resultSelectedItem != exitInt) {
            mainMenu->run();
            resultSelectedItem = mainMenu->getSelectedItem();
            switch (resultSelectedItem) {
            case 0:
                resultStudentSelectedItem = 1;
                while (resultStudentSelectedItem != exitIntStudentMenu) {
                    studentsMenu->eraseItem();
                    studentsMenu->addItem("Выход");
                    studentsMenu->addItem("Удалить данные о студенте");
                    for (int i = 0; i < sdb->DataBase.size(); i++)
                    { // добавить пункты меню ФИО студентов
                        sn = &sdb->DataBase.at(i);
                        string tmpString = sn->surName + " " + sn->name + " " + sn->middleName + " " + sn->group;
                        studentsMenu->addItem(tmpString); //добавить в меню студентов
                    }
                    studentsMenu->run();
                    resultStudentSelectedItem = studentsMenu->getSelectedItem();
                    if (resultStudentSelectedItem == exitIntStudentMenu) {
                        break;
                    }
                    if (resultStudentSelectedItem == 1) //удаление данных о студенте
                    {
                        delStudentsMenu->eraseAll();
                        delStudentsMenu->addTitleItem("Выберите студента для удаления данных");
                        delStudentsMenu->addItem("Выход");
                        int resultDel = 1;
                        const int exitDel = 0;
                        for (int i = 0; i < sdb->DataBase.size(); i++)
                        { // добавить пункты меню ФИО студентов
                            sn = &sdb->DataBase.at(i);
                            string tmpString = sn->surName + " " + sn->name + " " + sn->middleName + " " + sn->group;
                            delStudentsMenu->addItem(tmpString); //добавить в меню студентов
                        }
                        while (resultDel != exitDel) {
                            delStudentsMenu->run();
                            resultDel = delStudentsMenu->getSelectedItem();
                            if (resultDel == exitDel) {
                                break;
                            }
                            int num = resultDel - 1;
                            sdb->DataBase.erase(std::next(sdb->DataBase.begin(), num));
                        }
                    }
                    if (resultStudentSelectedItem > 1)
                    {
                        int num = resultStudentSelectedItem - 2; //!
                        sn = &sdb->DataBase.at(num);
                        string oldRecordСardNumber = "";
                        oldRecordСardNumber = sn->recordСardNumber;
                        st->editStudent(sn);

                        if (sdb->getSameRecordСardNumber(sn->recordСardNumber) > 1)
                        {
                            sn->recordСardNumber = oldRecordСardNumber;
                            cout << "Ошибка введен номер зачетной книжки который уже есть в БД";
                            _getch();
                        }
                    }
                }
                break;
            case 1:
                sn = new StudentNode();
                for (int i = 0; i < 9; i++)
                    for (int j = 0; j < 10; j++)
                        sn->examsRecordsData[i][j].isEmpty = true;
                st->editStudent(sn);
                if (sdb->getSameRecordСardNumber(sn->recordСardNumber) == 1)
                {
                    cout << "Ошибка введен номер зачетной книжки который уже есть в БД";
                    _getch();
                }
                else
                {
                    sdb->DataBase.push_front(*sn);
                }
                break;
            case 2: //Сохранить в файл
                sdb->saveDataToFile(sdb->FileName);
                break;
            case 3: //Вариант 80

                ce->setLabel("Введите начальный год рождения для интервала выборки. ");
                startYear = ce->setDataInt(1900, 2021);
                ce->setLabel("Введите конечный год рождения для интервала выборки. ");
                endYear = ce->setDataInt(1900, 2021);
                sdb->getgroups(groupsptr);
                sort(groups.begin(), groups.end());
                unique(groups.begin(), groups.end());
                sdb->sortbyname();

                groupsMenu->eraseAll();
                groupsMenu->addTitleItem("Выберите группу");
                groupsMenu->addItem("Выход");
                for (int i = 0; i < groups.size(); i++)
                {
                    string tmpString = groups[i];
                    groupsMenu->addItem(tmpString);
                }
                while (resultGr != exitGr) {
                    groupsMenu->run();
                    resultGr = groupsMenu->getSelectedItem();
                    if (resultGr == exitGr) {
                        break;
                    }
                    sesMenu->addItem("Выход");   //0
                    for (int i = 1; i < 10; i++) {
                        sesMenu->addItem("Сессия " + std::to_string(i));
                    }
                    while (resultSes != exitSes) {
                        sesMenu->eraseTitle();
                        sesMenu->addTitleItem("Выберите сессию");
                        sesMenu->run();
                        resultSes = sesMenu->getSelectedItem();
                        if (resultSes == exitSes) {
                            break;
                        }
                        sdb->showtaskres(groupsptr, indexesptr1, indexesptr2, resultGr, resultSes, startYear, endYear);
                        resultSes = exitSes;
                    }
                    sesMenu->eraseAll();
                    _getch();
                    resultGr = exitGr;
                }
                resultGr = 1;
                resultSes = 1;
                groupsMenu->eraseAll();
                indexes1.clear();
                indexes2.clear();
                

            }
        }
 }
