#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <filesystem>
#include <cstdint>

#define SSDSIZE 100	// SSD ũ��

const std::string SSD = "test.exe";	// SSD.exe
const std::string RESULTFILE = "result.txt";	// result.txt
const std::string TESTVALUE = "0x11111111";	// TESTVALUE: TestApp1�� �ʿ��� value
std::vector<std::string> fullReadValue;	// fullRead test�� ���Ǵ� ���� �����ϴ� ����
std::ofstream logFile;	// logFile ��ü
std::string logFileName = "latest.log";	// logFile �̸�

void SSDWrite(const int& idx, const std::string& data);	// LBA[idx]�� data�� ��
const std::string SSDRead(const int& idx);	// LBA[idx]�� data�� return

void SSDFullWrite(const std::string& data);	// LBA[0] ~ LBA[99]�� data�� ��
const bool SSDFullRead(const bool validChk, const int testValue);	// LBA[0] ~ LBA[99]�� data�� �а� ���, validChk�� true�� test�� ���, testValue�� true�� test �� ����

void SSDErase(const int& idx, const int& size);	// LBA[idx] ���� LBA[idx + size - 1]�� �����͸� ����(0x00000000)

void BUFFERFlush();	// ���ۿ� �ִ� ����� ����

const bool QueryChk(const int& idx);	// �����͸� �� ���� idx�� ���� ���� �� ���Դٸ� true return
const bool QueryChk(std::string& data);	// �������� ���� ������ ���̸� true return
const bool QueryChkErase(const int& idx, const int& size);	// �����͸� ���� ���� idx���� size���� �����ϴٸ� true return
const bool QueryChkEraseRange(const int& startIdx, const int& endIdx, int& idx, int& size);	// LBA[startIdx]���� LBA[endIdx]���� �����͸� ����µ� idx���� size�� ��ȯ

void Run_List(const std::string& fileName);	// fileName�� ���� �׽�Ʈ ���� �׽�Ʈ
const bool TestApp1();	// TestApp1 test
const bool TestApp2();	// TestApp2 test
const bool FullWriteReadCompare();	// FullWriteReadCompare test
const bool FullRead10AndCompare();	// FullRead10AndCompare test
const bool Write10AndCompare();	// Write10AndCompare test
const bool Loop_WriteAndReadCompare();	// Loop_WriteAndReadCompare test

void Log(const int logNum);	// logNum�� ���� �α� ���
const std::string DateTime(int type);	// ���� ��¥ �ð� ��ȯ

const std::string DateTime(int type) {	// type�� 0�̸� �α�, 1�̸� ���ϸ�

	std::time_t curr = std::time(nullptr);
	tm tm;

	localtime_s(&tm, &curr);
	std::string year = std::to_string((tm.tm_year + 1900) % 100);	// ��
	std::string month = (tm.tm_mon + 1) < 10 ? "0" + std::to_string(tm.tm_mon + 1) : std::to_string(tm.tm_mon + 1);	// ��
	std::string day = tm.tm_mday < 10 ? "0" + std::to_string(tm.tm_mday) : std::to_string(tm.tm_mday);	// ��
	std::string hour = tm.tm_hour < 10 ? "0" + std::to_string(tm.tm_hour) : std::to_string(tm.tm_hour);	// ��
	std::string minute = tm.tm_min < 10 ? "0" + std::to_string(tm.tm_min) : std::to_string(tm.tm_min);	// ��
	std::string second = tm.tm_sec < 10 ? "0" + std::to_string(tm.tm_sec) : std::to_string(tm.tm_sec);	// ��

	if (type == 0) { return "[" + year + "." + month + "." + day + "\t" + hour + ":" + minute + ":" + second + "]\t"; }
	else if (type == 1) { return "untile_" + year + month + day + "_" + hour + "h_" + minute + "m_" + second + "s.log"; }

	return "";
}

void Log(const int logNum) {

	std::string logMessage;

	if (logNum == 0) { 
		logFile.open(logFileName); 
		if (logFile.is_open()) {	// SSDStart	-> �Լ��� �ƴ����� �α� ���
			logMessage = DateTime(0) + "SSDStart()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 1) {
		if (logFile.is_open()) {	// SSDWrite
			logMessage = DateTime(0) + "SSDWrite()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 2) {
		if (logFile.is_open()) {	// SSDRead
			logMessage = DateTime(0) + "SSDRead()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 3) {
		if (logFile.is_open()) {	// SSDFullWrite
			logMessage = DateTime(0) + "SSDFullWrite()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 4) {
		if (logFile.is_open()) {	// SSDFullRead
			logMessage = DateTime(0) + "SSDFullRead()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 5) {
		if (logFile.is_open()) {	// SSDErase
			logMessage = DateTime(0) + "SSDErase()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 6) {
		if (logFile.is_open()) {	// Run_List
			logMessage = DateTime(0) + "Run_List()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 7) {
		if (logFile.is_open()) {	// FullWriteReadCompare
			logMessage = DateTime(0) + "FullWriteReadCompare()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 8) {
		if (logFile.is_open()) {	// FullRead10AndCompare
			logMessage = DateTime(0) + "FullRead10AndCompare()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 9) {
		if (logFile.is_open()) {	// Write10AndCompare
			logMessage = DateTime(0) + "Write10AndCompare()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 10) {
		if (logFile.is_open()) {	// Loop_WriteAndReadCompare
			logMessage = DateTime(0) + "Loop_WriteAndReadCompare()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if (logNum == 11) {
		if (logFile.is_open()) {	// BUFFERFlush
			logMessage = DateTime(0) + "BUFFERFlush()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
	}
	else if(logNum == -1) {
		if (logFile.is_open()) {	// exit -> �Լ��� �ƴ����� �α� ���
			logMessage = DateTime(0) + "exit()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
		logFile.close(); 
	}

	if (std::filesystem::file_size(logFileName) >= 10'000) {	// 10KB �̻��� ���
		logFile.close();	// ���� �ݱ�
		std::rename(logFileName.c_str(), DateTime(1).c_str());	// ���ϸ� ����
		logFile.open(logFileName);	// �ٽ� ���� ����
	}

	return;
}

void Run_List(const std::string& fileName) {

	Log(6);
	std::string testString = "";
	char test[30];

	std::ifstream testFile;
	testFile.open(fileName + ".txt");
	if (testFile.is_open()) {
		while (!testFile.eof()) {
			testFile.getline(test, 30);

			testString = test;

			std::cout << testString << "\t---\t";
			if (testString == "FullWriteReadCompare") {
				std::cout << "Run...\t";
				if (FullWriteReadCompare()) { std::cout << "Pass\n"; }
				else {
					std::cout << "FAIL!\n";
					break;
				}
			}
			else if (testString == "FullRead10AndCompare") {
				std::cout << "Run...\t";
				if (FullRead10AndCompare()) { std::cout << "Pass\n"; }
				else {
					std::cout << "FAIL!\n";
					break;
				}
			}
			else if (testString == "Write10AndCompare") {
				std::cout << "Run...\t";
				if (Write10AndCompare()) { std::cout << "Pass\n"; }
				else {
					std::cout << "FAIL!\n";
					break;
				}
			}
			else if (testString == "Loop_WriteAndReadCompare") {
				std::cout << "Run...\t";
				if (Loop_WriteAndReadCompare()) { std::cout << "Pass\n"; }
				else {
					std::cout << "FAIL!\n";
					break;
				}
			}
		}
	}
	testFile.close();

	return;
}

const bool TestApp1() {

	bool ret = true;

	SSDFullWrite(TESTVALUE);	// fullwrite ����
	ret = SSDFullRead(true, true);	// read�Ǵ��� Ȯ��

	return ret;
}

const bool TestApp2() {

	bool ret = true;

	for (int idx = 0; idx < 5; ++idx) {	// 0 ~ 5�� LBA��
		for (int iter = 0; iter < 30; ++iter) {	// 30�� write�� ����
			SSDWrite(idx, "0xAAAABBBB");
		}
	}

	for (int idx = 0; idx < 5; ++idx) {	// 0 ~ 5�� LBA�� over write�� ����
		SSDWrite(idx, "0x12345678");		
	}

	for (int idx = 0; idx < 5; ++idx) {	// 0 ~ 5�� LBA read ���� �� ���������� ���� �������� Ȯ��
		if (SSDRead(idx) != "0x12345678") {
			ret = false;
			break;
		}
	}

	return ret;
}

const bool FullWriteReadCompare() {

	Log(7);
	bool ret = true;

	fullReadValue.assign(100, "");	// fullReadValue �� �ʱ�ȭ �� ���� �Ҵ�
	SSDFullRead(true, 0);	// FullWriteReadCompare �׽�Ʈ �� �� ����

	SSDFullWrite(TESTVALUE);	// �׽�Ʈ ������ ������	--> FullWrite

	ret = SSDFullRead(true, -1);	// read�Ǵ��� Ȯ��	--> ReadCompare

	for (int idx = 0; idx < SSDSIZE; ++idx) { SSDWrite(idx, fullReadValue[idx]); }	// �� ����

	return ret;
}

const bool FullRead10AndCompare() {
	
	Log(8);
	fullReadValue.assign(100, "");	// fullReadValue �� �ʱ�ȭ �� ���� �Ҵ�

	SSDFullRead(true, 0);	// FullRead �� ����	--> FullRead 1

	for (int iter = 1; iter < 10; ++iter) {	// FullRead 9�� �߰� �ݺ�	--> FullRead 2 ~ 10
		SSDFullRead(true, iter);
	}

	return SSDFullRead(true, -2);	// Compare
}

const bool Write10AndCompare() {
	
	Log(9);
	bool ret = true;
	
	int randIdx = std::rand() % 100;	// randIdx: 10�� Write�� ������ ������ Idx
	std::string value = SSDRead(randIdx);	// value: �׽�Ʈ �� �� ����

	for (int iter = 0; iter < 10; ++iter) { SSDWrite(randIdx, TESTVALUE); }	// --> Write10

	if (SSDRead(randIdx) != TESTVALUE) { ret = false; }	// Compare

	SSDWrite(randIdx, value);	// �� ����

	return ret;
}

const bool Loop_WriteAndReadCompare() {	
	
	Log(10);
	for (int idx = 0; idx < SSDSIZE; ++idx) {	// Loop
		std::string value = SSDRead(idx);	// value: �� ����
		SSDWrite(idx, TESTVALUE);	// TestValue ������

		if (SSDRead(idx) != TESTVALUE) {	// compare
			SSDWrite(idx, value);	// �� ����
			return false;
		}
		SSDWrite(idx, value);	// �� ����
	}

	return true;
}

const bool QueryChk(const int& idx) {	

	if (idx >= 0 && idx < 100) { return true; }	// LBA�� 0 ~ 99 ������ ���� ����

	return false;
}

const bool QueryChk(std::string& data) {
	
	if (data[0] != '0') { return false; }	// data�� 0 ���� ����
	if (data[1] != 'x') { return false; }	// data�� 1�� �ε����� x
	if (data.length() != 10) { return false; }	// data�� ���̴� 10

	for (int i = 2; i < 10; ++i) {
		if (data[i] >= 'A' && data[i] <= 'F') { continue; }	// �빮��
		if (data[i] >= 'a' && data[i] <= 'f') { 
			data[i] = data[i] - 'a' + 'A';	// �ҹ��ڷ� ���� �����ʹ� �빮�ڷ� ����
			continue; 
		}
		if (data[i] >= '0' && data[i] <= '9') { continue; }	// ����

		return false;
	}

	return true;
}

const bool QueryChkErase(int& idx, int& size) {

	if (idx < 0 && idx >= 100) { return false; }	// �ε��� ���� Ȯ��
	if (size <= 0 || size > 10) { return false; }	// size�� �ִ� 10�� ���� �ƴ� ����

	if (idx + size > SSDSIZE) {	// SSDSIZE�� �Ѿ�� ���
		size += SSDSIZE - (idx + size);	// �ʰ����� �ʵ��� ����
		std::cout << "������ �ʰ��Ͽ� erase [" << idx << "] " << "[" << size << "] �� �����մϴ�\n";
	}

	return true;
}

const bool QueryChkEraseRange(const int& startIdx, const int& endIdx, int& idx, int& size) {

	if (startIdx < 0 && startIdx >= 100) { return false; }	// start �ε��� ���� ����
	if (endIdx < 0 && endIdx >= 100) { return false; }	// end �ε��� ���� ����
	if (startIdx > endIdx) { return false; }	// end �ε����� start �ε������� �׻� ŭ

	if (endIdx - startIdx > 10) {
		std::cout << "���� ���� �ʿ�(������ �� �ִ� �ִ� size�� 10ĭ �Դϴ�.)\n";
		return false;
	}

	idx = startIdx;
	size = endIdx - startIdx;

	return true;
}

void BUFFERFlush() {

	Log(11);
	std::string to_SSD = SSD + " " + "F";
	std::system(to_SSD.c_str());

	return;
}

void SSDWrite(const int& idx, const std::string& data) {	// write ���

	Log(1);
	std::string to_SSD = SSD + " " + "W" + " " + std::to_string(idx) + " " + data;
	std::system(to_SSD.c_str());

	return;
}

const std::string SSDRead(const int& idx) {	// read ���
	
	Log(2);
	std::string ret = "";	

	std::string to_SSD = SSD + " " + "R" + " " + std::to_string(idx);
	std::system(to_SSD.c_str());

	std::ifstream resultFile;
	resultFile.open(RESULTFILE);
	if (resultFile.is_open()) { std::getline(resultFile, ret); }
	resultFile.close();	

	return ret;
}

void SSDFullWrite(const std::string& data) {	// fullwrite ���

	Log(3);
	for (int i = 0; i < SSDSIZE; ++i) { SSDWrite(i, data); }

	return;
}

const bool SSDFullRead(const bool vaildChk, const int testValue) {	// fullread ���
	
	Log(4);
	for (int i = 0; i < SSDSIZE; ++i) { 
		std::string data = SSDRead(i);		

		if (vaildChk) {	// �׽�Ʈ�� ��� -> ����� ����
			if (testValue == -1) {
				if (data != TESTVALUE) { return false; }	// �׽�Ʈ ���� ���� ��ġ ���� �ʴ´ٸ� return false
			}
			else {	// �׽�Ʈ value�� ���� ���
				if (testValue == 0) { fullReadValue[i] = data; }	// ù ��° �� ����
				else if (testValue == -2) {
					if (fullReadValue[i] != data) { return false; }
				}			
			}
		}
		else {
			std::cout << i << "\t: " << data << '\n';
			std::cout << "\n";
		}
	}

	return true;
}

void SSDErase(const int& idx, const int& size) {

	Log(5);
	std::string to_SSD = SSD + " " + "E" + " " + std::to_string(idx) + " " + std::to_string(size);
	std::system(to_SSD.c_str());

	return;
}

int main() {

	std::string query;
	int idx, size;
	std::string data;
	std::string testFile;
	int startIdx, endIdx;		

	Log(0);	// �α׸� �ۼ��� ���� ����

	while (true) {
		
		std::cout << "\n>>>>>>> ";
		std::cin >> query;

		if (query == "write") {			
			std::cin >> idx >> data;
			if (QueryChk(idx) && QueryChk(data)) {				
				SSDWrite(idx, data);
			}
			else { std::cout << "write ����� �߸��� �Է�\n"; }
		}
		else if (query == "read") {
			std::cin >> idx;
			if (QueryChk(idx)) {
				std::string result = SSDRead(idx);
				std::cout << result << '\n';
			}
			else { std::cout << "read ����� �߸��� �Է�\n"; }
		}
		else if (query == "exit") {
			std::cout << "����!\n";
			break;
		}
		else if (query == "help") {
			std::cout << "write			[IDX]		[DATA]		-> IDX�� DATA�� ����\n";
			std::cout << "read			[IDX]				-> IDX�� DATA�� �д´�\n";
			std::cout << "exit							-> ����\n";
			std::cout << "help							-> ����\n";
			std::cout << "fullwrite		[DATA]				-> ��� ���� DATA�� ����\n";
			std::cout << "fullread						-> ��� ���� DATA�� �д´�\n";
			std::cout << "erase			[IDX]		[SIZE]		-> IDX���� SIZE���� �����͸� �����Ѵ�\n";
			std::cout << "erase_range		[START IDX]	[END IDX]	-> START IDX���� END IDX ���������� �����͸� �����Ѵ�\n";
			std::cout << "test			[FILE NAME]			-> FILE NAME�� ���� TEST RUNNER ����\n";
			std::cout << "flush							-> ���ۿ� ���� ��ɾ ����\n";
		}
		else if (query == "fullwrite") {
			std::cin >> data;
			if (QueryChk(data)) { SSDFullWrite(data); }
			else { std::cout << "fullwrite ����� �߸��� �Է�\n"; }
		}
		else if (query == "fullread") { SSDFullRead(false, -1); }
		else if (query == "erase") {
			std::cin >> idx >> size;
			if (QueryChkErase(idx, size)) { SSDErase(idx, size); }
			else { std::cout << "erase ����� �߸��� �Է�\n"; }
		}
		else if (query == "erase_range") {
			std::cin >> startIdx >> endIdx;
			if (QueryChkEraseRange(startIdx, endIdx, idx, size)) { SSDErase(idx, size); }
			else { std::cout << "erase_range ����� �߸��� �Է�\n"; }
		}
		else if (query == "test") {
			std::cin >> testFile;
			if (testFile == "testapp1") { TestApp1() ? std::cout << "Pass\n" : std::cout << "FAIL!\n"; }
			else if (testFile == "testapp2") { TestApp2() ? std::cout << "Pass\n" : std::cout << "FAIL\n"; }
			else if (testFile == "run_list") { Run_List(testFile); }
				
			
		}
		else if (query == "flush") { BUFFERFlush(); }
		else { std::cout << "INVALID COMMAND\n"; }
	}

	Log(-1);	// �α� ���� ����

	return 0;
}