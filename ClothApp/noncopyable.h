#pragma once

class NonCopyable {
private:
  NonCopyable(const NonCopyable &other) = delete;
  NonCopyable &operator=(const NonCopyable &other) = delete;

public:
  NonCopyable() {}
};
