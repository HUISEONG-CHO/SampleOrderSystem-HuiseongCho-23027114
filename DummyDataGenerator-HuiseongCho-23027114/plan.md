# PoC 4 개발 PLAN — Dummy 데이터 생성 Tool

## 목표

테스트용 Dummy 데이터를 자동으로 생성하여 JSON 파일에 저장하는 도구.  
`std::mt19937` 기반 무작위 생성, CLI 인수로 수량 조절, 외부 라이브러리 없이 자립형 구조.

## Repository

이름: `DummyDataGenerator-HuiseongCho-23027114`  
위치: `C:\reviewer\monitor\DummyDataGenerator-HuiseongCho-23027114\`

---

## Phase 목록

| Phase | 제목 | 핵심 목표 |
|---|---|---|
| Phase 1 | 환경 설정 + JSON 모듈 통합 | CMakeLists.txt + GTest + json/ 복사 |
| Phase 2 | SampleGenerator 구현 | 시료 Dummy 데이터 무작위 생성 |
| Phase 3 | OrderGenerator 구현 | 주문 Dummy 데이터 무작위 생성 |
| Phase 4 | main.cpp 통합 | CLI 인수 수량 조절 + JSON 파일 저장 |

---

## Phase 1 — 환경 설정 + JSON 모듈 통합

### 목적
CMakeLists.txt 작성, GTest 연동, PoC 2의 JSON 모듈을 `src/json/`으로 복사하여 빌드를 검증한다.

### 산출물
```
DummyDataGenerator-HuiseongCho-23027114/
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
- GTest PASS 확인

---

## Phase 2 — SampleGenerator 구현

### 목적
`std::mt19937`로 시료(Sample) Dummy 데이터를 무작위 생성한다.

### 산출물
```
src/generator/
  SampleGenerator.h / .cpp
tests/
  test_sample_generator.cpp
```

### Sample 필드 (본 프로젝트 데이터 모델 기준)
| 필드 | 타입 | 생성 규칙 |
|---|---|---|
| id | int | 1부터 순차 증가 |
| name | std::string | "Sample_" + 순번 |
| avgProductionTime | int | 10 ~ 120 (분) 무작위 |
| yieldRate | double | 0.5 ~ 1.0 무작위 |

### SampleGenerator 인터페이스
```cpp
class SampleGenerator {
public:
    explicit SampleGenerator(unsigned int seed = 42);
    JsonValue generate(int count);   // count개의 Sample 배열 반환
};
```

### RED
테스트 목표:
- `generate(3)` 호출 시 3개의 Sample 배열 반환
- 각 Sample에 id, name, avgProductionTime, yieldRate 필드 존재
- avgProductionTime 범위: 10 ≤ v ≤ 120
- yieldRate 범위: 0.5 ≤ v ≤ 1.0
- 동일 seed → 동일 결과 (재현성 검증)

### GREEN
- SampleGenerator.h / .cpp 구현
- 모든 테스트 PASS

---

## Phase 3 — OrderGenerator 구현

### 목적
`std::mt19937`로 주문(Order) Dummy 데이터를 무작위 생성한다.

### 산출물
```
src/generator/
  OrderGenerator.h / .cpp
tests/
  test_order_generator.cpp
```

### Order 필드 (본 프로젝트 데이터 모델 기준)
| 필드 | 타입 | 생성 규칙 |
|---|---|---|
| id | int | 1부터 순차 증가 |
| sampleId | int | 주어진 sampleId 범위 내 무작위 |
| customer | std::string | "Customer_" + 무작위 번호 |
| quantity | int | 1 ~ 100 무작위 |
| status | std::string | RESERVED / PRODUCING / CONFIRMED 중 무작위 |

### OrderGenerator 인터페이스
```cpp
class OrderGenerator {
public:
    explicit OrderGenerator(unsigned int seed = 42);
    JsonValue generate(int count, int maxSampleId);
};
```

### RED
테스트 목표:
- `generate(5, 3)` 호출 시 5개의 Order 배열 반환
- 각 Order에 id, sampleId, customer, quantity, status 필드 존재
- sampleId: 1 ≤ v ≤ maxSampleId
- quantity: 1 ≤ v ≤ 100
- status: RESERVED / PRODUCING / CONFIRMED 중 하나
- 동일 seed → 동일 결과

### GREEN
- OrderGenerator.h / .cpp 구현
- 모든 테스트 PASS

---

## Phase 4 — main.cpp 통합

### 목적
CLI 인수로 생성 수량을 조절하고, 결과를 JSON 파일로 저장한다.

### 산출물
```
src/main.cpp  (완성)
```

### CLI 사용법
```
./DummyDataGenerator [sampleCount] [orderCount]
예) ./DummyDataGenerator 10 30
    → samples.json에 10개, orders.json에 30개 생성
기본값: sampleCount=5, orderCount=10
```

### 완료 기준
- CLI 인수 없이 실행: 기본값(5, 10)으로 생성
- CLI 인수 지정: 해당 수량 생성
- samples.json / orders.json 파일 생성 확인
- 별도 GTest 불필요 (파일 생성 + 콘솔 출력으로 확인)

---

## 전체 완료 기준

- [ ] Phase 1~3 모든 GTest PASS
- [ ] Phase 4 실행 결과 (samples.json, orders.json 생성 확인)
- [ ] std::mt19937 사용 (random_device로 seed 초기화 옵션)
- [ ] SubAgent4 Compliance PASS
- [ ] GitHub repo 생성 → push
