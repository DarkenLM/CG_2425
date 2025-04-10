#include "engine/input.hpp"

KeypressEvent::KeypressEvent(Key key, bool up, int mouseX, int mouseY) {
    this->kind = INPUT_EVENT_KEYPRESS;

    this->key = key;
    this->up = up;
    this->mouseX = mouseX;
    this->mouseY = mouseY;
}

void KeypressEvent::process() {
    InputManager::handleEvent(this);
}

Map<Key, std::size_t> InputManager::keymap;
InputCallback InputManager::callback;
MouseEvent InputManager::lastMousePos;

void InputManager::_keydown(unsigned char keycode, int x, int y) {
    printf("KEYDOWN: %c\n", keycode);
    Key key = Key(static_cast<int>(keycode));
    InputManager::keymap.add(key.getHash(), key);

    KeypressEvent e = KeypressEvent(key, false, x, y);
    callback(&e);
}

void InputManager::_keyup(unsigned char keycode, int x, int y) {
    printf("KEYUP: %c\n", keycode);
    Key key = Key(static_cast<int>(keycode));
    InputManager::keymap.remove(key.getHash());

    KeypressEvent e = KeypressEvent(key, true, x, y);
    callback(&e);
}

void InputManager::_keydown_special(int keycode, int x, int y) {
    printf("SPECIAL KEYDOWN: %d\n", keycode);
    Key key = Key(keycode, true);
    InputManager::keymap.add(key.getHash(), key);

    KeypressEvent e = KeypressEvent(key, false, x, y);
    callback(&e);
}

void InputManager::_keyup_special(int keycode, int x, int y) {
    printf("SPECIAL KEYUP: %d\n", keycode);
    Key key = Key(keycode, true);
    InputManager::keymap.remove(key.getHash());

    KeypressEvent e = KeypressEvent(key, true, x, y);
    callback(&e);
}

