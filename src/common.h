#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int check_mount(char *s);
void prepare_update(void);
void update_uptime(void);
void update_meminfo(void);
void update_net_stats(void);
void update_cpu_usage(void);
void update_total_processes(void);
void update_uname(void);
void update_running_processes(void);
void update_i8k(void);
void update_stuff(void);
char get_freq(char *, size_t, const char *, int, unsigned int);
void get_freq_dynamic(char *, size_t, const char *, int);
char get_voltage(char *, size_t, const char *, int, unsigned int);	/* ptarjan */
void update_load_average(void);
void update_top(void);
void free_all_processes(void);
struct process *get_first_process(void);
void get_cpu_count(void);
double get_time(void);

FILE *open_file(const char *, int *);
void variable_substitute(const char *s, char *dest, unsigned int n);

void format_seconds(char *buf, unsigned int n, long t);
void format_seconds_short(char *buf, unsigned int n, long t);

#ifdef X11
void update_x11info(void);
#endif

int round_to_int_temp(float);

unsigned int round_to_int(float);

extern unsigned long long need_mask;
extern int no_buffers;

struct dns_data {
        int nscount;
        char **ns_list;
};
void free_dns_data(void);

struct net_stat {
        char *dev;
        int up;
        long long last_read_recv, last_read_trans;
        long long recv, trans;
        double recv_speed, trans_speed;
        struct sockaddr addr;
#if defined(__linux__)
        char addrs[273];
#endif /* __linux__ */
        double net_rec[15], net_trans[15];
        // wireless extensions
        char essid[32];
        char bitrate[16];
        char mode[16];
        int link_qual;
        int link_qual_max;
        char ap[18];
};
void clear_net_stats(void);
struct net_stat *get_net_stat(const char *);
int interface_up(const char *dev);

void get_adt746x_cpu(char *, size_t);
void get_adt746x_fan(char *, size_t);

int open_acpi_temperature(const char *name);
double get_acpi_temperature(int fd);
void get_acpi_ac_adapter(char *, size_t);
void get_acpi_fan(char *, size_t);
void get_battery_stuff(char *buf, unsigned int n, const char *bat, int item);
int get_battery_perct(const char *bat);
int get_battery_perct_bar(const char *bat);
void get_battery_short_status(char *buf, unsigned int n, const char *bat);

#endif /* _COMMON_H */
