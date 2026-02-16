#define _POSIX_C_SOURCE 200809L // have to add this for sa to not be squigly. weird 
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN_FD 0
#define SPEED_MIN 0
#define BUF_SIZE 256

static volatile sig_atomic_t inc_pending = 0;
static volatile sig_atomic_t dec_pending = 0;

static void handle_signal(int sig)
{
    if (sig == SIGUSR1)
    {
        ++dec_pending;
    }
    else if (sig == SIGUSR2)
    {
        ++inc_pending;
    }
}

static void change_speed(int *speed, int delta, const char *label)
{
    int updated = *speed + delta;

    if (updated < SPEED_MIN)
    {
        updated = SPEED_MIN;
    }

    *speed = updated;
    printf("%s speed to: %d\n", label, *speed);
    fflush(stdout);
}

static void apply_pending_signals(int *speed)
{
    while (inc_pending > 0)
    {
        --inc_pending;
        change_speed(speed, 1, "increase");
    }

    while (dec_pending > 0)
    {
        --dec_pending;
        change_speed(speed, -1, "decrease");
    }
}

int main(void)
{
    struct sigaction sa;
    sigset_t block_mask;
    sigset_t orig_mask;
    int speed = 0;

    printf("PID=%d\n", getpid());
    fflush(stdout);

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction SIGUSR1");
        return EXIT_FAILURE;
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        perror("sigaction SIGUSR2");
        return EXIT_FAILURE;
    }

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);
    sigaddset(&block_mask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    for (;;)
    {
        fd_set read_fds;
        char buffer[BUF_SIZE];
        ssize_t bytes_read;

        apply_pending_signals(&speed);

        FD_ZERO(&read_fds);
        FD_SET(STDIN_FD, &read_fds);

        if (pselect(STDIN_FD + 1, &read_fds, NULL, NULL, NULL, &orig_mask) == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("pselect");
            return EXIT_FAILURE;
        }

        if (!FD_ISSET(STDIN_FD, &read_fds))
        {
            continue;
        }

        bytes_read = read(STDIN_FD, buffer, sizeof(buffer));
        if (bytes_read == 0)
        {
            break;
        }
        if (bytes_read < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("read");
            return EXIT_FAILURE;
        }

        for (ssize_t i = 0; i < bytes_read; ++i)
        {
            char command = buffer[i];

            if (command == '+')
            {
                change_speed(&speed, 1, "increase");
            }
            else if (command == '-')
            {
                change_speed(&speed, -1, "decrease");
            }
            else if (command == 'q')
            {
                return 0;
            }
        }
    }

    return 0;
}