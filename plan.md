# 본 프로젝트 개발 PLAN — SampleOrderSystem

## 목표

S-Semi 반도체 시료 생산주문관리 시스템 전체 구현.  
PoC 1~4에서 검증한 MVC 구조, JSON 영속성, 모니터링, Dummy 생성을 통합한다.

## Repository

이름: `SampleOrderSystem-HuiseongCho-23027114`  
작업 디렉터리: `C:\reviewer\monitor\` (본 디렉터리 자체)

---

## Phase 목록

| Phase | 제목 | 핵심 목표 |
|---|---|---|
| Phase 1 | 환경 설정 | CMakeLists.txt + GTest + JSON 모듈 |
| Phase 2 | 도메인 모델 | Sample / Order / ProductionJob |
| Phase 3 | Repository 레이어 | SampleRepository / OrderRepository |
| Phase 4 | SampleController + OrderController | 시료관리 + 주문 승인·거절 핵심 로직 |
| Phase 5 | ProductionController + MonitorController + ReleaseController | 생산·모니터링·출고 |
| Phase 6 | View 레이어 | 모든 View 클래스 |
| Phase 7 | main.cpp 통합 | 전체 메뉴 시스템 + 시나리오 검증 |

---

## Phase 1 — 환경 설정

### 산출물
```
CMakeLists.txt
src/json/       # PoC 2 JSON 모듈 복사
src/main.cpp    # 빈 진입점
tests/
  CMakeLists.txt
  test_placeholder.cpp
```

### RED / GREEN
- RED: CMakeLists.txt 없이 빌드 → FAIL
- GREEN: GTest 1/1 PASS

---

## Phase 2 — 도메인 모델

### 산출물
```
src/model/
  Sample.h / .cpp
  Order.h / .cpp
  ProductionJob.h / .cpp
tests/
  test_sample.cpp
  test_order.cpp
  test_production_job.cpp
```

### Sample 필드
| 필드 | 타입 | 설명 |
|---|---|---|
| id | `std::string` | 고유 식별자 |
| name | `std::string` | 시료 이름 |
| avgProductionTime | `int` | 평균 생산 시간 (분) |
| yieldRate | `double` | 수율 (0.0~1.0) |
| stock | `int` | 현재 재고 |

### Order 필드
| 필드 | 타입 | 설명 |
|---|---|---|
| id | `std::string` | 고유 식별자 |
| sampleId | `std::string` | 시료 ID |
| customer | `std::string` | 고객명 |
| quantity | `int` | 주문 수량 |
| status | `OrderStatus` | RESERVED/REJECTED/PRODUCING/CONFIRMED/RELEASE |
| createdAt | `std::string` | ISO 8601 접수 시각 |

### ProductionJob 필드
| 필드 | 타입 | 설명 |
|---|---|---|
| orderId | `std::string` | 연결된 주문 ID |
| sampleId | `std::string` | 생산할 시료 ID |
| productionQty | `int` | 실생산량 |
| totalTime | `int` | 총생산시간 (분) |
| enqueuedAt | `std::string` | 큐 등록 시각 |

### RED
- 각 모델 생성자, getter 테스트
- OrderStatus enum 값 검증

### GREEN
- Sample.h/.cpp / Order.h/.cpp / ProductionJob.h/.cpp 구현
- 모든 테스트 PASS

---

## Phase 3 — Repository 레이어

### 산출물
```
src/repository/
  IRepository.h
  SampleRepository.h / .cpp
  OrderRepository.h / .cpp
tests/
  test_sample_repository.cpp
  test_order_repository.cpp
```

### RED
- SampleRepository: save / findById / findAll / update / remove
- OrderRepository: 동일 CRUD + findByStatus
- JSON 파일 저장·복원 검증

### GREEN
- 구현 + 모든 테스트 PASS

---

## Phase 4 — SampleController + OrderController

### 산출물
```
src/controller/
  SampleController.h / .cpp
  OrderController.h / .cpp
tests/
  test_sample_controller.cpp
  test_order_controller.cpp
```

### 핵심 알고리즘 — 주문 승인 시 재고 판단
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

### RED
- 시료 등록·조회·검색 (이름 부분일치)
- 주문 접수 → RESERVED
- 주문 승인: 재고 충분 → CONFIRMED
- 주문 승인: 재고 부족 → PRODUCING + ProductionJob 큐 등록
- 주문 거절 → REJECTED
- `ceil(shortage / (yieldRate × 0.9))` 계산 정확성

### GREEN
- 구현 + 모든 테스트 PASS

---

## Phase 5 — ProductionController + MonitorController + ReleaseController

### 산출물
```
src/controller/
  ProductionController.h / .cpp
  MonitorController.h / .cpp
  ReleaseController.h / .cpp
tests/
  test_production_controller.cpp
  test_monitor_controller.cpp
  test_release_controller.cpp
```

### RED
**ProductionController:**
- FIFO 큐 순서 보장
- 생산 완료 시: 재고 증가 + 주문 CONFIRMED

**MonitorController:**
- 상태별 주문 카운트 (REJECTED 제외)
- 재고 상태 레이블: 여유 / 부족 / 고갈

**ReleaseController:**
- CONFIRMED 목록 조회
- 출고 실행: 재고 차감 + RELEASE

### GREEN
- 구현 + 모든 테스트 PASS

---

## Phase 6 — View 레이어

### 산출물
```
src/view/
  MainView.h / .cpp
  SampleView.h / .cpp
  OrderView.h / .cpp
  MonitorView.h / .cpp
  ProductionView.h / .cpp
  ReleaseView.h / .cpp
tests/
  test_views.cpp       # stdout 캡처 방식
```

### RED
- 각 View의 출력 메서드가 올바른 내용을 stdout에 포함하는지

### GREEN
- 구현 + 모든 테스트 PASS

---

## Phase 7 — main.cpp 통합

### 산출물
```
src/main.cpp  (완성)
```

### 메인 메뉴
```
[1] 시료관리
[2] 주문 접수
[3] 주문 승인·거절
[4] 현황 모니터링
[5] 생산라인
[6] 출고처리
[0] 종료
```

### 완료 기준
- 전체 메뉴 동작 확인
- 시나리오: 시료 등록 → 주문 → 승인(재고 부족) → 생산 완료 → 출고
- JSON 파일로 데이터 영속성 확인 (재시작 후 복원)

---

## 전체 완료 기준

- [ ] Phase 1~6 모든 GTest PASS
- [ ] test_order.cpp: 주문 상태 전환 전체 흐름
- [ ] test_production.cpp: `ceil(부족분 / (수율 × 0.9))` + FIFO
- [ ] test_sample.cpp: 시료 등록·조회
- [ ] Phase 7 통합 시나리오 콘솔 확인
- [ ] SubAgent4 Compliance PASS
- [ ] GitHub repo 생성 → push
