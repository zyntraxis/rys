#include "../include/package_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void trim_newline(char *str) {
  size_t len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
}

static int file_exists(const char *filename) {
  struct stat buffer;
  return (stat(filename, &buffer) == 0);
}

static void parse_zyn_toml(struct PackageInfo *pkg) {
  FILE *f = fopen("zyn.toml", "r");
  if (!f)
    return;

  char line[512];
  while (fgets(line, sizeof(line), f)) {
    trim_newline(line);

    if (strncmp(line, "name =", 6) == 0) {
      const char *val = strchr(line, '"');
      if (val) {
        val++;
        char *end = strchr(val, '"');
        if (end) {
          *end = '\0';
          strncpy(pkg->package_name, val, MAX_STR_LEN);
        }
      }
    } else if (strncmp(line, "version =", 9) == 0) {
      const char *val = strchr(line, '"');
      if (val) {
        val++;
        char *end = strchr(val, '"');
        if (end) {
          *end = '\0';
          strncpy(pkg->version, val, MAX_STR_LEN);
        }
      }
    }
  }
  fclose(f);
}

static void parse_cmake_lists(struct PackageInfo *pkg) {
  FILE *f = fopen("CMakeLists.txt", "r");
  if (!f)
    return;

  char line[512];
  while (fgets(line, sizeof(line), f)) {
    trim_newline(line);

    if (strncmp(line, "project(", 8) == 0) {
      char *start = strchr(line, '(');
      char *end = strchr(line, ')');
      if (start && end && end > start) {
        char proj_info[512];
        strncpy(proj_info, start + 1, end - start - 1);
        proj_info[end - start - 1] = '\0';

        char *token = strtok(proj_info, " ");
        if (token) {
          strncpy(pkg->package_name, token, MAX_STR_LEN);
          while (token) {
            if (strcmp(token, "VERSION") == 0) {
              token = strtok(NULL, " ");
              if (token) {
                strncpy(pkg->version, token, MAX_STR_LEN);
                break;
              }
            }
            token = strtok(NULL, " ");
          }
        }
      }
    }
  }
  fclose(f);
}

struct PackageInfo get_package_info() {
  struct PackageInfo pkg = {0};

  int has_zyn = file_exists("zyn.toml");
  int has_cmake = file_exists("CMakeLists.txt");

  if (has_zyn) {
    parse_zyn_toml(&pkg);
  }

  if (has_cmake) {
    parse_cmake_lists(&pkg);
  }

  if (pkg.package_name[0] == '\0') {
    printf("Package name: ");
    if (fgets(pkg.package_name, sizeof(pkg.package_name), stdin))
      trim_newline(pkg.package_name);
  }

  if (pkg.executable_file[0] == '\0') {
    printf("Executable file: ");
    if (fgets(pkg.executable_file, sizeof(pkg.executable_file), stdin))
      trim_newline(pkg.executable_file);
  }

  if (pkg.version[0] == '\0') {
    printf("Version: ");
    if (fgets(pkg.version, sizeof(pkg.version), stdin))
      trim_newline(pkg.version);
  }

  if (pkg.description[0] == '\0') {
    printf("Description: ");
    if (fgets(pkg.description, sizeof(pkg.description), stdin))
      trim_newline(pkg.description);
  }

  if (pkg.maintainer[0] == '\0') {
    printf("Maintainer: ");
    if (fgets(pkg.maintainer, sizeof(pkg.maintainer), stdin))
      trim_newline(pkg.maintainer);
  }

  if (pkg.arch[0] == '\0') {
    printf("Architecture (amd64/arm/all): ");
    if (fgets(pkg.arch, sizeof(pkg.arch), stdin))
      trim_newline(pkg.arch);
  }

  if (pkg.app_type[0] == '\0') {
    printf("App type (console/gui/service/lib/script): ");
    if (fgets(pkg.app_type, sizeof(pkg.app_type), stdin))
      trim_newline(pkg.app_type);
  }

  return pkg;
}
