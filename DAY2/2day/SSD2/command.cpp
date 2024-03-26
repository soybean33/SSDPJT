#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <map>

using namespace std;

const string BUFFER_FILE = "command_buffer.txt";
const int MAX_BUFFER_SIZE = 10;

enum CommandType {
    WRITE,
    READ,
    ERASE,
    FLUSH,
    INVALID
};

struct Command {
    CommandType type;
    int lba;
    int size;
    string value;
};

queue<Command> commandBuffer; //Ŀ��� �����ϴ� ����
map<int, string> optimizedCommands; //Ŀ���� ����ȭ ������ ���� map

//validate
bool checkNumberRange(const int& lbaaddress); //lba �ּ� ���� 0�̻� 99����
bool checkSizeRange(const int& size); //erase ���� �� �ִ� ���� 1�̻� 10����
bool isValidHex(const string& data); //16���� �´��� üũ

//fileIO
void writeLBA(int lba, string& value); //����
void readLBA(int lba); //�б� (nand ���Ͽ��� ���� �о��)
void readLBA(int lba, string data); //�б� (���� �ٷ� result�� ����)
void eraseLBA(int lba, int size); //�����

//command
void loadBufferFromFile(); //���Ͽ��� ��ɾ� ���� �о����. �� �� ������ ��ɾ���� �ҷ��´�.
CommandType parseCommandType(const string& cmdType); // commandType ��Ī
void addCommandToBuffer(const Command& cmd); //��ɾ ���ۿ� ������, ���� ����� Flush�� ������ ��ɾ�� ����
void optimizeCommands(); //��ɾ���� ����ȭ�ϴ� �Լ�
void processCommands(int readResult); //�б�, ����, ����� file �Լ��� �����ϴ� �Լ�, ����ȭ�� ��ɾ���� �����Ѵ�.
void saveBufferToFile(const Command& cmd); //���� ��ɾ ���� ���Ͽ� �����ϴ� �Լ�


// ���Ͽ��� ��ɾ� ���� �о����. �� �� ������ ��ɾ���� �ҷ��´�.
void loadBufferFromFile() {
    ifstream inFile(BUFFER_FILE, ios::in);
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        Command cmd;
        int cmdType;
        ss >> cmdType >> cmd.lba >> cmd.size >> cmd.value;
        cmd.type = static_cast<CommandType>(cmdType);
        commandBuffer.push(cmd);
    }
    inFile.close();
}

// commandType ��Ī
CommandType parseCommandType(const string& cmdType) {
    if (cmdType == "W") return WRITE;
    if (cmdType == "R") return READ;
    if (cmdType == "E") return ERASE;
    if (cmdType == "F") return FLUSH;
    return INVALID;
}

/*
    ��ɾ ���ۿ� ������,
    ���� ����� Flush, read�� ���� ���) ����ȭ �Լ� -> ����ȭ�� ��ɾ� ó������
    �ƴ� ���) ���� ������ ���� ���Ͽ� �����ϴ� �Լ�
*/
void addCommandToBuffer(const Command& cmd) {
    if (cmd.type != READ) {
        commandBuffer.push(cmd);
    }
    if (cmd.type == READ || cmd.type == FLUSH || commandBuffer.size() >= MAX_BUFFER_SIZE) {// ���۰� ���� ���ų� FLUSH ��ɾ ������ ��ɾ� ó��
        optimizeCommands();
        processCommands(cmd.lba);
        return;
    }
    saveBufferToFile(cmd);
}

/*
    ��ɾ���� ����ȭ�ϴ� �Լ�
    write, erase : map�� ���� ��ȭ�ϴ� �������� ������� �����Ͽ� ���� ������ �����͸� ã�´�.
*/
void optimizeCommands() {
    while (!commandBuffer.empty()) {
        Command cmd = commandBuffer.front();
        commandBuffer.pop();

        switch (cmd.type) {
        case WRITE:
            optimizedCommands[cmd.lba] = cmd.value;
            break;
        case READ:
            break;
        case ERASE:
            for (int i = cmd.lba; i < min(cmd.lba + cmd.size, 100); i++) {
                optimizedCommands[i] = "0x00000000";
            }
            break;
        default:
            break;
        }
    }

    //���� ����
    ofstream outFile(BUFFER_FILE, ios::out | ios::trunc); // ������ ���� ���� ������ ����
    outFile.close();
}

/*
    �б�, ����, ����� file �Լ��� �����ϴ� �Լ�, ����ȭ�� ��ɾ���� �����Ѵ�.
    write, erase : ����ȭ�� ������� ������ �ִ� map�� ���� write�� ����
    read : map�� �а� ���� LBA�ּҰ� �ִٸ� map�� ���ؼ� Ȯ���Ѵ�.
            ����, ���ٸ� �״�� nand.txt���� ���ؼ� Ȯ���Ѵ�.
*/
void processCommands(int readResult) {
    for (auto op : optimizedCommands) {
        writeLBA(op.first, op.second);
    }
    if (readResult >= 0 && readResult < 100) {
        auto dataIter = optimizedCommands.find(readResult);
        if (dataIter != optimizedCommands.end()) { //�Է¿� ���� ����ִٸ� nand�� ���� �ʰ� �ٷ� result
            readLBA(readResult, dataIter->second);
            return;
        }
        readLBA(readResult);
    }
}

// ���� ��ɾ ���� ���Ͽ� �����ϴ� �Լ�
void saveBufferToFile(const Command& cmd) {
    ofstream outFile(BUFFER_FILE, ios::app);
    outFile << cmd.type << " " << cmd.lba << " " << cmd.size << " " << cmd.value << endl;
    outFile.close();
}

int main(int argc, char* argv[]) {
    loadBufferFromFile(); // ���α׷� ���� �� ���� ���� �ҷ�����

    string cmdTypeStr(argv[1]);
    CommandType cmdType = parseCommandType(cmdTypeStr);

    Command cmd;
    cmd.type = cmdType;
    if (cmdType == WRITE || cmdType == READ || cmdType == ERASE) {
        if (argc < 3) {
            cerr << "Insufficient arguments for command " << cmdTypeStr << endl;
            return 1;
        }
        cmd.lba = stoi(argv[2]);
        if (!checkNumberRange(cmd.lba)) {
            cout << "error:: lba �ּ� �߸��� �Է�";
            return 0;
        }
        if (cmdType == WRITE) {
            if (argc < 4) {
                cerr << "Insufficient arguments for WRITE command" << endl;
                return 1;
            }
            cmd.value = argv[3];
            if (!isValidHex(cmd.value)) {
                cout << "error:: ���� ������ �߸��� �Է�";
                return 0;
            }
        }
        else if (cmdType == ERASE) {
            if (argc < 4) {
                cerr << "Insufficient arguments for ERASE command" << endl;
                return 1;
            }
            cmd.size = stoi(argv[3]);
            if (!checkSizeRange(cmd.size)) {
                cout << "error:: �߸��� ������ �Է�";
                return 0;
            }
        }
    }
    
    addCommandToBuffer(cmd); //���� �� ����

    return 0;
}
