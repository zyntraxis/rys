#include "../include/package_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void build_deb_package(const struct PackageInfo *pkg) {
  if (pkg->executable_file[0] == '\0' || pkg->package_name[0] == '\0' ||
      pkg->version[0] == '\0') {
    fprintf(stderr, "Error: missing required fields.\n");
    return;
  }

  mkdir("build_package", 0755);
  char path[512];

  const char *target_dir = strcmp(pkg->app_type, "lib") == 0       ? "usr/lib"
                           : strcmp(pkg->app_type, "script") == 0  ? "usr/bin"
                           : strcmp(pkg->app_type, "service") == 0 ? "usr/sbin"
                                                                   : "usr/bin";

  snprintf(path, sizeof(path), "build_package/%s", target_dir);
  char cmd[512];
  snprintf(cmd, sizeof(cmd), "mkdir -p %s", path);
  system(cmd);

  snprintf(cmd, sizeof(cmd), "cp %s build_package/%s/", pkg->executable_file,
           target_dir);
  system(cmd);

  snprintf(path, sizeof(path), "build_package/DEBIAN");
  mkdir(path, 0755);

  snprintf(path, sizeof(path), "build_package/DEBIAN/control");
  FILE *f = fopen(path, "w");
  if (!f) {
    perror("fopen dst");
    return;
  }

  fprintf(f, "Package: %s\n", pkg->package_name);
  fprintf(f, "Version: %s\n", pkg->version);
  fprintf(f, "Section: base\n");
  fprintf(f, "Priority: optional\n");
  fprintf(f, "Architecture: %s\n", pkg->arch);
  fprintf(f, "Maintainer: %s\n", pkg->maintainer);
  fprintf(f, "Description: %s\n", pkg->description);
  fclose(f);

  printf("Package structure prepared in './build_package/'.\n");
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    return;
  }

  if (pid == 0) {
    char *args[] = {"dpkg-deb", "--build", "./build_package/", NULL};
    execvp(args[0], args);
    perror("execvp");
    exit(1);
  } else {
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      printf("Package built successfully.\n");
    } else {
      printf("Package build failed with code %d\n", WEXITSTATUS(status));
    }
  }
}
