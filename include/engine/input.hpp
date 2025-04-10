#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "engine/glContext.hpp"
#include "common/util/maputil.hpp"

#define BETWEEN(lb, x, hb) lb <= x && x <= hb

typedef enum inputevent_kind {
    INPUT_EVENT_KEYPRESS,
    INPUT_EVENT_MOUSE
} InputEventKind;

class Key {
    public:
        bool special;
        unsigned int key;

        Key() = default;

        Key(unsigned int key): Key(key, false) {}

        Key(unsigned int key, bool special) {
            this->special = special;
            this->key = key;
        }

        bool isAlpha() {
            return this->special == false && (BETWEEN('A', this->key, 'Z') || BETWEEN('a', this->key, 'z'));
        }

        bool isNum() {
            return this->special == false && BETWEEN('0', this->key, '9');
        }

        bool isAlphaNum() {
            return this->isAlpha() || this->isNum();
        }

        bool isFunctionKey() {
            return this->special == true && BETWEEN(GLUT_KEY_F1, this->key, GLUT_KEY_F12);
        }

        bool isArrowKey() {
            return this->special == true && BETWEEN(GLUT_KEY_LEFT, this->key, GLUT_KEY_PAGE_DOWN);
        }

        std::size_t getHash() {
            return ((this->special & 0xFF) << 8) | (this->key & 0xFF);
        }

        std::string toString() {
            std::string ret;
            if (this->special) ret += "*";
            if (this->isAlphaNum()) ret += this->key;
            else {
                if (this->isFunctionKey()) ret += "F" + std::to_string(this->key);
                else if (this->isArrowKey()) {
                    switch (this->key) {
                        case GLUT_KEY_LEFT:  ret += "A_LEFT";  break;
                        case GLUT_KEY_UP:    ret += "A_UP";    break;
                        case GLUT_KEY_RIGHT: ret += "A_RIGHT"; break;
                        case GLUT_KEY_DOWN:  ret += "A_DOWN";  break;
                    }
                } else {
                    switch (this->key) {
                        case GLUT_KEY_PAGE_UP:    ret += "P_UP";    break;
                        case GLUT_KEY_PAGE_DOWN:  ret += "P_DOWN";  break;
                        case GLUT_KEY_HOME:       ret += "HOME";  break;
                        case GLUT_KEY_END:        ret += "END";  break;
                        case GLUT_KEY_INSERT:     ret += "INSERT";  break;
                        default:                  ret += std::to_string(this->key);
                    }
                }
            }

            return ret;
        }
};

class InputEvent {
    public:
        virtual bool isMouseEvent() {
            return this->kind == INPUT_EVENT_MOUSE;
        }

        virtual bool isKeypressEvent() {
            return this->kind == INPUT_EVENT_KEYPRESS;
        }

    protected:
        InputEventKind kind;
};

class MouseEvent: public InputEvent {
    public:
        int mouseX;
        int mouseY;

        MouseEvent() {
            this->mouseX = 0;
            this->mouseY = 0;
        }

        MouseEvent(int mouseX, int mouseY) {
            this->mouseX = mouseX;
            this->mouseY = mouseY;
        }
};

class KeypressEvent: public InputEvent {
    public:
        Key key;
        bool up;
        int mouseX;
        int mouseY;

        // KeypressEvent(Key key, bool up, int mouseX, int mouseY) {
        //     this->kind = INPUT_EVENT_KEYPRESS;

        //     this->key = key;
        //     this->up = up;
        //     this->mouseX = mouseX;
        //     this->mouseY = mouseY;
        // }

        KeypressEvent(Key key, bool up, int mouseX, int mouseY);

        void process();
};

typedef void(*InputCallback)(InputEvent* event);

class InputManager {
    public:
        static void listen(InputCallback callback) {
            glutKeyboardFunc(InputManager::_keydown);
            glutKeyboardUpFunc(InputManager::_keyup);
            glutSpecialFunc(InputManager::_keydown_special);
            glutSpecialUpFunc(InputManager::_keyup_special);

            InputManager::callback = callback;
        };
        static void unlisten() {
            glutKeyboardFunc(NULL);
            glutKeyboardUpFunc(NULL);
            glutSpecialFunc(NULL);
            glutSpecialUpFunc(NULL);
        }

        static bool hasInput() {
            return !InputManager::keymap.isEmpty();
        }

        static bool isKeyPressed(Key key) {
            return InputManager::keymap.hasKey(key.getHash());
        }

        static std::vector<Key> getKeys() {
            return InputManager::keymap.values();
        }

        static MouseEvent getLastMousePos() {
            return InputManager::lastMousePos;
        }

        static void handleKey(Key k) {
            InputManager::keymap.remove(k.getHash());
        }

        static void handleEvent(InputEvent* e) {
            if (e->isKeypressEvent()) {
                KeypressEvent* ke = (KeypressEvent*)(e);
                InputManager::handleKey(ke->key);
            } else {
                // TODO
            }
        }

    private:
        static Map<Key, std::size_t> keymap;
        static InputCallback callback;
        static MouseEvent lastMousePos;

        // static void _keydown(unsigned char keycode, int x, int y) {
        //     Key key = Key(keycode);
        //     InputManager::keymap.add(key.getHash(), key);
        // }
        // static void _keyup(unsigned char keycode, int x, int y) {
        //     Key key = Key(keycode);
        //     InputManager::keymap.remove(key.getHash());
        // }
        
        // static void _keydown_special(int keycode, int x, int y) {
        //     Key key = Key(keycode, true);
        //     InputManager::keymap.add(key.getHash(), key);
        // }
        // static void _keyup_special(int keycode, int x, int y) {
        //     Key key = Key(keycode, true);
        //     InputManager::keymap.remove(key.getHash());
        // }

        static void _keydown(unsigned char keycode, int x, int y);
        static void _keyup(unsigned char keycode, int x, int y);
        static void _keydown_special(int keycode, int x, int y);
        static void _keyup_special(int keycode, int x, int y);

        // TODO
        static void _mouse(unsigned char key, int x, int y);
};