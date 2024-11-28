#pragma once

namespace prettyurl {

class application final {
public:
  application(const application&) = delete;
  application(application&&) = delete;

  application& operator=(const application&) = delete;
  application& operator=(application&&) = delete;  

  static void run();

private:
  application() = default;
};

} // namespace prettyurl