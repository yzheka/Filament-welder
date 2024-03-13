#define MOTOR1_STEP_PIN                          36
#define MOTOR1_DIR_PIN                           34
#define MOTOR1_ENABLE_PIN                        30

#define MOTOR2_STEP_PIN                          54
#define MOTOR2_DIR_PIN                           55
#define MOTOR2_ENABLE_PIN                        38

#define STEPS_PER_MM                             138.0
#define LOAD_DIST                                52.5*STEPS_PER_MM

#define HEATER_PIN                               45
#define FAN_PIN                                  9
#define TEMP_PIN                                 13

#define BTN_UP_PIN                               19
#define BTN_DOWN_PIN                             18
#define BTN_OK_PIN                               2

#define ENDSTOP_LEFT_PIN                         14
#define ENDSTOP_RIGHT_PIN                        15

#define MIN_TEMP                                 150
#define MAX_TEMP                                 280
#define COOLDOWN_TEMP                            60
#define MAX_SPEED                                300.0*STEPS_PER_MM
#define WELD_SPEED                               2.0*STEPS_PER_MM
#define WELD_DIST_MIN                            1.0*STEPS_PER_MM
#define WELD_DIST_MAX                            15.0*STEPS_PER_MM
#define SWING_SPEED                              30.0*STEPS_PER_MM
#define SWING_DIST                               4.0*STEPS_PER_MM

#define CFG_PID_P                                47.276
#define CFG_PID_I                                13.132
#define CFG_PID_D                                42.549
#define MELT_TIME_MIN                            1
#define MELT_TIME_MAX                            120
