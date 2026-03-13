# ATSPI DumpTree 고속화 가이드 (memfd_create 적용)

기존 `atspi_accessible_dump_tree` 방식은 대용량 JSON 문자열을 D-Bus 페이로드(signature: `s`)로 직접 전달하여 직렬화 병목과 메시지 크기 제한 문제를 일으켰습니다.

이를 해결하기 위해 `at-spi2-core`의 D-Bus 인터페이스를 수정하고, 송신 측(`dali-adaptor`)에서 `memfd_create`로 파일 디스크립터(FD)를 생성하여 반환하며, 수신 측(`aurum`)에서 FD를 받아 `mmap`으로 읽어들이도록 각 컴포넌트를 수정하는 가이드입니다.

---

## 1. at-spi2-core 수정 (D-Bus 인터페이스 변경)

`at-spi2-core`의 D-Bus XML 인터페이스에서 JSON 문자열(`s`)을 반환하던 부분을 파일 디스크립터(`h`)를 반환하도록 수정해야 합니다.

**수정 파일:** `at-spi2-core/xml/Accessible.xml` (또는 해당 메서드가 정의된 XML 파일)

```xml
<!-- 기존 -->
<!-- <method name="DumpTree"> -->
<!--   <arg direction="out" type="s" name="tree_json"/> -->
<!-- </method> -->

<!-- 변경: 반환 타입을 파일 디스크립터 'h'로 변경 -->
<method name="DumpTreeFd">
  <arg direction="out" type="h" name="tree_fd"/>
</method>
```

그리고 `at-spi2-core`의 클라이언트 C API (`atspi-accessible.c`)에 FD를 받아 처리하는 함수를 추가하거나 기존 `atspi_accessible_dump_tree`를 수정합니다.

```c
/* at-spi2-core/atspi/atspi-accessible.c */

int atspi_accessible_dump_tree_fd(AtspiAccessible *obj, GError **error) {
    DBusMessage *reply;
    int fd = -1;

    reply = _atspi_dbus_call_partial(obj, atspi_interface_accessible, "DumpTreeFd", error, "");
    if (!reply) return -1;

    if (!dbus_message_get_args(reply, NULL, DBUS_TYPE_UNIX_FD, &fd, DBUS_TYPE_INVALID)) {
        g_set_error(error, ATSPI_ERROR, ATSPI_ERROR_IPC, "Failed to get FD from reply");
    }

    dbus_message_unref(reply);
    return fd; // 수신자는 이 fd를 mmap 해서 읽어야 함
}
```

---

## 2. dali-adaptor 수정 (memfd_create로 송신)

Dali 측의 Accessibility Adaptor에서는 JSON 트리를 문자열로 덤프한 후, `memfd_create`로 메모리 파일을 만들어 기록하고 그 FD를 D-Bus로 반환해야 합니다.

**수정 파일:** `dali-adaptor/adaptors/ubuntu/accessibility/bridge/bridge-accessible.cpp` (경로는 Dali 버전에 따라 다를 수 있음)

```cpp
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>

// D-Bus 메서드 핸들러 (DumpTreeFd 요청이 들어왔을 때)
DBusHandlerResult OnDumpTreeFd(DBusConnection *connection, DBusMessage *message, void *userData) {
    // 1. Dali UI 트리를 JSON 스트링으로 생성 (기존 로직 활용)
    std::string json_tree = DumpDaliTreeToJson(); 

    // 2. memfd_create로 익명 파일 생성 (O_CLOEXEC 사용)
    int fd = memfd_create("dali_a11y_tree", MFD_CLOEXEC);
    if (fd < 0) {
        // 에러 처리: 빈 응답 리턴 등
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    // 3. JSON 데이터를 FD에 쓰기
    if (write(fd, json_tree.c_str(), json_tree.length()) != (ssize_t)json_tree.length()) {
        close(fd);
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    // 4. 수신 측에서 바로 읽을 수 있도록 파일 오프셋 초기화
    lseek(fd, 0, SEEK_SET);

    // 5. D-Bus Reply 메시지 생성 및 FD 첨부
    DBusMessage *reply = dbus_message_new_method_return(message);
    if (reply) {
        DBusMessageIter iter;
        dbus_message_iter_init_append(reply, &iter);
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_UNIX_FD, &fd);
        dbus_connection_send(connection, reply, NULL);
        dbus_message_unref(reply);
    }

    // 송신 측에서는 D-Bus에 넘긴 후 fd를 닫음. 
    // 커널/dbus-daemon이 수신 측으로 fd를 복제하여 전달해 줌.
    close(fd);

    return DBUS_HANDLER_RESULT_HANDLED;
}
```

---

## 3. aurum 수정 (수신 및 mmap 읽기)

Aurum에서는 `atspi_accessible_dump_tree_fd`를 호출하여 FD를 얻어오고, `mmap`을 통해 JSON 문자열을 Zero-Copy로 읽어들여 파싱합니다.

**수정 영역:** Aurum의 UI 트리 스냅샷/덤프 수신부 로직

```cpp
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <atspi/atspi.h> // 수정된 at-spi2-core 헤더 포함

void FetchDaliTree(AtspiAccessible *app_accessible) {
    GError *error = NULL;
    
    // 1. at-spi2-core의 수정된 API를 호출하여 FD 획득
    int fd = atspi_accessible_dump_tree_fd(app_accessible, &error);
    
    if (fd < 0 || error != NULL) {
        std::cerr << "트리 FD를 가져오는데 실패했습니다." << std::endl;
        if (error) g_error_free(error);
        return;
    }

    // 2. FD 크기 알아내기
    struct stat st;
    if (fstat(fd, &st) < 0) {
        std::cerr << "fstat 실패" << std::endl;
        close(fd);
        return;
    }
    size_t file_size = st.st_size;

    // 3. mmap으로 메모리에 직접 매핑 (데이터 복사 없음)
    char *json_data = (char *)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (json_data == MAP_FAILED) {
        std::cerr << "mmap 실패" << std::endl;
        close(fd);
        return;
    }

    // 4. JSON 파서에 전달하거나 활용
    // std::string_view를 사용하면 추가 복사 없이 안전하게 활용 가능
    std::string_view tree_view(json_data, file_size);
    std::cout << "[Aurum] 수신된 트리 크기: " << file_size << " bytes\n";
    // 파싱 로직: ParseJsonTree(tree_view);

    // 5. 메모리 매핑 해제 및 FD 닫기
    munmap(json_data, file_size);
    close(fd);
}
```

## 핵심 요약
* **at-spi2-core**: D-Bus 인터페이스 리턴 타입을 `s`(String)에서 `h`(UNIX FD)로 변경.
* **dali-adaptor**: JSON String 덤프 후, `memfd_create`로 생성한 익명 파일에 `write`하고 해당 FD를 D-Bus Reply로 전달.
* **aurum**: 전달받은 FD의 `fstat`으로 사이즈를 구하고 `mmap`으로 메모리에 올려 파싱 후 FD를 `close`. 
* 이 과정을 통해 엄청난 크기의 UI 트리가 D-Bus의 직렬화/역직렬화 병목과 사이즈 제한 없이 초고속으로 전달됩니다.