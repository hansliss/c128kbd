/*
Copyright (c) 2020, Hans Liss

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/

#define STABILIZE_STROBE_DELAY 10
#define SCANNER_INTERVAL 1000

#define NELEMS(x) (sizeof(x)/sizeof(*x))

int colpins[] = {0,1,2,3,4,5,6,7,8,9,10,11};
int rowpins[] = {14,15,16,17,18,19,20,21,22,23,24}; 

#define NROWS NELEMS(rowpins)
#define NCOLS NELEMS(colpins)

IntervalTimer scanTimer;

/*
const char *keyname[NROWS][NCOLS] = {
  {"INST/DEL",  "3",      "5",    "7",    "9",    "+",    "£",        "1",        "HELP", "ESC",        "ALT",        "N/A"},
  {"RETURN",    "W",      "R",    "Y",    "I",    "P",    "*",        "LEFTARR",  "KP_8", "KP_+",       "KP_0",       "N/A"},
  {"CRSR L/R",  "A",      "D",    "G",    "J",    "L",    ";",        "CTRL",     "KP_5", "KP_-",       "KP_.",       "N/A"},
  {"F7",        "4",      "6",    "8",    "0",    "-",    "CLR/HOME", "2",        "TAB",  "LINE FEED",  "UP",         "N/A"},
  {"F1",        "Z",      "C",    "B",    "M",    ".",    "R_SHFT",   "SPACE",    "KP_2", "KP_ENTER",   "DOWN",       "N/A"},
  {"F3",        "S",      "F",    "H",    "K",    ":",    "=",        "CBM",      "KP_4", "KP_6",       "LEFT",       "N/A"},
  {"F5",        "E",      "T",    "U",    "O",    "@",    "UPARR",    "Q",        "KP_7", "KP_9",       "RIGHT",      "N/A"},
  {"CRSR U/D",  "L_SHFT", "X",    "V",    "N",    ",",    "/",        "RUN/STOP", "KP_1", "KP_3",       "NO SCROLL",  "N/A"},
  {"N/A",       "N/A",    "N/A",  "N/A",  "N/A",  "N/A",  "N/A",      "N/A",      "N/A",  "N/A",        "N/A",        "Restore"},
  {"N/A",       "N/A",    "N/A",  "N/A",  "N/A",  "N/A",  "N/A",      "N/A",      "N/A",  "N/A",        "N/A",        "40/80"},
  {"N/A",       "N/A",    "N/A",  "N/A",  "N/A",  "N/A",  "N/A",      "N/A",      "N/A",  "N/A",        "N/A",        "CAPS LOCK"},
};
*/

// https://github.com/PaulStoffregen/cores/blob/master/teensy4/keylayouts.h
// Key codes to use with VICE
// Basic code set is same as The C64, with KEY_F23 for "+" and KEY_F24 for minus due to issues with The C64 Mini
// Using KEY_TILDE for KEY_GRAVE since it appears to be the same keycode
// Using KEY_QUOTE for KEY_APOSTROPHE since it appears to be the same keycode
// Using KEY_F15 for ESC
// Using KEY_F16 for TAB
// Using KEY_F17 for CAPS LOCK
// Using KEY_F18 for HELP
// Using KEY_F19 for LINE FEED
// Using KEY_F20 for 40/80

// Using KEY_F21 for DOWN
// Using KEY_F22 for RIGHT

const int keycode1[NROWS][NCOLS] = {
  {KEY_BACKSPACE, KEY_3, KEY_5, KEY_7, KEY_9, KEY_F23, KEY_MINUS, KEY_1, KEY_F18, KEY_F15, MODIFIERKEY_ALT, -1},
  {KEY_ENTER, KEY_W, KEY_R, KEY_Y, KEY_I, KEY_P, KEY_SEMICOLON, KEY_TILDE, KEYPAD_8, KEYPAD_ASTERIX, KEYPAD_0, -1},
  {KEY_RIGHT, KEY_A, KEY_D, KEY_G, KEY_J, KEY_L, KEY_RIGHT_BRACE, MODIFIERKEY_CTRL, KEYPAD_5, KEYPAD_SLASH, KEYPAD_PERIOD, -1},
  {KEY_F7, KEY_4, KEY_6, KEY_8, KEY_0, KEY_F24, KEY_HOME, KEY_2, KEY_F16, KEY_F19, KEY_UP, -1},
  {KEY_F1, KEY_Z, KEY_C, KEY_B, KEY_M, KEY_PERIOD, MODIFIERKEY_RIGHT_SHIFT, KEY_SPACE, KEYPAD_2, KEYPAD_ENTER, KEY_F21, -1},
  {KEY_F3, KEY_S, KEY_F, KEY_H, KEY_K, KEY_LEFT_BRACE, KEY_EQUAL, MODIFIERKEY_GUI, KEYPAD_4, KEYPAD_6, KEY_LEFT, -1},
  {KEY_F5, KEY_E, KEY_T, KEY_U, KEY_O, KEY_BACKSLASH, KEY_QUOTE, KEY_Q, KEYPAD_7, KEYPAD_9, KEY_F22, -1},
  {KEY_DOWN, MODIFIERKEY_SHIFT, KEY_X, KEY_V, KEY_N, KEY_COMMA, KEY_SLASH, KEY_ESC, KEYPAD_1, KEYPAD_3, KEY_SCROLL_LOCK, -1},
  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, KEY_TAB},
  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, KEY_F20},
  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, KEY_F17}
};

// Key codes for use with Windows, using the included keyboard layout spec
// This maps a lot of things to semi-sensible codes but a few keys have been desperately mapped to *something* that MS Keyboard
// Layout Creator 1.4 sees. Some keys that should produce something useful when shifted do not do so, since they are
// invisible to MSKLC.
const int keycode2[NROWS][NCOLS] = {
  {KEY_BACKSPACE, KEY_3, KEY_5, KEY_7, KEY_9, KEY_MINUS, KEY_NON_US_NUM, KEY_1, 117 | 0xf000, KEY_ESC, MODIFIERKEY_ALT, -1},
  {KEY_ENTER, KEY_W, KEY_R, KEY_Y, KEY_I, KEY_P, KEY_RIGHT_BRACE, KEY_TILDE, KEYPAD_8, KEYPAD_PLUS, KEYPAD_0, -1},
  {KEY_RIGHT, KEY_A, KEY_D, KEY_G, KEY_J, KEY_L, KEY_QUOTE, MODIFIERKEY_CTRL, KEYPAD_5, KEYPAD_MINUS, KEYPAD_PERIOD, -1},
  {KEY_F7, KEY_4, KEY_6, KEY_8, KEY_0, KEY_EQUAL, KEY_HOME, KEY_2, KEY_TAB, KEY_PAGE_DOWN, KEY_UP, -1},
  {KEY_F1, KEY_Z, KEY_C, KEY_B, KEY_M, KEY_PERIOD, MODIFIERKEY_RIGHT_SHIFT, KEY_SPACE, KEYPAD_2, KEYPAD_ENTER, KEY_DOWN, -1},
  {KEY_F3, KEY_S, KEY_F, KEY_H, KEY_K, KEY_SEMICOLON, KEY_NON_US_BS , MODIFIERKEY_GUI, KEYPAD_4, KEYPAD_6, KEY_LEFT, -1},
  {KEY_F5, KEY_E, KEY_T, KEY_U, KEY_O, KEY_LEFT_BRACE, KEY_PAGE_UP, KEY_Q, KEYPAD_7, KEYPAD_9, KEY_RIGHT, -1},
  {KEY_DOWN, MODIFIERKEY_SHIFT, KEY_X, KEY_V, KEY_N, KEY_COMMA, KEY_SLASH, KEY_PAUSE, KEYPAD_1, KEYPAD_3, KEY_SCROLL_LOCK, -1},
  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, KEY_TAB},
  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, KEY_CAPS_LOCK}
};

// Keep track of the current keycode set. This is switchable, using Shift-40/80.
int (*keycode)[12] = keycode1;

// Keep track of key state changes, in the easiest way possible
uint8_t keydown[NROWS][NCOLS];
uint8_t oldkeydown[NROWS][NCOLS];

// These are used only for controller-local magic, like the keycode set switch function
boolean isShift = false;
boolean isCtrl = false;

// Set up I/O pins and start the scanner
void setup() {
  // Strobe pins. Keep these set to INPUT when you're not strobing - the ghosting glitches that occur if you keep
  // them high (so the current roams the matris when keys are held down) are not nice.
  for (unsigned int row = 0; row < NROWS; row++) {
    pinMode(rowpins[row], INPUT);
  }
  // We have to pull up the input pins and sink them through the switches.
  for (unsigned int col = 0; col < NCOLS; col++) {
    pinMode(colpins[col], INPUT_PULLUP);
  }
  memset(keydown, 0, sizeof(keydown));
  memset(oldkeydown, 0, sizeof(oldkeydown));

  // Start the keyboard scanner. It will keep the current state in "keydown" updated
  scanTimer.begin(scanKeyboard, SCANNER_INTERVAL);
}

// Scan the keyboard, and keep the current state of all keys updated in "keydown"
void scanKeyboard() {
  // We keep a local debouncing state array here
  static uint8_t debounce[NROWS][NCOLS];

  // Strobe each row in order
  for (unsigned int row = 0; row < NROWS; row++) {
    // We set only this row to OUTPUT, and strobe by pulling it low
    pinMode(rowpins[row], OUTPUT);
    digitalWrite(rowpins[row], LOW);

    // Wait a bit to let it settle
    delayMicroseconds(STABILIZE_STROBE_DELAY);

    // Now read all the columns in order, handle debouncing, and update the state array
    for (unsigned int col = 0; col < NCOLS; col++) {
      if (digitalRead(colpins[col]) == LOW) {
        switch (debounce[row][col]) {
          // We debounce by just waiting one extra cycle (SCANNER_INTERVAL) and making
          // sure the key is still pressed
          case 0:
            debounce[row][col]++;
            break;
          case 1:
            debounce[row][col]++;
            keydown[row][col] = 1;
            break;
          case 2:
            break;
        }
      } else {
        debounce[row][col] = 0;
        keydown[row][col] = 0;
      }
    }

    // When done with this pin, set it to INPUT so as not to disturb anything else in the matrix.
    pinMode(rowpins[row], INPUT);
  }
}

// Scan through the key state array, compare to the saved state from last run, and
// generate events. Some special cases are handled internally
void checkForChanges() {
  for (unsigned int col = 0; col < NCOLS; col++) {
    for (unsigned int row = 0; row < NROWS; row++) {
      if (keydown[row][col] && !oldkeydown[row][col]) {
        // Key Down event

        // Keep track of SHIFT and CTRL state, for our own internal use
        if (keycode[row][col] == MODIFIERKEY_SHIFT || keycode[row][col] == MODIFIERKEY_RIGHT_SHIFT) {
          isShift = true;
        }
        if (keycode[row][col] == MODIFIERKEY_CTRL || keycode[row][col] == MODIFIERKEY_RIGHT_CTRL) {
          isCtrl = true;
        }

        // If SHIFT is held down and 40/80 is toggled, switch to the second keycode set
        if (isShift && row == 9 && col == 11) {
          keycode = keycode2;
        } else {
          // Otherwise send the press event with this keycode
          Keyboard.press(keycode[row][col]);
        }
      } else if (!keydown[row][col] && oldkeydown[row][col]) {
        // Key Uup event

        // Keep track of SHIFT and CTRL state, for our own internal use
        if (keycode[row][col] == MODIFIERKEY_SHIFT || keycode[row][col] == MODIFIERKEY_RIGHT_SHIFT) {
          isShift = false;
        }
        if (keycode[row][col] == MODIFIERKEY_CTRL || keycode[row][col] == MODIFIERKEY_RIGHT_CTRL) {
          isCtrl = false;
        }

        // If 40/80 is released, switch to the first keycode set
        if (row == 9 && col == 11) {
          keycode = keycode1;
        } else {
          // Otherwise send the release event
          Keyboard.release(keycode[row][col]);
        }
      }
      // Update the saved state
      oldkeydown[row][col] = keydown[row][col];
    }
  }
}

// Check for key state changes regularly. We could do so much more here.
void loop() {
  checkForChanges();
  delay(5);
}
