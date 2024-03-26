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

#define SSDSIZE 100	// SSD 크기

const std::string SSD = "test.exe";	// SSD.exe
const std::string RESULTFILE = "result.txt";	// result.txt
const std::string TESTVALUE = "0x11111111";	// TESTVALUE: TestApp1에 필요한 value
std::vector<std::string> fullReadValue;	// fullRead test에 사용되는 값을 저장하는 변수
std::ofstream logFile;	// logFile 객체
std::string logFileName = "latest.log";	// logFile 이름

void SSDWrite(const int& idx, const std::string& data);	// LBA[idx]에 data를 씀
const std::string SSDRead(const int& idx);	// LBA[idx]의 data를 return

void SSDFullWrite(const std::string& data);	// LBA[0] ~ LBA[99]에 data를 씀
const bool SSDFullRead(const bool validChk, const int testValue);	// LBA[0] ~ LBA[99]의 data를 읽고 출력, validChk이 true면 test에 사용, testValue가 true면 test 값 있음

void SSDErase(const int& idx, const int& size);	// LBA[idx] 부터 LBA[idx + size - 1]의 데이터를 지움(0x00000000)

void BUFFERFlush();	// 버퍼에 있는 명령을 실행

const bool QueryChk(const int& idx);	// 데이터를 쓸 곳인 idx가 범위 내로 잘 들어왔다면 true return
const bool QueryChk(std::string& data);	// 데이터의 값이 가능한 값이면 true return
const bool QueryChkErase(const int& idx, const int& size);	// 데이터를 지울 곳이 idx부터 size개가 가능하다면 true return
const bool QueryChkEraseRange(const int& startIdx, const int& endIdx, int& idx, int& size);	// LBA[startIdx]부터 LBA[endIdx]까지 데이터를 지우는데 idx부터 size로 변환

void Run_List(const std::string& fileName);	// fileName을 갖는 테스트 파일 테스트
const bool TestApp1();	// TestApp1 test
const bool TestApp2();	// TestApp2 test
const bool FullWriteReadCompare();	// FullWriteReadCompare test
const bool FullRead10AndCompare();	// FullRead10AndCompare test
const bool Write10AndCompare();	// Write10AndCompare test
const bool Loop_WriteAndReadCompare();	// Loop_WriteAndReadCompare test

void Log(const int logNum);	// logNum에 따른 로그 출력
const std::string DateTime(int type);	// 현재 날짜 시간 반환

const std::string DateTime(int type) {	// type이 0이면 로그, 1이면 파일명

	std::time_t curr = std::time(nullptr);
	tm tm;

	localtime_s(&tm, &curr);
	std::string year = std::to_string((tm.tm_year + 1900) % 100);	// 년
	std::string month = (tm.tm_mon + 1) < 10 ? "0" + std::to_string(tm.tm_mon + 1) : std::to_string(tm.tm_mon + 1);	// 월
	std::string day = tm.tm_mday < 10 ? "0" + std::to_string(tm.tm_mday) : std::to_string(tm.tm_mday);	// 일
	std::string hour = tm.tm_hour < 10 ? "0" + std::to_string(tm.tm_hour) : std::to_string(tm.tm_hour);	// 시
	std::string minute = tm.tm_min < 10 ? "0" + std::to_string(tm.tm_min) : std::to_string(tm.tm_min);	// 분
	std::string second = tm.tm_sec < 10 ? "0" + std::to_string(tm.tm_sec) : std::to_string(tm.tm_sec);	// 초

	if (type == 0) { return "[" + year + "." + month + "." + day + "\t" + hour + ":" + minute + ":" + second + "]\t"; }
	else if (type == 1) { return "untile_" + year + month + day + "_" + hour + "h_" + minute + "m_" + second + "s.log"; }

	return "";
}

void Log(const int logNum) {

	std::string logMessage;

	if (logNum == 0) { 
		logFile.open(logFileName); 
		if (logFile.is_open()) {	// SSDStart	-> 함수는 아니지만 로그 출력
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
		if (logFile.is_open()) {	// exit -> 함수는 아니지만 로그 출력
			logMessage = DateTime(0) + "exit()\t\n";
			logFile.write(logMessage.c_str(), logMessage.length());
		}
		logFile.close(); 
	}

	if (std::filesystem::file_size(logFileName) >= 10'000) {	// 10KB 이상인 경우
		logFile.close();	// 파일 닫기
		std::rename(logFileName.c_str(), DateTime(1).c_str());	// 파일명 변경
		logFile.open(logFileName);	// 다시 파일 열기
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

	SSDFullWrite(TESTVALUE);	// fullwrite 수행
	ret = SSDFullRead(true, true);	// read되는지 확인

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

const bool FullWriteReadCompare() {

	Log(7);
	bool ret = true;

	fullReadValue.assign(100, "");	// fullReadValue 값 초기화 및 공간 할당
	SSDFullRead(true, 0);	// FullWriteReadCompare 테스트 전 값 저장

	SSDFullWrite(TESTVALUE);	// 테스트 값으로 덮어씌우기	--> FullWrite

	ret = SSDFullRead(true, -1);	// read되는지 확인	--> ReadCompare

	for (int idx = 0; idx < SSDSIZE; ++idx) { SSDWrite(idx, fullReadValue[idx]); }	// 값 복원

	return ret;
}

const bool FullRead10AndCompare() {
	
	Log(8);
	fullReadValue.assign(100, "");	// fullReadValue 값 초기화 및 공간 할당

	SSDFullRead(true, 0);	// FullRead 값 저장	--> FullRead 1

	for (int iter = 1; iter < 10; ++iter) {	// FullRead 9번 추가 반복	--> FullRead 2 ~ 10
		SSDFullRead(true, iter);
	}

	return SSDFullRead(true, -2);	// Compare
}

const bool Write10AndCompare() {
	
	Log(9);
	bool ret = true;
	
	int randIdx = std::rand() % 100;	// randIdx: 10번 Write를 수행할 임의의 Idx
	std::string value = SSDRead(randIdx);	// value: 테스트 전 값 저장

	for (int iter = 0; iter < 10; ++iter) { SSDWrite(randIdx, TESTVALUE); }	// --> Write10

	if (SSDRead(randIdx) != TESTVALUE) { ret = false; }	// Compare

	SSDWrite(randIdx, value);	// 값 복원

	return ret;
}

const bool Loop_WriteAndReadCompare() {	
	
	Log(10);
	for (int idx = 0; idx < SSDSIZE; ++idx) {	// Loop
		std::string value = SSDRead(idx);	// value: 값 저장
		SSDWrite(idx, TESTVALUE);	// TestValue 덮어씌우기

		if (SSDRead(idx) != TESTVALUE) {	// compare
			SSDWrite(idx, value);	// 값 복원
			return false;
		}
		SSDWrite(idx, value);	// 값 복원
	}

	return true;
}

const bool QueryChk(const int& idx) {	

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

const bool QueryChkErase(int& idx, int& size) {

	if (idx < 0 && idx >= 100) { return false; }	// 인덱스 범위 확인
	if (size <= 0 || size > 10) { return false; }	// size는 최대 10의 음이 아닌 정수

	if (idx + size > SSDSIZE) {	// SSDSIZE를 넘어가는 경우
		size += SSDSIZE - (idx + size);	// 초과하지 않도록 조절
		std::cout << "범위를 초과하여 erase [" << idx << "] " << "[" << size << "] 로 변경합니다\n";
	}

	return true;
}

const bool QueryChkEraseRange(const int& startIdx, const int& endIdx, int& idx, int& size) {

	if (startIdx < 0 && startIdx >= 100) { return false; }	// start 인덱스 범위 조절
	if (endIdx < 0 && endIdx >= 100) { return false; }	// end 인덱스 범위 조절
	if (startIdx > endIdx) { return false; }	// end 인덱스는 start 인덱스보다 항상 큼

	if (endIdx - startIdx > 10) {
		std::cout << "범위 조정 필요(삭제할 수 있는 최대 size는 10칸 입니다.)\n";
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

void SSDWrite(const int& idx, const std::string& data) {	// write 명령

	Log(1);
	std::string to_SSD = SSD + " " + "W" + " " + std::to_string(idx) + " " + data;
	std::system(to_SSD.c_str());

	return;
}

const std::string SSDRead(const int& idx) {	// read 명령
	
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

void SSDFullWrite(const std::string& data) {	// fullwrite 명령

	Log(3);
	for (int i = 0; i < SSDSIZE; ++i) { SSDWrite(i, data); }

	return;
}

const bool SSDFullRead(const bool vaildChk, const int testValue) {	// fullread 명령
	
	Log(4);
	for (int i = 0; i < SSDSIZE; ++i) { 
		std::string data = SSDRead(i);		

		if (vaildChk) {	// 테스트의 경우 -> 출력이 없음
			if (testValue == -1) {
				if (data != TESTVALUE) { return false; }	// 테스트 값과 값이 일치 하지 않는다면 return false
			}
			else {	// 테스트 value가 없는 경우
				if (testValue == 0) { fullReadValue[i] = data; }	// 첫 번째 값 저장
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

	Log(0);	// 로그를 작성할 파일 열기

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
			std::cout << "write			[IDX]		[DATA]		-> IDX에 DATA를 쓴다\n";
			std::cout << "read			[IDX]				-> IDX에 DATA를 읽는다\n";
			std::cout << "exit							-> 종료\n";
			std::cout << "help							-> 도움말\n";
			std::cout << "fullwrite		[DATA]				-> 모든 곳에 DATA를 쓴다\n";
			std::cout << "fullread						-> 모든 곳의 DATA를 읽는다\n";
			std::cout << "erase			[IDX]		[SIZE]		-> IDX부터 SIZE개의 데이터를 삭제한다\n";
			std::cout << "erase_range		[START IDX]	[END IDX]	-> START IDX부터 END IDX 이전까지의 데이터를 삭제한다\n";
			std::cout << "test			[FILE NAME]			-> FILE NAME을 갖는 TEST RUNNER 실행\n";
			std::cout << "flush							-> 버퍼에 쌓인 명령어를 실행\n";
		}
		else if (query == "fullwrite") {
			std::cin >> data;
			if (QueryChk(data)) { SSDFullWrite(data); }
			else { std::cout << "fullwrite 명령의 잘못된 입력\n"; }
		}
		else if (query == "fullread") { SSDFullRead(false, -1); }
		else if (query == "erase") {
			std::cin >> idx >> size;
			if (QueryChkErase(idx, size)) { SSDErase(idx, size); }
			else { std::cout << "erase 명령의 잘못된 입력\n"; }
		}
		else if (query == "erase_range") {
			std::cin >> startIdx >> endIdx;
			if (QueryChkEraseRange(startIdx, endIdx, idx, size)) { SSDErase(idx, size); }
			else { std::cout << "erase_range 명령의 잘못된 입력\n"; }
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

	Log(-1);	// 로그 파일 종료

	return 0;
}