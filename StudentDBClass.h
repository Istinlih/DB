#pragma once
#include "StudentClass.h"
#include <iostream>
#include "FileManagerClass.h"
#include "list.hpp"
#include <vector>
using namespace std;
class StudentDBClass : public FileManagerClass
{
private:

	//StudentNode* head;
	//StudentNode* getNextNode;
	//int count;

public:
	List <StudentNode> DataBase;
	StudentDBClass() {
		//head = NULL;
		//getNextNode = NULL;
		//count = 0;
		DataBase.clear();
	}
	~StudentDBClass() {
		DataBase.clear();
	}
	void loadDataFromFile() {
		string line;
		int count = 0;
		std::ifstream inFile(FileName); // окрываем файл для чтения
		if (inFile.is_open())
		{
			bool isRecord = false;
			StudentNode* sn = new StudentNode();
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 10; j++)
					sn->examsRecordsData[i][j].isEmpty = true;
			int studentId = 0;
			while (getline(inFile, line))
			{
				if (strcmp(startRecordString.c_str(), line.c_str()) == 0) {
					isRecord = true;
					continue;
				}
				if (strcmp(endRecordString.c_str(), line.c_str()) == 0) {
					isRecord = false;
					studentId++;
					DataBase.push_front(*sn);
					for (int i = 0; i < 9; i++)
						for (int j = 0; j < 10; j++)
							sn->examsRecordsData[i][j].isEmpty = true;
					continue;
				}
				if (isRecord) {

					string valueName = getName(line);
					if (strcmp(getType(line).c_str(), "str") == 0)
					{
						string value = getValueStr(line);
						//cout << value << endl;
						if (strcmp("surName", valueName.c_str()) == 0)
							sn->surName = value;
						if (strcmp("name", valueName.c_str()) == 0)
							sn->name = value;
						if (strcmp("middleName", valueName.c_str()) == 0)
							sn->middleName = value;
						if (strcmp("faculty", valueName.c_str()) == 0)
							sn->faculty = value;
						if (strcmp("department", valueName.c_str()) == 0)
							sn->department = value;
						if (strcmp("group", valueName.c_str()) == 0)
							sn->group = value;
						if (strcmp("recordСardNumber", valueName.c_str()) == 0)
							sn->recordСardNumber = value;
						if (strcmp("birthDateString", valueName.c_str()) == 0)
							sn->birthDateString = value;
						// ExamsRecords
						for (int i = 0; i < 9; i++)
							for (int j = 0; j < 10; j++) {
								//examsResults_0_2_n
											// i j
								string testNameString = "";
								testNameString = testNameString + "examsResults_" + std::to_string(i) + "_" + std::to_string(j) + "_n";
								string testMarkString = "";
								testMarkString = testMarkString + "examsResults_" + std::to_string(i) + "_" + std::to_string(j) + "_m";
								string testSpecString = "";
								testSpecString = testSpecString + "examsResults_" + std::to_string(i) + "_" + std::to_string(j) + "_s";
								if (strcmp(testNameString.c_str(), valueName.c_str()) == 0) {
									sn -> examsRecordsData[i][j].name = value;
									sn->examsRecordsData[i][j].isEmpty = false;
								}
								if (strcmp(testSpecString.c_str(), valueName.c_str()) == 0) {
									sn->examsRecordsData[i][j].special = atoi(value.c_str());
									sn->examsRecordsData[i][j].isEmpty = false;
								}
								if (strcmp(testMarkString.c_str(), valueName.c_str()) == 0) {
									sn->examsRecordsData[i][j].mark = atoi(value.c_str());
									sn->examsRecordsData[i][j].isEmpty = false;
								}
							}
					}
					else // Тогда явно int но надо проверить
					{
						int value = getValueInt(line);
						if (strcmp("id", valueName.c_str()) == 0)
							sn->id = studentId;
						if (strcmp("startYear", valueName.c_str()) == 0)
							sn->startYear = value;
						if (strcmp("sex", valueName.c_str()) == 0)
							if (value == 0)
								sn->sex = false;
							else
								sn->sex = true;
					}
				}
			}
			inFile.close();     // закрываем файл
		}
		else {
			cout << " \n Файл БД не найден \n";
			_getch();
		}
		inFile.close();
	}
	void saveDataToFile(string inFileName) {
		std::ofstream outFile;          // поток для записи
		ifstream iff(inFileName); //если файл есть удаляем
		if (iff.bad() == false)
		{
			iff.close();
			if (remove(inFileName.c_str())) {
				printf("Error removing file");
				_getch();
			}

		}
		outFile.open(inFileName, std::ios::app); // окрываем файл для записи
		if (outFile.is_open())
		{
			StudentClass st = StudentClass();
			int recordsCount = getRecordCount();
			StudentNode* sn;
			for (int i = 0; i < recordsCount; i++) {
				outFile << startRecordString << std::endl;
				sn = &DataBase.at(i);
				st.UpdateMasString(sn);
				while (!st.stringList.empty()) {
					outFile << st.stringList.at(0) << std::endl;
					st.stringList.erase(st.stringList.begin());
				}
				outFile << endRecordString << std::endl;
			}

		}
		outFile.close();
	}
	void setData(StudentNode* tNode, StudentNode* st) {
		tNode->surName = st->surName;
		tNode->name = st->name;
		tNode->middleName = st->middleName;
		tNode->faculty = st->faculty;
		tNode->department = st->department;
		tNode->group = st->group;
		tNode->recordСardNumber = st->recordСardNumber;
		tNode->sex = st->sex;
		tNode->startYear = st->startYear;
		tNode->birthDateString = st->birthDateString;
		tNode->avrMarks = st->avrMarks; ///!!!!
		// //по аналогии
		//ExamsRecords data[9][10];
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 10; j++) {
				tNode->examsRecordsData[i][j].isEmpty = st->examsRecordsData[i][j].isEmpty;
				tNode->examsRecordsData[i][j].name = st->examsRecordsData[i][j].name;
				tNode->examsRecordsData[i][j].special = st->examsRecordsData[i][j].special;
				tNode->examsRecordsData[i][j].mark = st->examsRecordsData[i][j].mark;
			}
	}

	void printAllSurName() {
		for (int i = 0; i < DataBase.size(); i++) {
			cout << DataBase.at(i).surName << endl;
		}
	}
	int getRecordCount() {
		return DataBase.size();
	}
	int getSameRecordСardNumber(string inString) {
		int count = 0;
		return count;
	}
	void updateAvrMarks() {
		StudentNode* curr = NULL;
		StudentClass* stud = new StudentClass();
		for (int i = 0; i < DataBase.size(); i++) {
			DataBase.at(i).avrMarks = stud->getAvrMarks(&DataBase.at(i));
			curr = curr->next;
		}
		delete stud;
	}
	void printAllSurName_Name_MName_bYaear_avrMarks() {
		StringBuilderClass* sb = new StringBuilderClass();
		for (int i = 0; i < DataBase.size(); i++) {
			cout << DataBase.at(i).surName + " " + DataBase.at(i).name + " " + DataBase.at(i).middleName + " " + sb->split(DataBase.at(i).birthDateString, '.', 2) + " " + std::to_string(DataBase.at(i).avrMarks) << endl;
		}
		delete sb;
	}

	double getMaxAvrMarks() {
		int max = 0;
		return max;

	}

	void getgroups(vector<string>* groups) {
		StudentNode* sn;
		StudentDBClass* sdb = new StudentDBClass();
		bool check;
		for (int i = 0; i < DataBase.size(); i++)
		{
			check = TRUE;
			for (int j = 0; j < groups->size(); j++) {
				if (groups->at(j) == DataBase.at(i).group) {
					check = FALSE;
				}
			}
			if (check) {
				groups->push_back(DataBase.at(i).group);
			}
		}
	}
	void sortbyname() {
		for (int i = 1; i < DataBase.size(); i++)
		{
			int j = i;
			string* key = &DataBase.at(j).surName;
			string* tmp = &DataBase.at(j - 1).surName;
			while ((strcmp(tmp->c_str(), key->c_str()) > 0)) {
				swap(DataBase.at(j - 1), DataBase.at(j));
				j -= 1;
				if (j > 0)
				{
					key = &DataBase.at(j).surName;
					tmp = &DataBase.at(j - 1).surName;
				}
			}
		}
	}
	int getBDYear(string tmp) {
		string styear;
		if (tmp.size() > 0) {
			styear.push_back(tmp[tmp.size() - 4]);
			styear.push_back(tmp[tmp.size() - 3]);
			styear.push_back(tmp[tmp.size() - 2]);
			styear.push_back(tmp[tmp.size() - 1]);
			return stoi(styear);
		}
		else {
			return -1;
		}
	}
	void showtaskres(vector<string>* groups, vector<int>* ind1, vector<int>* ind2, int selgr, int selsem, int byear, int eyear) {
		for (int i = 0; i < DataBase.size(); i++) {
			bool flag1part = 0, flag2part = 0, flagelse = 0;
			if ((getBDYear(DataBase.at(i).birthDateString) >= byear && (getBDYear(DataBase.at(i).birthDateString) <= eyear) && groups->at(selgr-1) == DataBase.at(i).group)) {
				for (int j = 0; j < 10; j++) {
					if (DataBase.at(i).examsRecordsData[selsem-1][j].special != 0) {
						if ((DataBase.at(i).examsRecordsData[selsem-1][j].mark == 4 || DataBase.at(i).examsRecordsData[selsem-1][j].mark == 5)) {
							flag1part = 1;
						}
						else if (DataBase.at(i).examsRecordsData[selsem-1][j].mark == 3) {
							flag2part = 1;
						}
						else if (DataBase.at(i).examsRecordsData[selsem-1][j].mark == 2 || DataBase.at(i).examsRecordsData[selsem-1][j].mark == 1 || DataBase.at(i).examsRecordsData[selsem-1][j].mark == 0) {
							flagelse = 1;
						}
					}
				}
				if (flagelse == 0) {
					if (flag1part == 1 && flag2part == 0) {
						ind1->push_back(i);
					}
					else if (flag1part == 1 && flag2part == 1) {
						ind2->push_back(i);
					}
				}
			}
		}
		cout << "\nСтуденты, сдавшие спецпредметы без 3: \n";
		for (int i = 0; i < ind1->size(); i++) {
			cout << DataBase.at(ind1->at(i)).surName << " "<< DataBase.at(ind1->at(i)).middleName << " " << DataBase.at(ind1->at(i)).name << '\n';
		}
		cout << "Студенты, сдавшие спецпредметы c 3: \n";
		for (int i = 0; i < ind2->size(); i++) {
			cout << DataBase.at(ind2->at(i)).surName << " " << DataBase.at(ind2->at(i)).middleName << " " << DataBase.at(ind2->at(i)).name << '\n';
		}
	}
};