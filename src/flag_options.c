#include "mkproj.h"

char g_bin_path[MAX_PATH_LEN];
char g_build_path[MAX_PATH_LEN];
char g_include_path[MAX_PATH_LEN];
char g_lib_path[MAX_PATH_LEN];
char g_src_path[MAX_PATH_LEN];
char g_main_path[MAX_PATH_LEN];
char g_readme_path[MAX_PATH_LEN];
char g_lic_path[MAX_PATH_LEN];
char g_makefile_path[MAX_PATH_LEN];

// == HELPER ==============================================================
static int file_path_maker(const char *parent, char *child, const char *child_name) {
  int check = snprintf(child, MAX_PATH_LEN, "%s/%s", parent, child_name);
  if(check >= MAX_PATH_LEN) {
    fprintf(stderr, "-fatal: name too long\n");
    return EXIT_FAILURE;
  }

  if (check < 0) {
    fprintf(stderr, "-fatal: encoding error\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static int directory_maker(char *file_path) {
  int check = MAKE_DIR(file_path);
  if (check != 0) fprintf(stderr, "-fatal: failed to create directory %s: %s\n", file_path, strerror(errno));
  return (check == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static FILE *file_opener(const char *file_path) {
  FILE *fptr = fopen(file_path, "w");
  if (!fptr) {
    fprintf(stderr, "-fatal: failed to create file %s: %s\n", file_path, strerror(errno));
    return NULL;
  }

  return fptr;
}

static int file_exists(const char* filename) {
  struct stat buffer;
  int exist = stat(filename, &buffer);

  return !exist;
}
// ============================================================== HELPER ==

// == PRIMARY =============================================================
int option_bare(const char *dir_name) {
  if (file_path_maker(dir_name, g_main_path, "main.c") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_readme_path, "README.md") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_lic_path, "LICENSE.md") == EXIT_FAILURE) return EXIT_FAILURE;

  if (!file_exists(g_main_path)) {
    FILE *main_fptr = file_opener(g_main_path);
    if (!main_fptr) return EXIT_FAILURE;
    populate_main(main_fptr);
    fclose(main_fptr);
  }

  if (!file_exists(g_readme_path)) {
    FILE *readme_fptr = file_opener(g_readme_path);
    if (!readme_fptr) goto cleanup_main;
    populate_md(readme_fptr, README);
    fclose(readme_fptr);
  }

  if (!file_exists(g_lic_path)) {
    FILE *lic_fptr = file_opener(g_lic_path);
    if (!lic_fptr) goto cleanup_readme;
    populate_md(lic_fptr, LICENSE);
    fclose(lic_fptr);
  }

  return EXIT_SUCCESS;

cleanup_readme:
  remove(g_readme_path);
cleanup_main:
  remove(g_main_path);
  return EXIT_FAILURE;
}

int option_default(const char *dir_name) {
  if (file_path_maker(dir_name, g_build_path, "build") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_include_path, "include") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_src_path, "src") == EXIT_FAILURE) return EXIT_FAILURE;

  if (directory_maker(g_build_path) == EXIT_FAILURE) return EXIT_FAILURE;
  if (directory_maker(g_include_path) == EXIT_FAILURE) goto cleanup_build;
  if (directory_maker(g_src_path) == EXIT_FAILURE) goto cleanup_include;

  if (file_path_maker(g_src_path, g_main_path, "main.c") == EXIT_FAILURE) goto cleanup_src;
  if (!file_exists(g_main_path)) {
    FILE *main_fptr = file_opener(g_main_path);
    if (!main_fptr) return EXIT_FAILURE;
    populate_main(main_fptr);
    fclose(main_fptr);
  }

  if (file_path_maker(dir_name, g_makefile_path, "Makefile") == EXIT_FAILURE) goto cleanup_main;
  if (!file_exists(g_makefile_path)) {
    FILE *makefile_fptr = file_opener(g_makefile_path);
    if (!makefile_fptr) goto cleanup_main;
    populate_makefile(makefile_fptr, DEFAULT);
    fclose(makefile_fptr);
  }

  if (file_path_maker(dir_name, g_readme_path, "README.md") == EXIT_FAILURE) goto cleanup_makefile;
  if (!file_exists(g_readme_path)) {
    FILE *readme_fptr = file_opener(g_readme_path);
    if (!readme_fptr) goto cleanup_makefile;
    populate_md(readme_fptr, README);
    fclose(readme_fptr);
  }

  if (file_path_maker(dir_name, g_lic_path, "LICENSE.md") == EXIT_FAILURE) goto cleanup_readme;
  if (!file_exists(g_lic_path)) {
    FILE *lic_fptr = file_opener(g_lic_path);
    if (!lic_fptr) goto cleanup_readme;
    populate_md(lic_fptr, LICENSE);
    fclose(lic_fptr);
  }


  return EXIT_SUCCESS;

cleanup_readme:
  remove(g_readme_path);
cleanup_makefile:
  remove(g_makefile_path);
cleanup_main:
  remove(g_main_path);
cleanup_src:
  rmdir(g_src_path);
cleanup_include:
  rmdir(g_include_path);
cleanup_build:
  rmdir(g_build_path);
  return EXIT_FAILURE;
}

int option_full(const char *dir_name) {
  if (file_path_maker(dir_name, g_bin_path, "bin") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_build_path, "build") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_include_path, "include") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_lib_path, "lib") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, g_src_path, "src") == EXIT_FAILURE) return EXIT_FAILURE;

  if (directory_maker(g_bin_path) == EXIT_FAILURE) return EXIT_FAILURE;
  if (directory_maker(g_build_path) == EXIT_FAILURE) goto cleanup_bin;
  if (directory_maker(g_include_path) == EXIT_FAILURE) goto cleanup_build;
  if (directory_maker(g_lib_path) == EXIT_FAILURE) goto cleanup_include;
  if (directory_maker(g_src_path) == EXIT_FAILURE) goto cleanup_lib;

  if (file_path_maker(g_src_path, g_main_path, "main.c") == EXIT_FAILURE) goto cleanup_src;
  if (!file_exists(g_main_path)) {
    FILE *main_fptr = file_opener(g_main_path);
    if (!main_fptr) return EXIT_FAILURE;
    populate_main(main_fptr);
    fclose(main_fptr);
  }

  if (file_path_maker(dir_name, g_makefile_path, "Makefile") == EXIT_FAILURE) goto cleanup_main;
  if (!file_exists(g_makefile_path)) {
    FILE *makefile_fptr = file_opener(g_makefile_path);
    if (!makefile_fptr) goto cleanup_main;
    populate_makefile(makefile_fptr, DEFAULT);
    fclose(makefile_fptr);
  }

  if (file_path_maker(dir_name, g_readme_path, "README.md") == EXIT_FAILURE) goto cleanup_makefile;
  if (!file_exists(g_readme_path)) {
    FILE *readme_fptr = file_opener(g_readme_path);
    if (!readme_fptr) goto cleanup_makefile;
    populate_md(readme_fptr, README);
    fclose(readme_fptr);
  }

  if (file_path_maker(dir_name, g_lic_path, "LICENSE.md") == EXIT_FAILURE) goto cleanup_readme;
  if (!file_exists(g_lic_path)) {
    FILE *lic_fptr = file_opener(g_lic_path);
    if (!lic_fptr) goto cleanup_readme;
    populate_md(lic_fptr, LICENSE);
    fclose(lic_fptr);
  }


  return EXIT_SUCCESS;

cleanup_readme:
  remove(g_readme_path);
cleanup_makefile:
  remove(g_makefile_path);
cleanup_main:
  remove(g_main_path);
cleanup_src:
  rmdir(g_src_path);
cleanup_lib:
  rmdir(g_lib_path);
cleanup_include:
  rmdir(g_include_path);
cleanup_build:
  rmdir(g_build_path);
cleanup_bin:
  rmdir(g_bin_path);
  return EXIT_FAILURE;
}
// ============================================================= PRIMARY ==
