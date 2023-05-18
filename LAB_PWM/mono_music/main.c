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

// TONES
#define C 2100
#define D 1870
#define E 1670
#define f 1580 // Does not seem to like capital F
#define G 1400
// Define a special note, 'R', to represent a rest
#define R 0

// Định nghĩa các nốt nhạc cho bài hát
int melody[] = {E, E, E, R,
                E, E, E, R,
                E, G, C, D, E, R,
                f, f, f, f, f, E, E, E, E, D, D, E, D, R, G, R,
                E, E, E, R,
                E, E, E, R,
                E, G, C, D, E, R,
                f, f, f, f, f, E, E, E, G, G, f, D, C, R};

// Định nghĩa thời lượng cho mỗi nốt nhạc
int noteDurations[] = {4, 4, 4, 4,
                       4, 4, 4, 4,
                       4, 4, 4, 4, 4, 4,
                       4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                       4, 4, 4, 4,
                       4, 4, 4, 4,
                       4, 4, 4, 4, 4, 4,
                       4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

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

        if (note != R)
        {                                                 // Only perform division if note is not a rest
            set_PWM_period(1000000000 / note, PWM0_PATH); // 1s/Hz = us
            set_PWM_duty(500000000 / note, PWM0_PATH);    // 50% duty cycle
            // Enable PWM to play the note
            PWM_status(PWM_ENABLE, PWM0_PATH);
        }
        else
        {
            // Rest (note == R), so disable PWM
            PWM_status(PWM_DISABLE, PWM0_PATH);
        }

        // Đợi cho đến khi nốt nhạc kết thúc
        usleep(note_duration * 125000); // 125 ms cho mỗi đơn vị thời gian

        // Tắt PWM sau khi chơi nốt nhạc
        PWM_status(PWM_DISABLE, PWM0_PATH);

        // Đợi một chút trước khi chơi nốt nhạc tiếp theo
        usleep(1000); // Đợi 1 ms
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
