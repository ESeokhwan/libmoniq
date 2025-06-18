# libmoniq

## Introduction

**libmoniq** is a monitoring queue and logging library written in C++. It implements a message queue-based monitoring system and supports various message formats and logging strategies.

## Main Features

libmoniq provides the following main features:

1. **Monitor Queue**: Implements a message queue-based monitoring system.
2. **Message Adaptor**: Supports various message formats through an adaptor system.
3. **Write Strategy**: Supports multiple logging strategies, including console output.

## Build and Installation Instructions

### Requirements

- **CMake** 3.15 or higher
- **C++ Compiler** (GCC, Clang, etc.)


### Build Instructions

1. Create a build directory in the project root:
```bash
mkdir -p build && cd build
```

2. Configure the project using CMake:
```bash
cmake ..
```

3. Build the project:
```bash
make
```


### Installation Instructions

From the build directory, install the library with:

```bash
make install
```

By default, the library is installed to the system’s default installation path (usually `/usr/local`). To install to a custom path, specify `CMAKE_INSTALL_PREFIX`:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/your/install/path ..
make install
```


## Usage Example

### Include Header Files

```cpp
#include <libmoniq/monitor_log.h>
#include <libmoniq/monitor_queue.h>
#include <libmoniq/util/message_adaptor.h>
#include <libmoniq/writer/monitor_log_writer.h>
#include <libmoniq/util/naive_message_generator.h>
#include <libmoniq/writer/write_strategy/console_monitor_log_write_strategy.h>
```


### Basic Usage Example

```cpp
#include <chrono>
#include <memory>
#include <thread>

#include <libmoniq/monitor_log.h>
#include <libmoniq/monitor_queue.h>
#include <libmoniq/util/message_adaptor.h>
#include <libmoniq/writer/monitor_log_writer.h>
#include <libmoniq/util/naive_message_generator.h>
#include <libmoniq/writer/write_strategy/console_monitor_log_write_strategy.h>

int64_t get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return ms.count();
}

int64_t get_current_timestamp_nano() {
    auto now = std::chrono::steady_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    return ns.count();
}

int main() {
    // Set up monitoring components
    auto queue = monitor::util::MonitorQueue();
    auto adaptor = monitor::util::NaiveMessageGenerator(1000, 1000);
    auto write_strategy = monitor::writer::ConsoleMonitorLogWriteStrategy(adaptor, true, false, true)
    auto writer = monitor::writer::MonitorLogWriter(queue, write_strategy, 1000);

    // Start the writer thread
    thread writer_thread(&monitor::writer::MonitorLogWriter::run, &writer);
    
    ...

    // Add a log to message queue
    int64_t now = get_current_timestamp();
    int64_t now_nano = get_current_timestamp_nano();
    string msg = adaptor.generate("core_msg");
    queue.enqueue(make_unique<monitor::MonitorLog>(
        monitor::RequestType::PRODUCE, msg, monitor::State::REQUESTED,
        now, now_nano
    ));
    writer->notify_if_needed();
    
    ...
    
    // Clean up before shutdown
    writer.graceful_shutdown();
    writer_thread.join();
    
    return 0;
}
```


<!-- ## Linking the Library

To use libmoniq in a CMake project, add the following to your `CMakeLists.txt`:

```cmake
find_package(libmoniq REQUIRED)
target_link_libraries(your_target PRIVATE libmoniq)
```


## License

Add your license information here.

## Contributing

Issues and pull requests are always welcome!
 -->
