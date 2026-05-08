# 반도체 시료 생산주문관리 시스템 — 프로젝트 구성

## 프로젝트 개요

가상의 반도체 회사 S-Semi의 콘솔 기반 시료 생산주문관리 시스템.  
총 5개 Repository를 개발·제출한다.

**언어: C++ (모든 소스 코드)**  
빌드 도구: CMake 3.15 이상, 표준: C++17

---

## 로컬 디렉터리 구조

PoC 4개는 `C:\reviewer\monitor\` 아래 각각의 하위 디렉터리로 개발한다.  
**본 프로젝트(SampleOrderSystem)는 `C:\reviewer\monitor\` 디렉터리 자체**가 해당 Repository다.

```
C:\reviewer\monitor\          ← SampleOrderSystem-HuiseongCho-23027114 (본 프로젝트)
  plan.md
  CLAUDE.md
  PRD.md
  CMakeLists.txt
  src/
  tests/
  ConsoleMVC-HuiseongCho-23027114/
  DataPersistence-HuiseongCho-23027114/
  DataMonitor-HuiseongCho-23027114/
  DummyDataGenerator-HuiseongCho-23027114/
```

---

## 제출 목록

| # | 항목 | Repository 이름 |
|---|---|---|
| 1 | MVC 스켈레톤 코드 | `ConsoleMVC-HuiseongCho-23027114` |
| 2 | 데이터 영속성 처리 | `DataPersistence-HuiseongCho-23027114` |
| 3 | 데이터 모니터링 Tool | `DataMonitor-HuiseongCho-23027114` |
| 4 | Dummy 데이터 생성 Tool | `DummyDataGenerator-HuiseongCho-23027114` |
| 5 | 본 프로젝트 | `SampleOrderSystem-HuiseongCho-23027114` |

모든 Repository는 **Public**으로 생성.

---

## PoC 1 — MVC 스켈레톤 코드 (`ConsoleMVC-HuiseongCho-23027114`)

### 목적
Model / Controller / View 패키지 구조와 역할 분리를 완성하는 뼈대 코드.  
이후 본 프로젝트의 아키텍처 기반이 된다.

### 구현 내용

```
monitor/ConsoleMVC-HuiseongCho-23027114/
  src/
    model/
      Item.h / Item.cpp        # 샘플 도메인 객체
    controller/
      ItemController.h / .cpp  # 비즈니스 로직 처리
    view/
      ItemView.h / .cpp        # 콘솔 입출력 담당
    main.cpp                   # 진입점
  CMakeLists.txt
```

- Model: 도메인 객체 정의만, 로직 없음 (데이터 클래스)
- Controller: Model을 받아 처리, View에 의존하지 않음
- View: Controller 결과를 받아 `std::cout` 출력만, 직접 로직 수행 안 함
- 간단한 CRUD 시나리오 1~2개로 흐름 검증
- 헤더/구현 파일 분리 (.h / .cpp) 원칙 준수

---

## PoC 2 — 데이터 영속성 처리 (`DataPersistence-HuiseongCho-23027114`)

### 목적
애플리케이션을 재시작해도 데이터가 유지되는 구조 검증.

### 구현 내용

- 저장 방식: **JSON 파일** — JSON 파서·직렬화기를 **프로젝트 내부에서 직접 구현** (`src/json/`)
- CRUD 4가지 모두 구현
  - Create: 객체 생성 후 `Json::saveFile`로 직렬화·저장
  - Read: `Json::parseFile`로 로드 후 `JsonValue`에서 객체 역직렬화
  - Update: 특정 항목 수정 후 전체 재직렬화·저장
  - Delete: 특정 항목 제거 후 재저장
- 재시작 시나리오 테스트: 저장 → 종료 → 재실행 → 데이터 복원 확인

```
monitor/DataPersistence-HuiseongCho-23027114/
  src/
    json/
      JsonValue.h / .cpp      # JSON 타입 (object / array / string / number / bool / null)
      JsonParser.h / .cpp     # 텍스트 → JsonValue 파서
      JsonSerializer.h / .cpp # JsonValue → 텍스트 직렬화
      json.h                  # 통합 헤더: Json::parseFile / Json::saveFile
    model/
      Record.h / .cpp
    repository/
      JsonRepository.h / .cpp # 파일 I/O + 직렬화
    main.cpp
  CMakeLists.txt
```

**JSON 모듈 API (자체 구현 목표):**
```cpp
#include "json/json.h"

// 저장
JsonValue arr(JsonValue::Type::Array);
Json::saveFile(arr, "data.json", /*pretty=*/true);

// 로드
JsonValue data = Json::parseFile("data.json");
for (size_t i = 0; i < data.size(); ++i) {
    auto id   = data[i]["id"].asInt();
    auto name = data[i]["name"].asString();
}
```

### 핵심 설계 포인트

- JSON 모듈(`src/json/`)은 외부 라이브러리 없이 C++17 표준만으로 구현 — clone 즉시 빌드 가능
- 저장/로드 로직은 별도 `repository` 레이어로 분리 (`IRepository` 인터페이스 정의)
- 파일 경로는 생성자 인자로 주입 (하드코딩 금지)
- `JsonParseError` 예외 처리 포함

---

## PoC 3 — 데이터 모니터링 Tool (`DataMonitor-HuiseongCho-23027114`)

### 목적
현재 저장된 데이터 상태를 콘솔에서 실시간으로 조회하는 관리자 도구.

### 구현 내용

- PoC 2와 동일한 JSON 파일을 읽어 표시 (저장 포맷 공유)
- 명령 입력 시 갱신
- 표시 항목:
  - 전체 시료 목록 + 현재 재고 수량
  - 주문 목록 + 각 주문 상태
  - 생산 중인 항목 + 대기 큐
- Read-only: 이 도구에서는 데이터 수정 불가

```
monitor/DataMonitor-HuiseongCho-23027114/
  src/
    json/
      JsonValue.h / .cpp      # PoC 2와 동일한 자체 구현 JSON 모듈 (복사 포함)
      JsonParser.h / .cpp
      JsonSerializer.h / .cpp
      json.h
    reader/
      JsonReader.h / .cpp     # Json::parseFile 래핑, 쓰기 없음 (Read-only)
    display/
      ConsoleDisplay.h / .cpp
    main.cpp
  CMakeLists.txt
```

---

## PoC 4 — Dummy 데이터 생성 Tool (`DummyDataGenerator-HuiseongCho-23027114`)

### 목적
테스트용 Dummy Data를 자동으로 생성하여 JSON 파일에 삽입하는 도구.

### 구현 내용

- 시료(Sample) 더미: 이름·평균생산시간·수율 무작위 생성 (`std::mt19937` 사용)
- 주문(Order) 더미: 무작위 시료·수량·고객명 조합, 다양한 상태값 포함
- 생성 수량은 CLI 인자로 조절 가능 (예: `./DummyDataGenerator 10 30`)
- 생성된 데이터는 PoC 2와 동일한 JSON 파일에 삽입

```
monitor/DummyDataGenerator-HuiseongCho-23027114/
  src/
    json/
      JsonValue.h / .cpp      # 자체 구현 JSON 모듈 (복사 포함)
      JsonParser.h / .cpp
      JsonSerializer.h / .cpp
      json.h
    generator/
      SampleGenerator.h / .cpp
      OrderGenerator.h / .cpp
    main.cpp
  CMakeLists.txt
```

---

## 본 프로젝트 — `SampleOrderSystem-HuiseongCho-23027114`

### 아키텍처

```
C:\reviewer\monitor\          ← 본 프로젝트 루트 (= SampleOrderSystem-HuiseongCho-23027114)
  src/
    json/
      JsonValue.h / .cpp        # 자체 구현 JSON 모듈 (외부 의존 없음)
      JsonParser.h / .cpp
      JsonSerializer.h / .cpp
      json.h
    model/
      Sample.h / .cpp           # Sample 도메인 (ID, 이름, 평균생산시간, 수율, 재고)
      Order.h / .cpp            # Order 도메인 + OrderStatus enum
      ProductionJob.h / .cpp    # 생산 작업 (FIFO 큐 원소)
    repository/
      IRepository.h             # CRUD 인터페이스 (템플릿)
      SampleRepository.h / .cpp # Sample CRUD + JSON 영속성
      OrderRepository.h / .cpp  # Order CRUD + JSON 영속성
    controller/
      SampleController.h / .cpp     # 시료관리 로직
      OrderController.h / .cpp      # 주문 접수·승인·거절 로직
      ProductionController.h / .cpp # 생산라인 로직
      MonitorController.h / .cpp    # 모니터링 집계 로직
      ReleaseController.h / .cpp    # 출고처리 로직
    view/
      MainView.h / .cpp
      SampleView.h / .cpp
      OrderView.h / .cpp
      MonitorView.h / .cpp
      ProductionView.h / .cpp
      ReleaseView.h / .cpp
    main.cpp
  CMakeLists.txt
  tests/
    test_order.cpp              # 주문 상태 전환 테스트
    test_production.cpp         # 실생산량 계산 + FIFO 테스트
    test_sample.cpp             # 시료 등록·조회 테스트
  CLAUDE.md
  PRD.md
```

### 기능 요구사항

사용자 관점 기능 명세는 [FEATURE/](FEATURE/) 디렉터리에서 관리한다.

| 파일 | 기능 |
|---|---|
| [F01_sample_management.md](FEATURE/F01_sample_management.md) | 시료관리 |
| [F02_order_placement.md](FEATURE/F02_order_placement.md) | 시료주문 접수 |
| [F03_order_approval.md](FEATURE/F03_order_approval.md) | 주문 승인·거절 |
| [F04_monitoring.md](FEATURE/F04_monitoring.md) | 현황 모니터링 |
| [F05_production_line.md](FEATURE/F05_production_line.md) | 생산라인 관리 |
| [F06_release.md](FEATURE/F06_release.md) | 출고처리 |

### 핵심 알고리즘

#### 주문 승인 시 재고 판단

```
if stock >= quantity:
    status → CONFIRMED
else:
    shortage      = quantity - stock
    productionQty = ceil(shortage / (yieldRate × 0.9))
    totalTime     = avgProductionTime × productionQty
    enqueue ProductionJob
    status → PRODUCING
```

#### 생산량 계산식

- 실생산량: `ceil(부족분 / (수율 × 0.9))`
- 총생산시간: `평균생산시간(분) × 실생산량`

---

## 데이터 모델

### Sample
| 필드 | C++ 타입 | 설명 |
|---|---|---|
| id | `std::string` | 고유 식별자 |
| name | `std::string` | 시료 이름 |
| avgProductionTime | `int` | 평균 생산 시간 (분) |
| yieldRate | `double` | 수율 (0.0 ~ 1.0) |
| stock | `int` | 현재 재고 수량 |

### Order
| 필드 | C++ 타입 | 설명 |
|---|---|---|
| id | `std::string` | 고유 식별자 |
| sampleId | `std::string` | 주문한 시료 ID |
| customer | `std::string` | 고객명 |
| quantity | `int` | 주문 수량 |
| status | `enum class OrderStatus` | RESERVED / REJECTED / PRODUCING / CONFIRMED / RELEASE |
| createdAt | `std::string` | 주문 접수 시각 (ISO 8601) |

### ProductionJob
| 필드 | C++ 타입 | 설명 |
|---|---|---|
| orderId | `std::string` | 연결된 주문 ID |
| sampleId | `std::string` | 생산할 시료 ID |
| productionQty | `int` | 실생산량 |
| totalTime | `int` | 총생산시간 (분) |
| enqueuedAt | `std::string` | 큐 등록 시각 (ISO 8601) |

### OrderStatus enum

```cpp
enum class OrderStatus {
    RESERVED,
    REJECTED,
    PRODUCING,
    CONFIRMED,
    RELEASE
};
```

---

## 주문 상태 흐름

```
RESERVED → (승인, 재고 충분) → CONFIRMED
RESERVED → (승인, 재고 부족) → PRODUCING → CONFIRMED
RESERVED → (거절)           → REJECTED
CONFIRMED → (출고)          → RELEASE
```

REJECTED는 정상 흐름 외 상태 — 모니터링 제외.

---

## C++ 공통 규칙

- 표준: **C++17**
- 빌드: **CMake 3.15** 이상
- JSON: **프로젝트 내 자체 구현** (`src/json/`) — 외부 라이브러리 의존 없음, clone 후 즉시 빌드 가능
- 네이밍: 클래스 `PascalCase`, 멤버변수/함수 `camelCase`, 상수 `UPPER_SNAKE_CASE`
- 헤더 가드: `#pragma once`
- 메모리: 원시 포인터 대신 `std::unique_ptr` / `std::shared_ptr` 사용
- 입출력: `std::cin` / `std::cout` (콘솔 전용, GUI 없음)

---

## 개발 방법론

### SubAgent 역할

| Agent | 역할 |
|---|---|
| **SubAgent1** — 문서 정합성 검증 | Phase Design 문서가 CLAUDE.md 개발 계획과 정합하는지 검증 |
| **SubAgent2** — AI Action | 실제 코드 구현 수행 |
| **SubAgent3** — Test Verify | GTest 기반 테스트 실행 및 PASS 여부 확인 |
| **SubAgent4** — Compliance Verify | 코드가 C++ 공통 규칙·네이밍 컨벤션·아키텍처 원칙을 준수하는지 검증 |

> SubAgent3과 SubAgent4는 **병렬**로 실행한다.

---

### Phase 개발 절차

각 PoC는 세부 PHASE로 분할하며, 각 Phase는 아래 순서를 따른다.

```
Step 1. Phase Design 문서 작성
          ↓
Step 2. SubAgent1 — 문서 정합성 검증
          → 정합 확인 시: Design 문서 commit
          ↓
Step 3. SubAgent2 — 코드 구현 (RED→GREEN TDD 원칙 적용)
          ↓
Step 4. SubAgent3 & SubAgent4 — 테스트·컴플라이언스 병렬 검증
          ↓
Step 5. 빌드 후 사용자 리뷰
```

---

### TDD 원칙 (RED → GREEN)

테스트 도구: **Google Test (GTest)**

```
[RED]  테스트 작성 → 테스트 실행(FAIL 확인) → 커밋
         ↓
[GREEN] 구현 작성 → 테스트 실행(PASS 확인) → 커밋
```

> RED에서 테스트가 FAIL하지 않으면 잘못 작성된 테스트다. 반드시 실패를 확인한 뒤 커밋한다.

#### RED 단계 — 테스트 선행 작성 & 실패 확인

순서:
1. 요구사항 기반 테스트 코드 작성 (구현 없이)
2. 테스트 실행 → **FAIL 확인 필수** (통과되면 테스트 재작성)
3. 커밋

검토 항목:
- [ ] 테스트는 구현에 대한 계획과 목표점이 된다.
- [ ] 테스트를 제일 먼저 작성했는가
- [ ] 실행 결과가 FAIL인가 (PASS라면 테스트가 잘못된 것)
- [ ] Plan.md 검토 완료

커밋: `test: [Phase명] RED — 테스트 코드 작성`

#### GREEN 단계 — 구현 & PASS 확인

순서:
1. RED 테스트를 통과시키는 최소 구현 작성
2. 테스트 실행 → **PASS 확인 필수**
3. 커밋

검토 항목:
- [ ] RED 테스트가 요구를 벗어난 불필요한 코드를 만들지 않았는가
- [ ] 테스트 실행 결과가 PASS인가

커밋:
- `feat: [Phase명] GREEN — 구현 코드 작성`
- `test: [Phase명] GREEN — 테스트 PASS 확인`

---

## 개발 순서

각 PoC 완료 시 아래 **git 절차**를 동일하게 적용한다.

### git 절차 (PoC 완료마다 반복)
```
1. GitHub에서 Repository 생성 (Public, README 없이)
2. 해당 디렉터리에서 git 초기화
   git init
   git add .
   git commit -m "initial commit"
3. 원격 연결 및 push
   git remote add origin https://github.com/HuiseongCho/{repo-name}.git
   git branch -M main
   git push -u origin main
```

### 전체 진행 순서

각 Step 시작 전 해당 PoC의 **개발 PLAN**(세부 Phase 목록)을 먼저 작성한 뒤,
Phase별로 [Phase 개발 절차](#phase-개발-절차)를 반복한다.

**Step 1 — PoC 1: MVC 스켈레톤** (`ConsoleMVC-HuiseongCho-23027114`)
- 목표: MVC 구조 확정
- 완료 후: GitHub repo 생성 → git init → push

**Step 2 — PoC 2: 데이터 영속성** (`DataPersistence-HuiseongCho-23027114`)
- 목표: json-lib 기반 저장 방식 확정
- 완료 후: GitHub repo 생성 → git init → push

**Step 3 — PoC 3: 모니터링 Tool** (`DataMonitor-HuiseongCho-23027114`)
- 목표: 저장 데이터 실시간 조회 검증
- 완료 후: GitHub repo 생성 → git init → push

**Step 4 — PoC 4: Dummy 생성 Tool** (`DummyDataGenerator-HuiseongCho-23027114`)
- 목표: 테스트 데이터 준비
- 완료 후: GitHub repo 생성 → git init → push

**Step 5 — 본 프로젝트** (`SampleOrderSystem-HuiseongCho-23027114`)
- 목표: 위 PoC 통합 + 전체 기능 구현
- 작업 디렉터리: `C:\reviewer\monitor\` 자체 (별도 하위 디렉터리 없음)
- 완료 후: GitHub repo 생성 → `C:\reviewer\monitor\`에서 git init → push

---

## Agentic Engineering 주안점 (미션 2)

- `CLAUDE.md`: C++ 프로젝트 구조, 네이밍 컨벤션(PascalCase 클래스, camelCase 멤버), 도메인 규칙, 개발 방법론 문서화
- `PRD.md`: 사용자 관점 기능 요구사항 개요
- `FEATURE/`: 기능별 상세 요구사항 (비기술, 사용자 기준)
- **Harness**: CMake + GTest(`tests/` 디렉터리)로 자동화 테스트 구성
- **Test script**: 주문 상태 전환, `ceil(부족분 / (수율 × 0.9))` 계산, 생산라인 FIFO 순서 등 핵심 로직 커버
- **TDD**: 각 Phase마다 RED(테스트 선행) → GREEN(구현·PASS) 사이클 준수
- **SubAgent**: 문서 정합성(1) → 구현(2) → 테스트·컴플라이언스 병렬(3&4) 순서로 실행
- **CleanCode**: 단일 책임 원칙, RAII, `const` 정확성, 스마트 포인터 사용
- **Commit 이력**: Phase 단위 커밋, `test:/feat:/docs:` 접두사 사용
