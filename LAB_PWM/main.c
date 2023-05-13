#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_BRING_UP_PWM "pwmchip0/export"
#define PWM_CHANNEL_0 0
#define PWM0_PATH "pwmchip0/pwm0"
#define PERIOD 100000

void bring_up_PWM(int pwm_channel, char *path_bring_up);
void set_PWM(int period, int duty, char *path_set);
void enable_disable_PWM(int value, char *path_status);

int main()
{
    bring_up_PWM(PWM_CHANNEL_0, PATH_BRING_UP_PWM);
    enable_disable_PWM(1, PWM0_PATH);

    int duty;
    while (1)
    {
        for (duty = 0; duty <= PERIOD; duty += 1000)
        {
            set_PWM(PERIOD, duty, PWM0_PATH);
            usleep(50000); // sleep for 50ms
        }
        for (duty = PERIOD; duty >= 0; duty -= 1000)
        {
            set_PWM(PERIOD, duty, PWM0_PATH);
            usleep(50000); // sleep for 50ms
        }
    }
    return 0;
}

void bring_up_PWM(int pwm_channel, char *path_bring_up)
{
    char path[64];
    FILE *pwm_file;

    // bring_up_PWM
    sprintf(path, "/sys/class/pwm/%s", path_bring_up);
    pwm_file = fopen(path, "w");
    if (pwm_file == NULL)
    {
        printf("Cannot open %s.\n", path);
        exit(1);
    }
    fprintf(pwm_file, "%d", pwm_channel);
    fclose(pwm_file);

    printf("bring_up_PWM sucessfully\n");
}

void set_PWM(int period, int duty, char *path_set)
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

void enable_disable_PWM(int value, char *path_status)
{
    char path[128];
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
    printf("enable_disable_PWM sucessfully\n");
}