/*
 * Shell commands for our sensorled custom sensor.
 */

#include <zephyr/shell/shell.h>

static int cmd_foo(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "I said foo");
    return 0;
}

static int cmd_bar(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "I said bar");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_foobar,
    SHELL_CMD(foo, NULL, "Amazing foo command.", cmd_foo),
    SHELL_CMD(bar, NULL, "Amazing bar command.", cmd_bar),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(foobar, &sub_foobar, "Foobar commands", NULL);

