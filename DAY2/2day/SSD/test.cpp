#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <map>
#include "validate.cpp"
#include "fileIO.cpp"

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

class CommandProcessor {
private:
    queue<Command> commandBuffer; // 커멘드 저장하는 버퍼
    map<int, string> optimizedCommands; // 커멘드들 최적화 저장을 위한 map
    int readResult; // 최종적으로 읽을 데이터

    // 싱글톤 관련 부분
    static CommandProcessor* instance;
    CommandProcessor() { readResult = -1; }

public:
    // 싱글톤 인스턴스 반환 메소드
    static CommandProcessor* getInstance();

    // 싱글톤 삭제 방지
    CommandProcessor(CommandProcessor& other) = delete;
    void operator=(const CommandProcessor&) = delete;

    //command
    CommandType parseCommandType(const string& cmdType); // commandType 매칭
    void addCommandToBuffer(const Command& cmd); //명령어를 버퍼에 저장함, 버퍼 사이즈나 Flush를 만나면 명령어들 실행
    void optimizeCommands(); //명령어들을 최적화하는 함수
    void processCommands(); //읽기, 쓰기, 지우기 file 함수에 접근하는 함수, 최적화된 명령어들을 실행한다.
    void saveBufferToFile(const Command& cmd); //다음 명령어를 위해 파일에 저장하는 함수
    void loadBufferFromFile(); //파일에서 명령어 버퍼 읽어오기. 그 전 실행의 명령어들을 불러온다.
};

CommandProcessor* CommandProcessor::instance = nullptr;

CommandProcessor* CommandProcessor::getInstance() {
    if (instance == nullptr) {
        instance = new CommandProcessor();
    }
    return instance;
}

// 파일에서 명령어 버퍼 읽어오기. 그 전 실행의 명령어들을 불러온다.
void CommandProcessor::loadBufferFromFile() {
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

// commandType 매칭
CommandType CommandProcessor::parseCommandType(const string& cmdType) {
    if (cmdType == "W") return WRITE;
    if (cmdType == "R") return READ;
    if (cmdType == "E") return ERASE;
    if (cmdType == "F") return FLUSH;
    return INVALID;
}

/*
    명령어를 버퍼에 저장함, 
    버퍼 사이즈나 Flush를 만날 경우) 최적화 함수 -> 최적화된 명령어 처리실행
    아닐 경우) 다음 실행을 위해 파일에 저장하는 함수
*/
void CommandProcessor::addCommandToBuffer(const Command & cmd) {
    commandBuffer.push(cmd);
    if (cmd.type == FLUSH || commandBuffer.size() >= MAX_BUFFER_SIZE) {// 버퍼가 가득 차거나 FLUSH 명령어를 만나면 명령어 처리
        optimizeCommands();
        processCommands();
        return;
    }
    saveBufferToFile(cmd);
}

/*
    명령어들을 최적화하는 함수
    write, erase : map을 통해 변화하는 데이터의 결과값을 저장하여 최종 저장할 데이터를 찾는다.
    read : 마지막으로 읽는 LBA주소를 저장한다.
*/
void CommandProcessor::optimizeCommands() {
    readResult = -1;
    while (!commandBuffer.empty()) {
        Command cmd = commandBuffer.front();
        commandBuffer.pop();

        switch (cmd.type) {
        case WRITE:
            optimizedCommands[cmd.lba] = cmd.value;
            break;
        case READ:
            readResult = cmd.lba;
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

    //파일 비우기
    ofstream outFile(BUFFER_FILE, ios::out | ios::trunc); // 파일을 열어 기존 내용을 삭제
    outFile.close();
}

/*
    읽기, 쓰기, 지우기 file 함수에 접근하는 함수, 최적화된 명령어들을 실행한다.
    write, erase : 최적화된 결과값을 가지고 있는 map을 통해 write를 진행
    read : map에 읽고 싶은 LBA주소가 있다면 map을 통해서 확인한다.
            만약, 없다면 그대로 nand.txt파일 통해서 확인한다.
*/
void CommandProcessor::processCommands() {
    FileManager* fileManager = FileManager::getInstance();
    for (auto op : optimizedCommands) {
        fileManager->writeLBA(op.first, op.second);
    }
    if (readResult != -1) {
        auto dataIter = optimizedCommands.find(readResult);
        if (dataIter != optimizedCommands.end()) { //입력에 값이 들어있다면 nand를 읽지 않고 바로 result
            fileManager->readLBA(readResult, dataIter->second);
            return;
        }
        fileManager->readLBA(readResult);
    }
}

// 다음 명령어를 위해 파일에 저장하는 함수
void CommandProcessor::saveBufferToFile(const Command& cmd) {
    ofstream outFile(BUFFER_FILE, ios::app);
    outFile << cmd.type << " " << cmd.lba << " " << cmd.size << " " << cmd.value << endl;
    outFile.close();
}

int main(int argc, char* argv[]) {
    auto cmdProcessor = CommandProcessor::getInstance();
    cmdProcessor->loadBufferFromFile();

    Validator& validator = Validator::getInstance();

    string cmdTypeStr(argv[1]);
    CommandType cmdType = cmdProcessor->parseCommandType(cmdTypeStr);

    Command cmd;
    cmd.type = cmdType;
    if (cmdType == WRITE || cmdType == READ || cmdType == ERASE) {
        if (argc < 3) {
            cerr << "Insufficient arguments for command " << cmdTypeStr << endl;
            return 1;
        }
        cmd.lba = stoi(argv[2]);
        if (!validator.checkNumberRange(cmd.lba)) {
            cout << "error:: lba 주소 잘못된 입력";
            return 0;
        }
        if (cmdType == WRITE) {
            if (argc < 4) {
                cerr << "Insufficient arguments for WRITE command" << endl;
                return 1;
            }
            cmd.value = argv[3];
            if (!validator.isValidHex(cmd.value)) {
                cout << "error:: 저장 데이터 잘못된 입력";
                return 0;
            }
        }
        else if (cmdType == ERASE) {
            if (argc < 4) {
                cerr << "Insufficient arguments for ERASE command" << endl;
                return 1;
            }
            cmd.size = stoi(argv[3]);
            if (!validator.checkSizeRange(cmd.size)) {
                cout << "error:: 잘못된 사이즈 입력";
                return 0;
            }
        }
    }

    cmdProcessor->addCommandToBuffer(cmd); //읽은 값 저장

    return 0;
}
