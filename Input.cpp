#include "Input.hpp"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>

static termios g_oldTerm{};
static bool g_inited = false;

static void restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &g_oldTerm);
}

static void initTerminalOnce() {
    if (g_inited) return;
    g_inited = true;

    tcgetattr(STDIN_FILENO, &g_oldTerm);

    termios raw = g_oldTerm;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    std::atexit(restoreTerminal);
}

char readKey() {
    initTerminalOnce();

    char c = 0;
    ssize_t n = ::read(STDIN_FILENO, &c, 1);
    if (n == 1) return c;
    return 0;
}