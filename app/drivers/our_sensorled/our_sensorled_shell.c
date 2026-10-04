/*
 * Shell commands for our sensorled custom sensor.
 */

#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *sensorled = DEVICE_DT_GET(DT_NODELABEL(our_sensorled0));
    int ret;

    if (!device_is_ready(sensorled))
        shell_print(sh, "Sensor not ready");
    else {
        ret = sensor_sample_fetch(sensorled);
        if (ret < 0)
            shell_print(sh, "fetch failed, ret %d", ret);
        else
            shell_print(sh, "fetched ok");
    }
    return 0;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *sensorled = DEVICE_DT_GET(DT_NODELABEL(our_sensorled0));
    struct sensor_value value;
    int ret;

    if (!device_is_ready(sensorled))
        shell_print(sh, "Sensor not ready");
    else {
        ret = sensor_channel_get(sensorled, SENSOR_CHAN_AMBIENT_TEMP, &value);
        if (ret < 0)
            shell_print(sh, "read failed, ret %d", ret);
        else
            shell_print(sh, "read ok");
    }
    return 0;
}

static int cmd_info(const struct shell *sh, size_t argc, char **argv)
{

    const struct device *sensorled = DEVICE_DT_GET(DT_NODELABEL(our_sensorled0));
    int is_ready;

    is_ready = device_is_ready(sensorled);
    shell_print(sh, "info: %s, %s", sensorled->name, is_ready ? "ready" : "not ready");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcommands,
    SHELL_CMD(fetch, NULL, "Call sensor fetch handler.", cmd_fetch),
    SHELL_CMD(read, NULL, "Call sensor get handler", cmd_read),
    SHELL_CMD(info, NULL, "Print device name and ready.", cmd_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_subcommands, "Sensor commands", NULL);

