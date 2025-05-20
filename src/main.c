#include "../include/package_info.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <command>\n", argv[0]);
    return 1;
  }

  char *command = argv[1];

  if (strcmp(command, "build") == 0) {
    struct PackageInfo pkg = get_package_info();
    build_deb_package(&pkg);
  }

  return 0;
}
