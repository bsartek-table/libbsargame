# Functions

| Prototype                                 | Help                                                                                                                 |
| ----------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| `bool lbg_init(void)`                     | Initialize library and dependencies. Returns false if fails.                                                         |
| `void lbg_exit(void)`                     | Gracefully stops library and dependencies                                                                            |
| `bool lbg_get_screen(int32_t *screen)`    | Fill the first parameter with the screen represented as an int array                                                 |
| `bool lbg_update_screen(int32_t *screen)` | Merge the current screen and the next screen                                                                         |
| `bool lbg_poll_event(lbg_event_t *event)` | Pop the event on top of the event queue and stores it in the first parameter. Returns `false` if the queue is empty. |
| `void lbg_perror(void)`                   | man perror                                                                                                           |

# Macros

| Name            | Help                 |
| --------------- | -------------------- |
| `LBG_WIDTH`     | Width of the screen  |
| `LBG_HEIGHT`    | Height of the screen |
| `LBG_FRAMERATE` | Framerate            |

# Enums

## Colors

| Name            | Help      | Hex        |
| --------------- | --------- | ---------- |
| `LBG_BLACK`     | Black     | 0x0        |
| `LBG_WHITE`     | White     | 0x00202020 |
| `LBG_RED`       | Red       | 0x00200000 |
| `LBG_YELLOW`    | Yellow    | 0x00202000 |
| `LBG_GREEN`     | Green     | 0x00002000 |
| `LBG_LIGHTBLUE` | Lightblue | 0x00002020 |
| `LBG_BLUE`      | Blue      | 0x00000020 |
| `LBG_PURPLE`    | Purple    | 0x00100010 |
| `LBG_ORANGE`    | Orange    | 0x00201000 |
| `LBG_PINK`      | Pink      | 0x00200010 |

## Events

| Name           | Help                           |
| -------------- | ------------------------------ |
| `LBG_P1_UP`    | First player's left joystick   |
| `LBG_P1_RIGHT` | First player's right joystick  |
| `LBG_P1_DOWN`  | First player's down joystick   |
| `LBG_P1_LEFT`  | First player's left joystick   |
| `LBG_P2_UP`    | Second player's left joystick  |
| `LBG_P2_RIGHT` | Second player's right joystick |
| `LBG_P2_DOWN`  | Second player's down joystick  |
| `LBG_P2_LEFT`  | Second player's left joystick  |
| `LBG_P1_A`     | First player's A button        |
| `LBG_P1_B`     | First player's B button        |
| `LBG_P2_A`     | Second player's A button       |
| `LBG_P2_B`     | Second player's B button       |

## Errors

| Name                     | Help                                |
| ------------------------ | ----------------------------------- |
| `LBG_ERRNO_GPIO_INIT`    | Failed to initialize GPIO pins      |
| `LBG_ERRNO_GPIO_SETMODE` | Failed to set INPUT mode on pin %d  |
| `LBG_ERRNO_LED_INIT`     | Failed to initialize LEDs           |
| `LBG_ERRNO_SIGACTION`    | Failed to setup signal handlers: %s |
| `LBG_ERRNO_SUCCESS`      | Success                             |

# Types

| Type          | Help             |
| ------------- | ---------------- |
| `lbg_color_t` | See enums#colors |
| `lbg_event_t` | See enums#events |
