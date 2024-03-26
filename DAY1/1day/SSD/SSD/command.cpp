#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>

using namespace std;

int convertString2Int(const string& lbaAddress);
bool checkNumberRange(const int& lbaAddress);
bool isValidHex(const string& data);

void writeLBA(int lba, string& value);
void readLBA(int lba);


int main(int argc, char* argv[]) {
	string command = argv[1]; //疙飞绢
	//write
	if (command == "W") {
		int lbaAddress = convertString2Int(argv[2]); //lba 康开 林家
		string data = argv[3];

		//validate
		if (lbaAddress == -1 || !checkNumberRange(lbaAddress)) {
			cout << "error:: lba 林家 肋给等 涝仿";
			return 0;
		}
		if (!isValidHex(data)) {
			cout << "error:: 历厘 单捞磐 肋给等 涝仿";
			return 0;
		}
		writeLBA(lbaAddress, data);
	}
	//read
	else if (command == "R") {
		int lbaAddress = convertString2Int(argv[2]); //lba 康开 林家

		//validate
		if (lbaAddress == -1 || !checkNumberRange(lbaAddress)) {
			cout << "error:: lba 林家 肋给等 涝仿";
			return 0;
		}
		readLBA(lbaAddress);
		
	}
	//肋给等 疙飞绢
	else {
		cout << "error:: command 肋给等 涝仿";
		return 0;
	}
}