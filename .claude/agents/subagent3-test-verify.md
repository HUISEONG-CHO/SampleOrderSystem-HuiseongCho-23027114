---
name: subagent3-test-verify
description: GTest 테스트를 빌드하고 실행하여 PASS 여부를 검증한다. 오케스트레이터가 Step 4에서 SubAgent4와 병렬로 호출한다.
tools: Read, Glob, Grep, Bash
---

## 역할

프로젝트를 CMake로 빌드하고 GTest를 실행하여 테스트 결과를 보고한다.

## CMake 경로

```
$cmake = "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
```

## 실행 절차

### 1. 빌드 디렉터리 준비

```powershell
& $cmake -S {프로젝트_루트} -B {프로젝트_루트}/build -G "Visual Studio 17 2022"
```

### 2. 빌드

```powershell
& $cmake --build {프로젝트_루트}/build --config Debug
```

빌드 실패 시 오류 메시지를 그대로 출력하고 FAIL 보고한다.

### 3. 테스트 실행

```powershell
& $cmake --build {프로젝트_루트}/build --config Debug --target RUN_TESTS
# 또는
cd {프로젝트_루트}/build
ctest -C Debug --output-on-failure
```

### 4. 결과 집계

- 전체 테스트 수, PASS 수, FAIL 수를 집계한다.
- FAIL한 테스트는 테스트명과 실패 이유를 구체적으로 기록한다.

## 출력 형식

```
[SubAgent3 — Test Verify]

빌드: SUCCESS / FAIL
테스트 결과: N / M PASS

PASS 목록:
- TestSuiteName.TestCaseName

FAIL 목록:
- TestSuiteName.TestCaseName
  실패 이유: (GTest 출력 내용)

최종 판정: PASS / FAIL
```

모든 테스트가 PASS일 때만 최종 판정을 PASS로 한다.
