#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

using namespace std;

class Validator {
private:
    // Private 생성자
    Validator() {}

    // 삭제된 복사 생성자와 할당 연산자를 통해 복사를 방지
    Validator(const Validator&) = delete;
    Validator& operator=(const Validator&) = delete;

public:
    // 클래스의 유일한 인스턴스에 접근하는 함수
    static Validator& getInstance() {
        static Validator instance;
        return instance;
    }

    // 0이상 99이하 체크
    bool checkNumberRange(const int& lbaAddress) {
        return lbaAddress >= 0 && lbaAddress <= 99;
    }

    // 1이상 10이하 체크
    bool checkSizeRange(const int& size) {
        return size >= 1 && size <= 10;
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
};
