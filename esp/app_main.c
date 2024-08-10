/* Wi-Fi CSI console Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_console.h"

#include "esp_wifi.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "driver/gpio.h"
#include "driver/rmt.h"
#include "hal/uart_ll.h"
#include "led_strip.h"

#include "esp_radar.h"
#include "csi_commands.h"

#include "esp_ota_ops.h"
#include "esp_netif.h"
#include "esp_chip_info.h"

#include "mbedtls/base64.h"

#define RECV_ESPNOW_CSI
#define CONFIG_LESS_INTERFERENCE_CHANNEL    11
#define RADER_EVALUATE_SERVER_PORT          3232

static led_strip_t *g_strip_handle    = NULL;
static xQueueHandle g_csi_info_queue  = NULL;

/***在嵌入式系统和软件开发中, 使用TAG是一种常见的做法。
TAG是一个简短的字符串,它用于标识某个特定的模块、组件或者功能。
当程序输出日志信息时,TAG可以帮助开发者快速定位日志的来源,从而更容易地进行调试和问题排查。
***/
static const char *TAG  = "app_main";


#define HOST_IP_ADDR "192.168.3.2"
#define PORT 12345

static const char *TAGC = "example";
static bool flag = false;
static const char* payload = "有人";

//一个 TCP 客户端任务的实现。让我们逐步分析它的功能
static void tcp_client_task(void *pvParameters)
{
    //远程服务器的 IP 地址
    char host_ip[] = HOST_IP_ADDR;
    //地址族：IPv4
    int addr_family = 0;
    //协议类型，IP
    int ip_protocol = 0;
    struct sockaddr_in dest_addr;
    //将host_ip赋给左值
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    //创建一个套接字
    int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
    if (sock < 0) {
        ESP_LOGE(TAGC, "Unable to create socket: errno %d", errno);
    }
    ESP_LOGI(TAGC, "Socket created, connecting to %s:%d", host_ip, PORT);

    //远程连接服务器
    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
    if (err != 0) {
        ESP_LOGE(TAGC, "Socket unable to connect: errno %d", errno);
    }
    ESP_LOGI(TAGC, "Successfully connected");
    //如果连接成功，使用send（）发送payload
    err = send(sock, payload, strlen(payload), 0);
    if (err < 0) {
        ESP_LOGE(TAGC, "Error occurred during sending: errno %d", errno);
    }
    //关闭套接字
    if (sock != -1) {
        ESP_LOGE(TAGC, "Shutting down socket and restarting...");
        shutdown(sock, 0);
        close(sock);
    }
    vTaskDelete(NULL);
}

//实现了一个 UDP 客户端任务
static void udp_client_task(void *pvParameters) {
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_UDP; // 使用 UDP 协议
    int sock = socket(addr_family, SOCK_DGRAM, ip_protocol); // 使用 SOCK_DGRAM 表示 UDP 套接字
    if (sock < 0) {
        ESP_LOGE(TAGC, "Unable to create socket: errno %d", errno);
    }
    ESP_LOGI(TAGC, "Socket created, connecting to %s:%d", host_ip, PORT);
    

    int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAGC, "Error occurred during sending: errno %d", errno);
    }
    if (sock != -1) {
        ESP_LOGE(TAGC, "Shutting down socket and restarting...");
        close(sock);
    }
    vTaskDelete(NULL);
}


//创建UDP客户端任务
void tcpClientStart(void)
{
   // ESP_ERROR_CHECK(nvs_flash_init());
   // ESP_ERROR_CHECK(esp_netif_init());
   // ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
   // ESP_ERROR_CHECK(example_connect());

    //xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
    xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
}



//初始化 LED 灯带控制器的函数
esp_err_t led_init()
{
#ifdef CONFIG_IDF_TARGET_ESP32C3
#define CONFIG_LED_STRIP_GPIO        GPIO_NUM_8
#elif CONFIG_IDF_TARGET_ESP32S3
#define CONFIG_LED_STRIP_GPIO        GPIO_NUM_48
#else
#define CONFIG_LED_STRIP_GPIO        GPIO_NUM_18
#endif

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_LED_STRIP_GPIO, RMT_CHANNEL_0);
    // set counter clock to 40MHz
    config.clk_div = 2;
    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(1, (led_strip_dev_t)config.channel);
    g_strip_handle = led_strip_new_rmt_ws2812(&strip_config);
    g_strip_handle->set_pixel(g_strip_handle, 0, 255, 255, 255);
    ESP_ERROR_CHECK(g_strip_handle->refresh(g_strip_handle, 100));

    return ESP_OK;
}


//用于设置 LED 灯带的颜色
esp_err_t led_set(uint8_t red, uint8_t green, uint8_t blue)
{
    g_strip_handle->set_pixel(g_strip_handle, 0, red, green, blue);
    g_strip_handle->refresh(g_strip_handle, 100);
    return ESP_OK;
}


//打印设备的一些基本信息
void print_device_info()
{
    //芯片信息
    esp_chip_info_t chip_info = {0};
    const char *chip_name = NULL;
    //固件信息
    const esp_app_desc_t *app_desc = esp_ota_get_app_description();
    esp_netif_ip_info_t local_ip   = {0};
    wifi_ap_record_t ap_info       = {0};

    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &local_ip);
    esp_chip_info(&chip_info);
    esp_wifi_sta_get_ap_info(&ap_info);

    switch (chip_info.model) {
        case CHIP_ESP32:
            chip_name = "ESP32";
            break;

        case CHIP_ESP32S2:
            chip_name = "ESP32-S2";
            break;

        case CHIP_ESP32S3:
            chip_name = "ESP32-S3";
            break;

        case CHIP_ESP32C3:
            chip_name = "ESP32-C3";
            break;

        default:
            chip_name = "Unknown";
            break;
    }

    printf("DEVICE_INFO,%u,%s %s,%s,%d,%s,%s,%d,%d,%s,"IPSTR",%u\n",
           esp_log_timestamp(), app_desc->date, app_desc->time, chip_name,
           chip_info.revision, app_desc->version, app_desc->idf_ver,
           heap_caps_get_total_size(MALLOC_CAP_DEFAULT), esp_get_free_heap_size(),
           ap_info.ssid, IP2STR(&local_ip.ip), RADER_EVALUATE_SERVER_PORT);
}

//初始化 ESP32 设备的 Wi-Fi 功能
static void wifi_init(void)
{
    //初始化 NVS(非易失性存储器)。如果存在 NVS 问题,比如没有可用页面或者新版本,则先擦除 NVS 再进行初始化
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_set_channel(CONFIG_LESS_INTERFERENCE_CHANNEL, WIFI_SECOND_CHAN_BELOW));

#ifdef RECV_ESPNOW_CSI 
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
#endif
}

//结构体radar_args，雷达参数
static struct {
    struct arg_lit *train_start;//指示开始训练的开关型参数
    struct arg_lit *train_stop;//指示停止训练的开关型参数
    struct arg_lit *train_add;//指示添加训练数据的开关型参数
    struct arg_str *predict_someone_threshold;//预测某人时的阈值参数
    struct arg_str *predict_move_threshold;//预测运动时的阈值参数
    struct arg_int *predict_buff_size;//预测缓冲区大小参数
    struct arg_int *predict_outliers_number;//预测异常数据数量参数
    struct arg_str *collect_taget;//收集目标参数
    struct arg_int *collect_number;//收集数量参数
    struct arg_int *collect_duration;//收集时长参数
    struct arg_lit *csi_start;//指示开始 CSI 的开关型参数
    struct arg_lit *csi_stop;//指示停止 CSI 的开关型参数
    struct arg_str *csi_output_type;//CSI 输出类型参数
    struct arg_str *csi_output_format;//CSI 输出格式参数
    struct arg_end *end;//参数列表的结束标记
} radar_args;

//定义了一个结构体 console_input_config，并创建了一个静态全局变量 g_console_input_config，并对其进行了初始化赋值
static struct console_input_config {
    bool train_start;
    float predict_someone_threshold;
    float predict_move_threshold;
    uint32_t predict_buff_size;
    uint32_t predict_outliers_number;
    char collect_taget[16];
    uint32_t collect_number;
    char csi_output_type[16];
    char csi_output_format[16];
} g_console_input_config = {
    .predict_someone_threshold = 0.001,
    .predict_move_threshold    = 0.001,
    .predict_buff_size         = 5,
    .predict_outliers_number   = 2,
    .train_start               = false,
    .collect_taget             = "unknown",
    .csi_output_type           = "LLFT",
    .csi_output_format         = "decimal"
};

static TimerHandle_t g_collect_timer_handele = NULL;

//定时器回调函数,它被用来管理数据收集的过程
static void collect_timercb(void *timer)
{
    //递减 g_console_input_config.collect_number 变量的值,表示已经收集了一次数据
    g_console_input_config.collect_number--;

    //如果 g_console_input_config.collect_number 变量的值降到 0,说明数据收集已经完成
    if (!g_console_input_config.collect_number) {
        //停止并删除 g_collect_timer_handele 定时器。
        //将 g_console_input_config.collect_taget 变量设置为 "unknown"
        xTimerStop(g_collect_timer_handele, 0);
        xTimerDelete(g_collect_timer_handele, 0);
        g_collect_timer_handele = NULL;
        strcpy(g_console_input_config.collect_taget, "unknown");
        return;
    }
}

//处理命令行参数的函数,用于控制雷达相关的功能
static int wifi_cmd_radar(int argc, char **argv)
{
    //解析命令行参数,将其存储在 radar_args 结构体中。如果解析失败,则打印错误信息并返回 ESP_FAIL
    if (arg_parse(argc, argv, (void **) &radar_args) != ESP_OK) {
        arg_print_errors(stderr, radar_args.end, argv[0]);
        return ESP_FAIL;
    }

    //开始或停止雷达训练
    if (radar_args.train_start->count) {
        if (!radar_args.train_add->count) {
            esp_radar_train_remove();
        }

        esp_radar_train_start();
        g_console_input_config.train_start = true;
    }

    //设置预测阈值、缓冲区大小和异常数据数量
    if (radar_args.train_stop->count) {
        esp_radar_train_stop(&g_console_input_config.predict_someone_threshold,
                             &g_console_input_config.predict_move_threshold);
        g_console_input_config.predict_someone_threshold *= 1.1;
        g_console_input_config.predict_move_threshold    *= 1.1;
        g_console_input_config.train_start               = false;

        printf("RADAR_DADA,0,0,0,%.6f,0,0,%.6f,0\n", 
                g_console_input_config.predict_someone_threshold,
                g_console_input_config.predict_move_threshold);
    }

    //开始或停止数据采集,并设置采集目标、数量和持续时间
    if (radar_args.predict_move_threshold->count) {
        g_console_input_config.predict_move_threshold = atof(radar_args.predict_move_threshold->sval[0]);
    }

    if (radar_args.predict_someone_threshold->count) {
        g_console_input_config.predict_someone_threshold = atof(radar_args.predict_someone_threshold->sval[0]);
    }

    if (radar_args.predict_buff_size->count) {
        g_console_input_config.predict_buff_size = radar_args.predict_buff_size->ival[0];
    }

    if (radar_args.predict_outliers_number->count) {
        g_console_input_config.predict_outliers_number = radar_args.predict_outliers_number->ival[0];
    }

    if (radar_args.collect_taget->count && radar_args.collect_number->count && radar_args.collect_duration->count) {
        g_console_input_config.collect_number = radar_args.collect_number->ival[0];
        strcpy(g_console_input_config.collect_taget, radar_args.collect_taget->sval[0]);

        if (g_collect_timer_handele) {
            xTimerStop(g_collect_timer_handele, portMAX_DELAY);
            xTimerDelete(g_collect_timer_handele, portMAX_DELAY);
        }

        g_collect_timer_handele = xTimerCreate("collect", pdMS_TO_TICKS(radar_args.collect_duration->ival[0]),
                                               true, NULL, collect_timercb);
        xTimerStart(g_collect_timer_handele, portMAX_DELAY);
    }

    //设置 CSI 输出类型和格式
    if (radar_args.csi_output_format->count) {
        strcpy(g_console_input_config.csi_output_format, radar_args.csi_output_format->sval[0]);
    }

    if (radar_args.csi_output_type->count) {
        wifi_radar_config_t radar_config = {0};
        esp_radar_get_config(&radar_config);

        if (!strcasecmp(radar_args.csi_output_type->sval[0], "NULL")) {
            radar_config.wifi_csi_filtered_cb = NULL;
        } else {
            void wifi_csi_raw_cb(const wifi_csi_filtered_info_t *info, void *ctx);
            radar_config.wifi_csi_filtered_cb = wifi_csi_raw_cb;
            strcpy(g_console_input_config.csi_output_type, radar_args.csi_output_type->sval[0]);
        }

        esp_radar_set_config(&radar_config);
    }

    //开始或停止 CSI 数据采集
    if (radar_args.csi_start->count) {
        esp_radar_csi_start();
    }

    if (radar_args.csi_stop->count) {
        esp_radar_csi_stop();
    }

    return ESP_OK;
}

//用于注册和配置雷达相关的命令行参数和命令
void cmd_register_radar(void)
{
    //使用 arg_lit0()、arg_str0() 和 arg_int0() 等函数定义了一系列命令行参数，包括雷达训练相关的参数、预测阈值参数、数据采集参数、CSI 数据输出参数等
    radar_args.train_start = arg_lit0(NULL, "train_start", "Start calibrating the 'Radar' algorithm");
    radar_args.train_stop  = arg_lit0(NULL, "train_stop", "Stop calibrating the 'Radar' algorithm");
    radar_args.train_add   = arg_lit0(NULL, "train_add", "Calibrate on the basis of saving the calibration results");

    radar_args.predict_someone_threshold = arg_str0(NULL, "predict_someone_threshold", "<0 ~ 1.0>", "Configure the threshold for someone");
    radar_args.predict_move_threshold    = arg_str0(NULL, "predict_move_threshold", "<0 ~ 1.0>", "Configure the threshold for move");
    radar_args.predict_buff_size         = arg_int0(NULL, "predict_buff_size", "1 ~ 100", "Buffer size for filtering outliers");
    radar_args.predict_outliers_number   = arg_int0(NULL, "predict_outliers_number", "<1 ~ 100>", "The number of items in the buffer queue greater than the threshold");

    radar_args.collect_taget    = arg_str0(NULL, "collect_tagets", "<0 ~ 20>", "Type of CSI data collected");
    radar_args.collect_number   = arg_int0(NULL, "collect_number", "sequence", "Number of times CSI data was collected");
    radar_args.collect_duration = arg_int0(NULL, "collect_duration", "duration", "Time taken to acquire one CSI data");

    radar_args.csi_start  = arg_lit0(NULL, "csi_start", "Start collecting CSI data from Wi-Fi");
    radar_args.csi_stop   = arg_lit0(NULL, "csi_stop", "Stop CSI data collection from Wi-Fi");
    radar_args.csi_output_type   = arg_str0(NULL, "csi_output_type", "<NULL, LLFT, HT-LFT, STBC-HT-LTF>", "Type of CSI data");
    radar_args.csi_output_format = arg_str0(NULL, "csi_output_format", "<decimal, base64>", "Format of CSI data");
    radar_args.end               = arg_end(8);

    //创建了一个 radar_cmd 结构体，其中包含了雷达相关命令的信息，包括命令名称、帮助信息、处理函数指针、以及参数表等
    const esp_console_cmd_t radar_cmd = {
        .command = "radar",
        .help = "Radar config",
        .hint = NULL,
        .func = &wifi_cmd_radar,
        .argtable = &radar_args
    };

    //调用 esp_console_cmd_register() 函数注册了这个雷达相关的命令，使得在控制台中可以通过输入相应的命令来操作雷达功能
    ESP_ERROR_CHECK(esp_console_cmd_register(&radar_cmd));
}

//处理CSI数据并打印输出
static void csi_data_print_task(void *arg)
{
    //存储CSI数据信息
    wifi_csi_filtered_info_t *info = NULL;
    //缓冲区
    char *buffer = malloc(8 * 1024);
    //接受到的CSI信息的数量
    static uint32_t count = 0;

    //使用xQueueReceive函数从队列g_csi_info_queue中接收 CSI 数据信息，如果没有数据则会一直等待。
    while (xQueueReceive(g_csi_info_queue, &info, portMAX_DELAY)) {
        //记录当前输出字符串的长度。然后获取了info结构体中的rx_ctrl字段，这是一个指向wifi_pkt_rx_ctrl_t结构体的指针
        size_t len = 0;
        wifi_pkt_rx_ctrl_t *rx_ctrl = &info->rx_ctrl;

        //第一次输出CSI信息时
        if (!count) {
            ESP_LOGI(TAG, "================ CSI RECV ================");
            len += sprintf(buffer + len, "type,sequence,timestamp,taget_seq,taget,mac,rssi,rate,sig_mode,mcs,bandwidth,smoothing,not_sounding,aggregation,stbc,fec_coding,sgi,noise_floor,ampdu_cnt,channel,secondary_channel,local_timestamp,ant,sig_len,rx_state,len,first_word,data\n");
        }

        //根据用户配置的csi_output_type来设置info->valid_len的值。CSI的有效长度
        if (!strcasecmp(g_console_input_config.csi_output_type, "LLFT")) {
            info->valid_len = info->valid_llft_len;
        } else if (!strcasecmp(g_console_input_config.csi_output_type, "HT-LFT")) {
            info->valid_len = info->valid_llft_len + info->valid_ht_lft_len;
        } else if (!strcasecmp(g_console_input_config.csi_output_type, "STBC-HT-LTF")) {
            info->valid_len = info->valid_llft_len + info->valid_ht_lft_len + info->valid_stbc_ht_lft_len;
        }

        //格式化 CSI 数据的输出。具体分析如下:
        len += sprintf(buffer + len, "CSI_DATA,%d,%u,%u,%s," MACSTR ",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%d,%d,%d,%d,%d,",
                       count++, esp_log_timestamp(), g_console_input_config.collect_number, g_console_input_config.collect_taget,
                       MAC2STR(info->mac), rx_ctrl->rssi, rx_ctrl->rate, rx_ctrl->sig_mode,
                       rx_ctrl->mcs, rx_ctrl->cwb, rx_ctrl->smoothing, rx_ctrl->not_sounding,
                       rx_ctrl->aggregation, rx_ctrl->stbc, rx_ctrl->fec_coding, rx_ctrl->sgi,
                       rx_ctrl->noise_floor, rx_ctrl->ampdu_cnt, rx_ctrl->channel, rx_ctrl->secondary_channel,
                       rx_ctrl->timestamp, rx_ctrl->ant, rx_ctrl->sig_len, rx_ctrl->rx_state, info->valid_len, 0);

        // ESP_LOGW(TAG, "csi_output_format: %s", g_console_input_config.csi_output_format);

        //处理CSI数据的输出格式
        if (!strcasecmp(g_console_input_config.csi_output_format, "base64")) {
            //base64格式
            size_t size = 0;
            mbedtls_base64_encode((uint8_t *)buffer + len, sizeof(buffer) - len, &size, (uint8_t *)info->valid_data, info->valid_len);
            len += size;
            len += sprintf(buffer + len, "\n");
        } else {
            //非base64格式
            len += sprintf(buffer + len, "\"[%d", info->valid_data[0]);

            for (int i = 1; i < info->valid_len; i++) {
                len += sprintf(buffer + len, ",%d", info->valid_data[i]);
            }

            len += sprintf(buffer + len, "]\"\n");
        }

        //输出CSI数据
        printf("%s", buffer);
        free(info);
    }

    //释放空间
    free(buffer);
    vTaskDelete(NULL);
}

//处理原始数据
//回调函数是什么？ctx：上下文内容
//回调函数：当用户触发某个事件(如点击按钮)时,系统会调用相应的回调函数来处理这个事件。再比如在I/O操作中,当数据准备好时,系统会调用回调函数来处理数据。
void wifi_csi_raw_cb(const wifi_csi_filtered_info_t *info, void *ctx)
{
    wifi_csi_filtered_info_t *q_data = malloc(sizeof(wifi_csi_filtered_info_t) + info->valid_len);
    *q_data = *info;
    memcpy(q_data->valid_data, info->valid_data, info->valid_len);

    if (!g_csi_info_queue || xQueueSend(g_csi_info_queue, &q_data, 0) == pdFALSE) {
        ESP_LOGW(TAG, "g_csi_info_queue full");
        free(q_data);
    }
}

//处理Wi-Fi雷达功能相关的事件或数据。让我逐步解释这个函数的功能：
static void wifi_radar_cb(const wifi_radar_info_t *info, void *ctx)
{
    //声明了两个静态指针s_buff_wander和s_buff_jitter，用于保存雷达信息的漂移和抖动数据。
    static float *s_buff_wander = NULL;
    static float *s_buff_jitter = NULL;

    //当s_buff_wander和s_buff_jitter为空时，分配内存空间并初始化
    if (!s_buff_wander) {
        s_buff_wander = calloc(100, sizeof(float));
    }

    if (!s_buff_jitter) {
        s_buff_jitter = calloc(100, sizeof(float));
    }

    //静态uint32_t类型变量，用于记录当前数据缓冲区中已存储的数据点数量。
    static uint32_t s_buff_count          = 0;
    //uint32_t类型变量，用于存储配置参数中的数据缓冲区最大大小。
    uint32_t buff_max_size      = g_console_input_config.predict_buff_size;
    //uint32_t类型变量，用于存储配置参数中的数据异常值数量阈值。
    uint32_t buff_outliers_num  = g_console_input_config.predict_outliers_number;
    //uint32_t类型变量，用于统计数据缓冲区中超过某个阈值的数据点数量，用于判断是否有人出现。
    uint32_t someone_count = 0;
    //uint32_t类型变量，用于统计数据缓冲区中超过某个阈值的数据点数量，用于判断是否有移动发生。
    uint32_t move_count    = 0;
    //bool类型变量，用于记录房间状态，true表示房间有人。
    bool room_status       = false;
    //bool类型变量，用于记录人体移动状态，true表示有人体移动。
    bool human_status      = false;

    //将当前接收到的漂移数据info->waveform_wander存储到漂移数据缓冲区s_buff_wander中，使用取模运算符%和缓冲区最大大小buff_max_size来循环利用缓冲区空间。
    s_buff_wander[s_buff_count % buff_max_size] = info->waveform_wander;
    //将当前接收到的抖动数据info->waveform_jitter存储到抖动数据缓冲区s_buff_jitter中，同样使用取模运算符%和缓冲区最大大小buff_max_size来循环利用缓冲区空间。
    s_buff_jitter[s_buff_count % buff_max_size] = info->waveform_jitter;
    //更新数据点数量计数器s_buff_count，表示当前缓冲区中已存储的数据点数量加一。
    s_buff_count++;

    if (s_buff_count < buff_max_size) {
        return;
    }

    //遍历漂移数据缓冲区s_buff_wander中的数据，并统计超过某个阈值的数据点数量，记录有人的个数
    for (int i = 0; i < buff_max_size; i++) {
        if (s_buff_wander[i] > g_console_input_config.predict_someone_threshold) {
            someone_count++;
        }

        if (s_buff_jitter[i] > g_console_input_config.predict_move_threshold) {
            move_count++;
        }
    }

    if (someone_count >= buff_outliers_num) {
        room_status = true;
    }

    if (move_count >= buff_outliers_num) {
        human_status = true;
    }

    //对打印日志的次数计数
    static uint32_t s_count = 0;

    //第一次时打印以下内容
    if (!s_count) {
        ESP_LOGI(TAG, "================ RADAR RECV ================");
        ESP_LOGI(TAG, "type,sequence,timestamp,waveform_wander,someone_threshold,someone_status,waveform_jitter,move_threshold,move_status\n");
    }

    //时间戳
    char timestamp_str[32] = {0};
    sprintf(timestamp_str, "%u", esp_log_timestamp());


    if (ctx) {
        strncpy(timestamp_str, (char *)ctx, 31);
    }

    //打印状态
    printf("RADAR_DADA,%d,%s,%.6f,%.6f,%d,%.6f,%.6f,%d\n",
           s_count++, timestamp_str,
           info->waveform_wander, g_console_input_config.predict_someone_threshold, room_status,
           info->waveform_jitter, g_console_input_config.predict_move_threshold, human_status);

    //移动和有人的时间戳
    static uint32_t s_last_move_time = 0;
    static uint32_t s_last_someone_time = 0;

    //训练模式下，执行以下代码，通过led灯展示效果
    if (g_console_input_config.train_start) {
        static bool led_status = false;

        if (led_status) {
            led_set(0, 0, 0);
        } else {
            led_set(255, 255, 0);
        }

        led_status = !led_status;

        return;
    }

    //判断房间状态
    if (room_status) {
        if (human_status) {
            led_set(0, 255, 0);
            ESP_LOGI(TAG, "Someone moved");
            // //tcp client
            // tcpClientStart();
            // ESP_LOGI(TAG, "tcp client start");
            s_last_move_time = esp_log_timestamp();
        } else if (esp_log_timestamp() - s_last_move_time > 3 * 1000) {
            led_set(0, 0, 255);
            ESP_LOGI(TAG, "Someone");
        }

        s_last_someone_time = esp_log_timestamp();
    } else if (esp_log_timestamp() - s_last_someone_time > 3 * 1000) {
        if (human_status) {
            led_set(255, 0, 0);
            flag = true;
        } else {
            led_set(255, 255, 255);
            flag = false;
        }
    }
    //tcp client
    if(flag){
        tcpClientStart();
        ESP_LOGI(TAG, "tcp client start");
    }
    
}

/* Event handler for catching system events */
//事件处理函数,用于处理 Wi-Fi 相关的事件
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    //IP_EVENT_STA_GOT_IP事件
    //这个事件处理函数的主要目的是在 Wi-Fi 成功连接到 AP 后,对雷达相关的配置进行初始化和设置,并启动一些必要的服务
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        //获取当前连接的 AP 的 BSSID 信息,并将其设置到雷达配置中的 filter_mac 字段
        wifi_radar_config_t radar_config = {0};
        wifi_ap_record_t ap_info;

        esp_wifi_sta_get_ap_info(&ap_info);
        esp_radar_get_config(&radar_config);
        memcpy(radar_config.filter_mac, ap_info.bssid, sizeof(ap_info.bssid));
        esp_radar_set_config(&radar_config);

        //调用 print_device_info() 函数打印设备信息
        print_device_info();

        //运行 ping 命令,检查网络连接
        esp_err_t ret   = ESP_OK;
        const char *ping = "ping";
        ESP_ERROR_CHECK(esp_console_run(ping, &ret));

        //调用 rader_evaluate_server() 函数启动雷达评估服务器
        extern esp_err_t rader_evaluate_server(uint32_t port);
        //如果定义了 RECV_ESPNOW_CSI 宏,则停止 Wi-Fi 的混杂模式
        rader_evaluate_server(RADER_EVALUATE_SERVER_PORT);

#ifdef RECV_ESPNOW_CSI
        ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));
#endif
    }
}

void app_main(void)
{
    /**
     * @brief Install ws2812 driver, Used to display the status of the device
     */
    //初始化led灯
    led_init();

    /**
     * @brief Turn off the radar module printing information
     */
    // esp_log_level_set(ESP_LOG_WARN, "esp_radar");

    /**
     * @brief Register serial command
     */
    //RELP:串行命令行界面
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();//外观
    repl_config.prompt = "csi>";
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));//创建一个RELP实例

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
    /**< Fix serial port garbled code due to high baud rate */
    uart_ll_set_sclk(UART_LL_GET_HW(CONFIG_ESP_CONSOLE_UART_NUM), UART_SCLK_APB);
    uart_ll_set_baudrate(UART_LL_GET_HW(CONFIG_ESP_CONSOLE_UART_NUM), CONFIG_ESP_CONSOLE_UART_BAUDRATE);
#endif

    cmd_register_system();
    cmd_register_ping();
    cmd_register_wifi_config();
    cmd_register_wifi_scan();
    cmd_register_radar();
    ESP_ERROR_CHECK(esp_console_start_repl(repl));

    /**
     * @brief Initialize Wi-Fi Radar
     */
    wifi_radar_config_t radar_config = {
        //检测到物体移动时，处理相关信息
        //.wifi_radar_cb 是一个指向回调函数的结构体成员,它将在 Wi-Fi 雷达检测到物体移动时被调用
        .wifi_radar_cb        = wifi_radar_cb,
        // .wifi_csi_filtered_cb = wifi_csi_raw_cb,
        //过滤要监测的设备
        .filter_mac           = {0x1a, 0x00, 0x00, 0x00, 0x00, 0x00},
    };
    //初始化WiFi
    wifi_init();
    //初始化雷达
    esp_radar_init();
    //配置
    esp_radar_set_config(&radar_config);
    //开启雷达，进行监测
    esp_radar_start();

    /**
     * @brief Initialize CSI serial port printing task, Use tasks to avoid blocking wifi_csi_raw_cb
     */
    //这行代码创建了一个队列用于在CSI串口打印任务和其他任务之间传递数据。队列的容量为64,每个元素大小为void *指针的大小。
    g_csi_info_queue = xQueueCreate(64, sizeof(void *));
    //这行代码创建了一个名为csi_data_print_task的任务,用于处理CSI数据打印。该任务的栈大小为4KB,参数为NULL,优先级为0,没有传递额外参数。
    xTaskCreate(csi_data_print_task, "csi_data_print", 4 * 1024, NULL, 0, NULL);

    //这行代码注册了一个事件处理程序,用于处理Wi-Fi连接成功获取IP地址后的事件。当设备成功连接到Wi-Fi网络并获取到IP地址时,将调用wifi_event_handler函数来处理该事件
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
}
