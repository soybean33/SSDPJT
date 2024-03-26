#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

using namespace std;

/*
string -> int 변환
*/
int convertString2Int(const string& lbaAddress) {
    int lbaValue;
    try {
        lbaValue = std::stoi(lbaAddress);
    }
    //숫자가 아닐 경우
    catch (invalid_argument const& e) {
        return -1;
    }
    //int범위 아닐 경우
    catch (out_of_range const& e) {
        return -1;
    }
    return lbaValue;
}

/*
0이상 99이하 체크
*/
bool checkNumberRange(const int& lbaAddress) {
    if (lbaAddress >= 0 && lbaAddress <= 99) {
        return true;
    }
    return false;
}

bool isValidHex(const string& data) {
    // 입력 문자열의 길이가 10이 아니면 유효하지 않음
    if (data.length() != 10)
        return false;

    // 첫 번째 문자가 0이고 두 번째 문자가 x인지 확인
    if (data[0] != '0' || tolower(data[1]) != 'x')
        return false;

    // 나머지 문자들이 유효한 16진수 문자인지 확인
    for (size_t i = 2; i < data.length(); i++) {
        if (!isxdigit(data[i]))
            return false;
    }

    return true;
}

/*
// 16진수 문자열을 uint32_t로 변환하는 함수
uint32_t hexStringToUint32(const string& hexStr) {
    uint32_t result;
    stringstream ss;

    // 문자열에서 "0x" 접두사 제거
    string valueWithoutPrefix = hexStr.substr(2);

    // 16진수 문자열을 uint32_t 값으로 변환
    ss << hex << valueWithoutPrefix;
    ss >> result;

    return result;
}
*/