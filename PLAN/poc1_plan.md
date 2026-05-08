# PoC 1 개발 PLAN — MVC 스켈레톤

## 목표

Model / Controller / View 패키지 구조와 역할 분리를 C++ 코드로 확정한다.  
이후 본 프로젝트(SampleOrderSystem)의 아키텍처 기반이 된다.

## Repository

이름: `ConsoleMVC-HuiseongCho-23027114`  
위치: `C:\reviewer\monitor\ConsoleMVC-HuiseongCho-23027114\`

---

## Phase 목록

| Phase | 제목 | 핵심 목표 |
|---|---|---|
| Phase 1 | 프로젝트 환경 설정 | CMakeLists.txt + GTest FetchContent 연동 |
| Phase 2 | Model — Item 도메인 | 순수 데이터 클래스, 로직 없음 |
| Phase 3 | Controller — ItemController | CRUD 로직, View 비의존 검증 |
| Phase 4 | View — ItemView | 출력 전용, 로직 없음 검증 |
| Phase 5 | 통합 — main.cpp | MVC 연결 + CRUD 시나리오 실행 |

---

## Phase 1 — 프로젝트 환경 설정

### 목적
CMakeLists.txt를 작성하고 GTest가 FetchContent로 빌드에 통합되는지 확인한다.

### 산출물
```
ConsoleMVC-HuiseongCho-23027114/
  CMakeLists.txt
  src/
    main.cpp          (빈 진입점)
  tests/
    CMakeLists.txt
    test_placeholder.cpp
```

### RED
- `test_placeholder.cpp`에 `ASSERT_TRUE(true)` 더미 테스트 작성
- 빌드 → GTest 실행 → PASS 확인이 목표

### GREEN
- CMakeLists.txt 완성 (GTest FetchContent 포함)
- 더미 테스트 PASS

---

## Phase 2 — Model: Item 도메인 객체

### 목적
Item 도메인 객체를 구현한다. 비즈니스 로직 없이 데이터 보관만 담당.

### 산출물
```
src/model/
  Item.h
  Item.cpp
tests/
  test_item.cpp
```

### Item 필드

| 필드 | 타입 | 설명 |
|---|---|---|
| id | `int` | 고유 식별자 |
| name | `std::string` | 아이템 이름 |
| quantity | `int` | 수량 |

### RED
테스트 목표:
- `Item(id, name, quantity)` 생성자로 객체 생성
- `getId()` / `getName()` / `getQuantity()` 반환값 정확성
- 기본 생성자 호출 가능

### GREEN
- `Item.h` / `Item.cpp` 구현
- 모든 테스트 PASS

---

## Phase 3 — Controller: ItemController CRUD

### 목적
ItemController가 Item 목록에 대한 CRUD 로직을 처리하며 View에 의존하지 않음을 확인한다.

### 산출물
```
src/controller/
  ItemController.h
  ItemController.cpp
tests/
  test_item_controller.cpp
```

### ItemController 인터페이스
```cpp
class ItemController {
public:
    void        addItem(const Item& item);
    Item        getItem(int id) const;
    std::vector<Item> getAllItems() const;
    void        updateItem(int id, const Item& updated);
    void        deleteItem(int id);
};
```

### RED
테스트 목표:
- `addItem` 후 `getAllItems`로 조회되는지
- `getItem(id)`으로 특정 아이템 반환
- `updateItem` 후 변경 내용 반영 확인
- `deleteItem` 후 목록에서 제거 확인
- 존재하지 않는 id에 `getItem` 호출 시 예외 발생

### GREEN
- `ItemController.h` / `ItemController.cpp` 구현
- 모든 테스트 PASS

---

## Phase 4 — View: ItemView 출력

### 목적
ItemView가 `std::cout` 출력만 담당하고 비즈니스 로직을 수행하지 않음을 확인한다.

### 산출물
```
src/view/
  ItemView.h
  ItemView.cpp
tests/
  test_item_view.cpp
```

### ItemView 인터페이스
```cpp
class ItemView {
public:
    void showItem(const Item& item) const;
    void showAllItems(const std::vector<Item>& items) const;
    void showMessage(const std::string& message) const;
};
```

### RED
테스트 목표 (stdout 캡처 방식):
- `showItem` 호출 시 출력에 Item의 id / name / quantity 포함
- `showAllItems` 호출 시 모든 Item 정보가 출력에 포함
- `showMessage` 호출 시 메시지 문자열이 출력에 포함

### GREEN
- `ItemView.h` / `ItemView.cpp` 구현
- 모든 테스트 PASS

---

## Phase 5 — 통합: main.cpp + CRUD 시나리오

### 목적
main.cpp에서 Model → Controller → View가 올바르게 연결되어 CRUD 시나리오가 콘솔에 출력됨을 확인한다.

### 산출물
```
src/
  main.cpp    (완성)
```

### 시나리오
1. 아이템 3개 등록 → 전체 목록 출력
2. 특정 아이템 수정 → 수정된 목록 출력
3. 특정 아이템 삭제 → 남은 목록 출력

### 완료 기준
- 프로그램 실행 시 3개 시나리오가 콘솔에 정상 출력
- 각 레이어 역할이 코드 상 명확히 분리됨
- 별도 GTest 불필요 (통합 동작은 콘솔 출력으로 확인)

---

## 전체 완료 기준

- [ ] Phase 1~4 모든 GTest PASS
- [ ] Phase 5 빌드 + 콘솔 시나리오 실행 확인
- [ ] SubAgent4 Compliance PASS
  - 레이어 분리: Controller에 cout 없음 / View에 로직 없음 / Model에 로직 없음
  - 네이밍: PascalCase 클래스, camelCase 멤버
  - `#pragma once` 모든 헤더
  - 스마트 포인터 (원시 포인터 금지)
- [ ] 커밋 이력: `docs` → `test(RED)` → `feat/test(GREEN)` 패턴 준수
- [ ] GitHub repo 생성 → push
