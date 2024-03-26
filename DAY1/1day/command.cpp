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
	string command = argv[1]; //��ɾ�
	//write
	if (command == "W") {
		int lbaAddress = convertString2Int(argv[2]); //lba ���� �ּ�
		string data = argv[3];

		//validate
		if (lbaAddress == -1 || !checkNumberRange(lbaAddress)) {
			cout << "error:: lba �ּ� �߸��� �Է�";
			return 0;
		}
		if (!isValidHex(data)) {
			cout << "error:: ���� ������ �߸��� �Է�";
			return 0;
		}
		writeLBA(lbaAddress, data);
	}
	//read
	else if (command == "R") {
		int lbaAddress = convertString2Int(argv[2]); //lba ���� �ּ�

		//validate
		if (lbaAddress == -1 || !checkNumberRange(lbaAddress)) {
			cout << "error:: lba �ּ� �߸��� �Է�";
			return 0;
		}
		readLBA(lbaAddress);
		
	}
	//�߸��� ��ɾ�
	else {
		cout << "error:: command �߸��� �Է�";
		return 0;
	}
}