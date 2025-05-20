#pragma once

#define MAX_STR_LEN 256

struct PackageInfo {
  char package_name[MAX_STR_LEN];
  char executable_file[MAX_STR_LEN];
  char version[MAX_STR_LEN];
  char description[MAX_STR_LEN];
  char maintainer[MAX_STR_LEN];
  char arch[MAX_STR_LEN];
  char app_type[MAX_STR_LEN];
};

struct PackageInfo get_package_info();
void build_deb_package(const struct PackageInfo *package_info);
static void trim_newline(char *str);
static int file_exists(const char *filename);
static void parse_zyn_toml(struct PackageInfo *pkg);
static void parse_cmake_lists(struct PackageInfo *pkg);
