# PoC 3 개발 PLAN — 데이터 모니터링 Tool

## 목표

PoC 2가 생성한 JSON 파일을 읽어 콘솔에 실시간으로 표시하는 Read-only 관리자 도구.  
데이터 수정은 절대 불가하며, `src/json/` 모듈은 PoC 2와 동일한 자체 구현을 복사하여 사용한다.

## Repository

이름: `DataMonitor-HuiseongCho-23027114`  
위치: `C:\reviewer\monitor\DataMonitor-HuiseongCho-23027114\`

---

## Phase 목록

| Phase | 제목 | 핵심 목표 |
|---|---|---|
| Phase 1 | 환경 설정 + JSON 모듈 통합 | CMakeLists.txt + GTest + json/ 복사 |
| Phase 2 | JsonReader 구현 | Read-only JSON 파일 로더 (쓰기 없음) |
| Phase 3 | ConsoleDisplay 구현 | 데이터 포맷 출력 |
| Phase 4 | main.cpp 통합 | 명령 입력 시 갱신, 전체 흐름 검증 |

---

## Phase 1 — 환경 설정 + JSON 모듈 통합

### 목적
CMakeLists.txt 작성, GTest 연동, PoC 2의 JSON 모듈을 `src/json/`으로 복사하여 빌드를 검증한다.

### 산출물
```
DataMonitor-HuiseongCho-23027114/
  CMakeLists.txt
  src/
    json/                     # PoC 2 src/json/ 복사
      JsonValue.h / .cpp
      JsonParser.h / .cpp
      JsonSerializer.h / .cpp
      json.h
    main.cpp
  tests/
    CMakeLists.txt
    test_placeholder.cpp
```

### RED
- test_placeholder.cpp 작성 → CMakeLists.txt 없이 빌드 → FAIL 확인

### GREEN
- CMakeLists.txt 완성 (GTest FetchContent + json/ 소스 포함)
- JSON 모듈 복사 후 GTest PASS 확인

---

## Phase 2 — JsonReader 구현

### 목적
`Json::parseFile`만 래핑하는 Read-only 파일 로더를 구현한다.  
`Json::saveFile` 등 쓰기 기능은 절대 포함하지 않는다.

### 산출물
```
src/reader/
  JsonReader.h / .cpp
tests/
  test_json_reader.cpp
```

### JsonReader 인터페이스
```cpp
class JsonReader {
public:
    explicit JsonReader(const std::string& filePath);
    JsonValue load() const;          // parseFile 래핑, Read-only
    bool      fileExists() const;    // 파일 존재 여부 확인
};
```

### RED
테스트 목표:
- `JsonReader` 생성 + `load()`로 JSON 파일 읽기
- `fileExists()` 파일 존재 여부 반환
- 파일 없을 시 예외 발생 확인
- `JsonReader`에 쓰기 메서드가 없음을 컴파일 타임 검증 (설계 확인)

### GREEN
- JsonReader.h / .cpp 구현
- 모든 테스트 PASS

---

## Phase 3 — ConsoleDisplay 구현

### 목적
JsonValue 데이터를 받아 콘솔에 포맷팅하여 출력하는 순수 출력 클래스.  
비즈니스 로직 없이 출력만 담당한다.

### 산출물
```
src/display/
  ConsoleDisplay.h / .cpp
tests/
  test_console_display.cpp
```

### ConsoleDisplay 인터페이스
```cpp
class ConsoleDisplay {
public:
    void showRecords(const JsonValue& data) const;
    void showMessage(const std::string& message) const;
    void showError(const std::string& error) const;
};
```

### RED
테스트 목표 (stdout 캡처):
- `showRecords` 호출 시 레코드 정보(id, name, value)가 출력에 포함
- `showMessage` 호출 시 메시지가 출력에 포함
- `showError` 호출 시 에러 메시지가 출력에 포함

### GREEN
- ConsoleDisplay.h / .cpp 구현
- 모든 테스트 PASS

---

## Phase 4 — main.cpp 통합

### 목적
JsonReader + ConsoleDisplay를 연결하여 명령 입력 시 JSON 파일을 읽어 콘솔에 표시한다.

### 산출물
```
src/main.cpp  (완성)
```

### 동작 흐름
```
프로그램 시작
  └─ 안내 메시지 출력
  └─ 명령 대기 루프:
       [r] → JSON 파일 읽어 최신 데이터 표시
       [q] → 종료
```

### 완료 기준
- `r` 입력 시 PoC 2의 records.json 읽어 레코드 목록 출력
- 파일 없을 시 "파일 없음" 메시지 출력
- `q` 입력 시 정상 종료
- 별도 GTest 불필요 (콘솔 출력으로 확인)

---

## 전체 완료 기준

- [ ] Phase 1~3 모든 GTest PASS
- [ ] Phase 4 콘솔 동작 확인 (r 입력 → 데이터 표시, q 입력 → 종료)
- [ ] Read-only 준수: JsonReader에 저장 관련 코드 없음
- [ ] SubAgent4 Compliance PASS
- [ ] GitHub repo 생성 → push
