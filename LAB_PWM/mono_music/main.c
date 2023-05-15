#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define PWM0_PATH "pwmchip0/pwm0"
#define PWM_CHANNEL_0 0
#define PERIOD 100000

#define PWM_CONTROLLER_EXPORT "pwmchip0/export"
#define PWM_CONTROLLER_UNEXPORT "pwmchip0/unexport"

#define PWM_DISABLE 0
#define PWM_ENABLE 1

/*************************************************
 * Public Constants pitches.h
 *************************************************/
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Định nghĩa các nốt nhạc cho bài hát
int melody[] = {
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
    NOTE_F4, NOTE_F4,
    NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4,
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
    NOTE_F4, NOTE_F4,
    NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4};

// Định nghĩa thời lượng cho mỗi nốt nhạc
int noteDurations[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8, 2,
    8, 8,
    8, 8, 8, 8, 8, 2,
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8, 2,
    8, 8,
    8, 8, 8, 8, 8, 2};

void sigint_handler(int sig_num);
void PWM_controller(int pwm_channel, char *path_controller);
void PWM_status(int value, char *path_status);
void set_PWM_period(int period, char *path_set);
void set_PWM_duty(int duty, char *path_set);

int main()
{
    signal(SIGINT, sigint_handler);
    PWM_controller(PWM_CHANNEL_0, PWM_CONTROLLER_EXPORT);

    // Chơi từng nốt nhạc trong bài hát
    for (int i = 0; i < sizeof(melody) / sizeof(int); i++)
    {
        int note = melody[i];
        int note_duration = noteDurations[i];

        // Đặt tần số (period) của PWM để phù hợp với nốt nhạc
        set_PWM_period(1000000 / note, PWM0_PATH); // 1s/Hz = us
        set_PWM_duty(500000 / note, PWM0_PATH);    // 50% duty cycle

        // Enable PWM để chơi nốt nhạc
        PWM_status(PWM_ENABLE, PWM0_PATH);

        // Đợi cho đến khi nốt nhạc kết thúc
        usleep(note_duration * 125000); // 125 ms cho mỗi đơn vị thời gian

        // Tắt PWM sau khi chơi nốt nhạc
        PWM_status(PWM_DISABLE, PWM0_PATH);

        // Đợi một chút trước khi chơi nốt nhạc tiếp theo
        usleep(30000); // Đợi 30 ms
    }

    // Tắt PWM khi kết thúc
    sigint_handler(SIGINT);

    return 0;
}

void sigint_handler(int sig_num)
{
    /* Reset handler to catch SIGINT next time. */
    signal(SIGINT, sigint_handler);
    printf("\nTerminating and turning off PWM...\n");

    set_PWM_duty(0, PWM0_PATH);
    set_PWM_period(0, PWM0_PATH);
    PWM_status(PWM_DISABLE, PWM0_PATH);
    PWM_controller(PWM_CHANNEL_0, PWM_CONTROLLER_UNEXPORT);

    exit(0);
}

void PWM_controller(int pwm_channel, char *path_controller)
{
    char path[64];
    FILE *pwm_file;

    if (strcmp(path_controller, PWM_CONTROLLER_EXPORT) == 0)
    {
        // bring_up_pwm0
        sprintf(path, "/sys/class/pwm/%s", path_controller);
        pwm_file = fopen(path, "w");
        if (pwm_file == NULL)
        {
            printf("Cannot open %s.\n", path);
            exit(1);
        }
        fprintf(pwm_file, "%d", pwm_channel);
        fclose(pwm_file);

        printf("bring_up_pwm0 successfully\n");
    }

    if (strcmp(path_controller, PWM_CONTROLLER_UNEXPORT) == 0)
    {
        // delete_pwm0
        sprintf(path, "/sys/class/pwm/%s", path_controller);
        pwm_file = fopen(path, "w");
        if (pwm_file == NULL)
        {
            printf("Cannot open %s.\n", path);
            exit(1);
        }
        fprintf(pwm_file, "%d", pwm_channel);
        fclose(pwm_file);

        printf("delete_pwm0 successfully\n");
    }
}

void PWM_status(int value, char *path_status)
{
    char path[64];
    FILE *pwm_file;

    // Enable PWM
    sprintf(path, "/sys/class/pwm/%s/enable", path_status);
    pwm_file = fopen(path, "w");
    if (pwm_file == NULL)
    {
        printf("Cannot open %s.\n", path);
        exit(1);
    }
    fprintf(pwm_file, "%d", value);
    fclose(pwm_file);

    // Open the file again in read mode and print its content
    pwm_file = fopen(path, "r");
    if (pwm_file != NULL)
    {
        char buffer[64];
        fgets(buffer, sizeof(buffer), pwm_file);
        printf("Content written to %s: %s\n", path, buffer);
        fclose(pwm_file);
    }

    printf("PWM_status successfully\n");
}

void set_PWM_period(int period, char *path_set)
{
    char path[64];
    FILE *pwm_file;

    // Set period
    sprintf(path, "/sys/class/pwm/%s/period", path_set);
    pwm_file = fopen(path, "w");
    if (pwm_file == NULL)
    {
        printf("Cannot open %s.\n", path);
        exit(1);
    }
    fprintf(pwm_file, "%d", period);
    fclose(pwm_file);
}

void set_PWM_duty(int duty, char *path_set)
{
    char path[64];
    FILE *pwm_file;

    // Set duty cycle
    sprintf(path, "/sys/class/pwm/%s/duty_cycle", path_set);
    pwm_file = fopen(path, "w");
    if (pwm_file == NULL)
    {
        printf("Cannot open %s.\n", path);
        exit(1);
    }
    fprintf(pwm_file, "%d", duty);
    fclose(pwm_file);
}
