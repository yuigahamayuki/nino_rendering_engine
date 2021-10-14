#pragma once

#include <map>

namespace motion_animation {

class InputManager {
public:
  enum class KeyboardKey {
    w,
    s,
    a,
    d,
    up,
    down,
    left,
    right
  };

  static InputManager& GetSharedInstance();

  ~InputManager() = default;

  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;

  void SetKeyPressStateOfKey(KeyboardKey key, bool pressed);
  bool GetKeyPressStateOfKey(KeyboardKey key) const;

private:
  InputManager() = default;

  // true means the key has been pressed.
  std::map<KeyboardKey, bool> keyboard_key_press_states_;

};  // class InputManager

}  // namespace motion_animation