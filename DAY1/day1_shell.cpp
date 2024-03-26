#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define SSDSIZE 100	// SSD 크기

const std::string SSD = "test.exe";	// SSD.exe
const std::string RESULTFILE = "result.txt";	// result.txt
const std::string TESTVALUE = "0x11111111";	// TESTVALUE: TestApp1에 필요한 value

void SSDWrite(const int& idx, const std::string& data);	// LBA[idx]에 data를 씀
std::string SSDRead(const int& idx);	// LBA[idx]의 data를 return

void SSDFullWrite(const std::string& data);	// LBA[0] ~ LBA[99]에 data를 씀
const bool SSDFullRead(const bool validChk);	// LBA[0] ~ LBA[99]의 data를 읽고 출력, validChk이 true면 test에 사용

const bool QueryChk(const int idx);	// 데이터를 쓸 곳인 idx가 범위 내로 잘 들어왔다면 true return
const bool QueryChk(std::string& data);	// 데이터의 값이 가능한 값이면 true return

const bool TestApp1();	// TestApp1
const bool TestApp2();	// TestApp2

const bool TestApp1() {

	bool ret = true;

	SSDFullWrite(TESTVALUE);	// fullwrite 수행
	ret = SSDFullRead(true);	// read되는지 확인

	return ret;
}

const bool TestApp2() {

	bool ret = true;

	for (int idx = 0; idx < 5; ++idx) {	// 0 ~ 5번 LBA에
		for (int iter = 0; iter < 30; ++iter) {	// 30번 write을 수행
			SSDWrite(idx, "0xAAAABBBB");
		}
	}

	for (int idx = 0; idx < 5; ++idx) {	// 0 ~ 5번 LBA에 over write를 수행
		SSDWrite(idx, "0x12345678");		
	}

	for (int idx = 0; idx < 5; ++idx) {	// 0 ~ 5번 LBA read 했을 때 정상적으로 값이 읽히는지 확인
		if (SSDRead(idx) != "0x12345678") {
			ret = false;
			break;
		}
	}

	return ret;
}



const bool QueryChk(const int idx) {	
	if (idx >= 0 && idx < 100) { return true; }	// LBA는 0 ~ 99 사이의 값을 가짐
	return false;
}

const bool QueryChk(std::string& data) {
	
	if (data[0] != '0') { return false; }	// data는 0 으로 시작
	if (data[1] != 'x') { return false; }	// data의 1번 인덱스는 x
	if (data.length() != 10) { return false; }	// data의 길이는 10

	for (int i = 2; i < 10; ++i) {
		if (data[i] >= 'A' && data[i] <= 'F') { continue; }	// 대문자
		if (data[i] >= 'a' && data[i] <= 'f') { 
			data[i] = data[i] - 'a' + 'A';	// 소문자로 들어온 데이터는 대문자로 변경
			continue; 
		}
		if (data[i] >= '0' && data[i] <= '9') { continue; }	// 숫자

		return false;
	}

	return true;
}

void SSDWrite(const int& idx, const std::string& data) {	// write 명령

	std::string to_SSD = SSD + " " + "W" + " " + std::to_string(idx) + " " + data;
	std::system(to_SSD.c_str());

	return;
}

std::string SSDRead(const int& idx) {	// read 명령
	
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

void SSDFullWrite(const std::string& data) {	// fullwrite 명령

	for (int i = 0; i < SSDSIZE; ++i) { SSDWrite(i, data); }

	return;
}

const bool SSDFullRead(const bool vaildChk) {	// fullread 명령

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
			else { std::cout << "write 명령의 잘못된 입력\n"; }
		}
		else if (query == "read") {
			std::cin >> idx;
			if (QueryChk(idx)) {
				std::string result = SSDRead(idx);
				std::cout << result << '\n';
			}
			else { std::cout << "read 명령의 잘못된 입력\n"; }
		}
		else if (query == "exit") {
			std::cout << "종료!\n";
			break;
		}
		else if (query == "help") {
			std::cout << "write		[IDX]	[DATA]		-> IDX에 DATA를 쓴다\n";
			std::cout << "read		[IDX]			-> IDX에 DATA를 읽는다\n";
			std::cout << "exit					-> 종료\n";
			std::cout << "help					-> 도움말\n";
			std::cout << "fullwrite	[DATA]			-> 모든 곳에 DATA를 쓴다\n";
			std::cout << "fullread				-> 모든 곳의 DATA를 읽는다\n";
		}
		else if (query == "fullwrite") {
			std::cin >> data;
			if (QueryChk(data)) { SSDFullWrite(data); }
			else { std::cout << "fullwrite 명령의 잘못된 입력\n"; }
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