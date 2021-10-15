#include "input_manager.h"

namespace motion_animation {

InputManager& InputManager::GetSharedInstance() {
  static InputManager input_manager;
  return input_manager;
}

void InputManager::SetKeyPressStateOfKey(KeyboardKey key, bool pressed) {
  keyboard_key_press_states_[key] = pressed;
}

bool InputManager::GetKeyPressStateOfKey(KeyboardKey key) const {
  return keyboard_key_press_states_.at(key);
}

}  // namespace motion_animation