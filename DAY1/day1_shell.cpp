#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define SSDSIZE 100	// SSD ũ��

const std::string SSD = "test.exe";	// SSD.exe
const std::string RESULTFILE = "result.txt";	// result.txt
const std::string TESTVALUE = "0x11111111";	// TESTVALUE: TestApp1�� �ʿ��� value

void SSDWrite(const int& idx, const std::string& data);	// LBA[idx]�� data�� ��
std::string SSDRead(const int& idx);	// LBA[idx]�� data�� return

void SSDFullWrite(const std::string& data);	// LBA[0] ~ LBA[99]�� data�� ��
const bool SSDFullRead(const bool validChk);	// LBA[0] ~ LBA[99]�� data�� �а� ���, validChk�� true�� test�� ���

const bool QueryChk(const int idx);	// �����͸� �� ���� idx�� ���� ���� �� ���Դٸ� true return
const bool QueryChk(std::string& data);	// �������� ���� ������ ���̸� true return

const bool TestApp1();	// TestApp1
const bool TestApp2();	// TestApp2

const bool TestApp1() {

	bool ret = true;

	SSDFullWrite(TESTVALUE);	// fullwrite ����
	ret = SSDFullRead(true);	// read�Ǵ��� Ȯ��

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



const bool QueryChk(const int idx) {	
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

void SSDWrite(const int& idx, const std::string& data) {	// write ���

	std::string to_SSD = SSD + " " + "W" + " " + std::to_string(idx) + " " + data;
	std::system(to_SSD.c_str());

	return;
}

std::string SSDRead(const int& idx) {	// read ���
	
	std::string ret = "";
	char result[11];

	std::string to_SSD = SSD + " " + "R" + " " + std::to_string(idx);
	std::system(to_SSD.c_str());

	std::ifstream resultFile;
	resultFile.open(RESULTFILE);
	if (resultFile.is_open()) {
		
		resultFile.getline(result, 11);
	}
	resultFile.close();
	
	ret = result;

	return ret;
}

void SSDFullWrite(const std::string& data) {	// fullwrite ���

	for (int i = 0; i < SSDSIZE; ++i) { SSDWrite(i, data); }

	return;
}

const bool SSDFullRead(const bool vaildChk) {	// fullread ���

	bool ret = true;

	for (int i = 0; i < SSDSIZE; ++i) { 
		std::string data = SSDRead(i);
		std::cout << i << "\t: " << data << '\n';

		if (vaildChk) {
			if (data != TESTVALUE) {
				std::cout << "error\n";
				ret = false;
			}
			else { std::cout << "\n"; }			
		}
	}

	return ret;
}

int main() {

	std::string query;
	int idx;
	std::string data;
	std::string testFile;

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
			std::cout << "write		[IDX]	[DATA]		-> IDX�� DATA�� ����\n";
			std::cout << "read		[IDX]			-> IDX�� DATA�� �д´�\n";
			std::cout << "exit					-> ����\n";
			std::cout << "help					-> ����\n";
			std::cout << "fullwrite	[DATA]			-> ��� ���� DATA�� ����\n";
			std::cout << "fullread				-> ��� ���� DATA�� �д´�\n";
		}
		else if (query == "fullwrite") {
			std::cin >> data;
			if (QueryChk(data)) { SSDFullWrite(data); }
			else { std::cout << "fullwrite ����� �߸��� �Է�\n"; }
		}
		else if (query == "fullread") { SSDFullRead(false); }
		else if (query == "test") {
			std::cin >> testFile;
			if (testFile == "testapp1") { TestApp1() ? std::cout << "success\n" : std::cout << "failure\n"; }
			else if (testFile == "testapp2") { TestApp2() ? std::cout << "success\n" : std::cout << "failure\n"; }
		}
		else { std::cout << "INVALID COMMAND\n"; }
	}

	return 0;
}