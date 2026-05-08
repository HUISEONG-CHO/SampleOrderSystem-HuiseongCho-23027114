# PoC 2 개발 PLAN — 데이터 영속성 처리

## 목표

애플리케이션을 재시작해도 데이터가 유지되는 구조를 검증한다.  
JSON 파서·직렬화기를 C++17 표준만으로 직접 구현하며, 외부 라이브러리에 의존하지 않는다.

## Repository

이름: `DataPersistence-HuiseongCho-23027114`  
위치: `C:\reviewer\monitor\DataPersistence-HuiseongCho-23027114\`

---

## Phase 목록

| Phase | 제목 | 핵심 목표 |
|---|---|---|
| Phase 1 | 프로젝트 환경 설정 | CMakeLists.txt + GTest FetchContent 연동 |
| Phase 2 | JSON 모듈 구현 | JsonValue / JsonParser / JsonSerializer 자체 구현 |
| Phase 3 | Record 도메인 모델 | 순수 데이터 클래스 구현 |
| Phase 4 | Repository 레이어 | IRepository 인터페이스 + JsonRepository CRUD |
| Phase 5 | 재시작 시나리오 | 저장 → 종료 → 재실행 → 데이터 복원 확인 |

---

## Phase 1 — 프로젝트 환경 설정

### 목적
CMakeLists.txt 작성 및 GTest FetchContent 연동 확인.

### 산출물
```
DataPersistence-HuiseongCho-23027114/
  CMakeLists.txt
  src/main.cpp
  tests/
    CMakeLists.txt
    test_placeholder.cpp
```

### RED
- test_placeholder.cpp 작성 → CMakeLists.txt 없이 빌드 → FAIL 확인

### GREEN
- CMakeLists.txt 완성 (GTest FetchContent 포함)
- GTest PASS 확인

---

## Phase 2 — JSON 모듈 구현

### 목적
C++17 표준만으로 JSON 파서와 직렬화기를 직접 구현한다.  
외부 라이브러리 없이 clone 후 즉시 빌드 가능한 자립형 구조.

### 산출물
```
src/json/
  JsonValue.h / .cpp    # JSON 타입 (object/array/string/number/bool/null)
  JsonParser.h / .cpp   # 텍스트 → JsonValue
  JsonSerializer.h / .cpp # JsonValue → 텍스트 (pretty 지원)
  json.h                # 통합 헤더: Json::parse / Json::stringify / Json::parseFile / Json::saveFile
```

### JsonValue 지원 타입
| 타입 | C++ 표현 |
|---|---|
| null | 기본값 |
| boolean | `bool` |
| number | `double` |
| string | `std::string` |
| array | `std::vector<JsonValue>` |
| object | `std::map<std::string, JsonValue>` |

### JSON 모듈 API
```cpp
#include "json/json.h"

// 파싱
JsonValue v = Json::parse(R"({"id":1,"name":"test"})");
Json::parseFile("data.json");

// 직렬화
std::string s = Json::stringify(v, /*pretty=*/true);
Json::saveFile(v, "data.json", /*pretty=*/true);

// 값 접근
int id      = (int)v["id"].asDouble();
std::string name = v["name"].asString();
```

### RED
테스트 목표:
- `JsonValue` 기본 타입 생성 및 접근 (string / number / bool / null)
- `JsonValue` array / object 생성 및 접근
- `Json::parse` 로 JSON 문자열 파싱
- `Json::stringify` 로 직렬화 후 재파싱 시 동일한 값
- **잘못된 JSON 문자열 파싱 시 `JsonParseError` 예외 발생**

### GREEN
- JsonValue.h/.cpp / JsonParser.h/.cpp / JsonSerializer.h/.cpp / json.h 구현
- 모든 테스트 PASS

---

## Phase 3 — Record 도메인 모델

### 목적
JSON 영속성 대상이 되는 Record 도메인 객체 구현.

### 산출물
```
src/model/
  Record.h / .cpp
```

### Record 필드
| 필드 | 타입 | 설명 |
|---|---|---|
| id | `int` | 고유 식별자 |
| name | `std::string` | 레코드 이름 |
| value | `double` | 저장 값 |

### RED
테스트 목표:
- `Record(id, name, value)` 생성자 및 getter
- 기본 생성자 동작

### GREEN
- Record.h / .cpp 구현
- 모든 테스트 PASS

---

## Phase 4 — Repository 레이어

### 목적
IRepository 인터페이스와 JsonRepository를 구현하여 CRUD + JSON 영속성 검증.

### 산출물
```
src/repository/
  IRepository.h          # CRUD 인터페이스 (템플릿)
  JsonRepository.h / .cpp
```

### IRepository 인터페이스
```cpp
template <typename T>
class IRepository {
public:
    virtual void save(const T& item)           = 0;
    virtual T    findById(int id) const        = 0;
    virtual std::vector<T> findAll() const     = 0;
    virtual void update(int id, const T& item) = 0;
    virtual void remove(int id)                = 0;
    virtual ~IRepository() = default;
};
```

### JsonRepository 설계
- 생성자에서 파일 경로 주입 (하드코딩 금지)
- 모든 연산 후 JSON 파일 즉시 재직렬화·저장
- 파일 없을 시 빈 컬렉션으로 초기화

### RED
테스트 목표:
- `save` 후 `findAll`로 조회
- `findById`로 특정 레코드 반환
- `update` 후 변경 내용 반영
- `remove` 후 목록에서 제거
- 존재하지 않는 id 접근 시 예외 발생
- **파일 저장 검증**: save 후 JSON 파일이 실제로 생성되는가
- **JsonParseError 예외 처리**: 손상된 JSON 파일 로드 시 예외 발생 확인

### GREEN
- IRepository.h / JsonRepository.h/.cpp 구현
- 모든 테스트 PASS

---

## Phase 5 — 재시작 시나리오

### 목적
프로그램을 종료 후 재실행해도 JSON 파일에서 데이터가 복원됨을 확인.

### 산출물
```
src/main.cpp  (완성)
```

### 시나리오
1. Record 3개 저장 → JSON 파일 생성 확인
2. 프로그램 재시작 (JsonRepository 재생성) → 파일에서 데이터 복원 확인
3. 특정 레코드 수정 → 파일 갱신 확인
4. 특정 레코드 삭제 → 파일 갱신 확인

### 완료 기준
- 프로그램 실행 시 시나리오가 콘솔에 정상 출력
- JSON 파일이 실제로 생성·갱신됨
- 재시작 후 동일한 데이터 복원 확인

---

## 전체 완료 기준

- [ ] Phase 1~4 모든 GTest PASS
- [ ] Phase 5 재시작 시나리오 콘솔 출력 확인
- [ ] JSON 파일 실제 생성·갱신 확인
- [ ] SubAgent4 Compliance PASS
  - 외부 라이브러리 참조 없음
  - IRepository 인터페이스 분리
  - 파일 경로 생성자 주입
  - 네이밍 규칙 준수
- [ ] 커밋 이력: `test(RED)` → `feat/test(GREEN)` 패턴
- [ ] GitHub repo 생성 → push
