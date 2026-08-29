#include "mkproj.h"

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

static int generic_project_maker(const char *dir_name, makefile_mode_t makefile_mode) {
  char build_path[MAX_PATH_LEN];
  char include_path[MAX_PATH_LEN];
  char src_path[MAX_PATH_LEN];

  char main_path[MAX_PATH_LEN];
  char readme_path[MAX_PATH_LEN];
  char lic_path[MAX_PATH_LEN];
  char makefile_path[MAX_PATH_LEN];

  if (file_path_maker(dir_name, build_path, "build") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, include_path, "include") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, src_path, "src") == EXIT_FAILURE) return EXIT_FAILURE;

  if (directory_maker(build_path) == EXIT_FAILURE) return EXIT_FAILURE;
  if (directory_maker(include_path) == EXIT_FAILURE) goto cleanup_build;
  if (directory_maker(src_path) == EXIT_FAILURE) goto cleanup_include;

  if (file_path_maker(src_path, main_path, "main.c") == EXIT_FAILURE) goto cleanup_src;
  FILE *main_fptr = file_opener(main_path);
  if (!main_fptr) return EXIT_FAILURE;
  populate_main(main_fptr);
  fclose(main_fptr);

  if (file_path_maker(dir_name, makefile_path, "Makefile") == EXIT_FAILURE) goto cleanup_main;
  FILE *makefile_fptr = file_opener(makefile_path);
  if (!makefile_fptr) goto cleanup_main;
  populate_makefile(makefile_fptr, makefile_mode);
  fclose(makefile_fptr);

  if (file_path_maker(dir_name, readme_path, "README.md") == EXIT_FAILURE) goto cleanup_makefile;
  FILE *readme_fptr = file_opener(readme_path);
  if (!readme_fptr) goto cleanup_makefile;
  populate_md(readme_fptr, README);
  fclose(readme_fptr);

  if (file_path_maker(dir_name, lic_path, "LICENSE.md") == EXIT_FAILURE) goto cleanup_readme;
  FILE *lic_fptr = file_opener(lic_path);
  if (!lic_fptr) goto cleanup_readme;
  populate_md(lic_fptr, LICENSE);
  fclose(lic_fptr);

  return EXIT_SUCCESS;

	cleanup_readme:
  remove(readme_path);
	cleanup_makefile:
  remove(makefile_path);
	cleanup_main:
  remove(main_path);
	cleanup_src:
  rmdir(src_path);
	cleanup_include:
  rmdir(include_path);
	cleanup_build:
  rmdir(build_path);
  return EXIT_FAILURE;
}
// ============================================================== HELPER ==

// == PRIMARY =============================================================
int option_bare(const char *dir_name) {
  char main_path[MAX_PATH_LEN];
  char readme_path[MAX_PATH_LEN];
  char lic_path[MAX_PATH_LEN];

  if (file_path_maker(dir_name, main_path, "main.c") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, readme_path, "README.md") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, lic_path, "LICENSE.md") == EXIT_FAILURE) return EXIT_FAILURE;

  FILE *main_fptr = file_opener(main_path);
  if (!main_fptr) return EXIT_FAILURE;
  populate_main(main_fptr);
  fclose(main_fptr);

  FILE *readme_fptr = file_opener(readme_path);
  if (!readme_fptr) goto cleanup_main;
  populate_md(readme_fptr, README);
  fclose(readme_fptr);

  FILE *lic_fptr = file_opener(lic_path);
  if (!lic_fptr) goto cleanup_readme;
  populate_md(lic_fptr, LICENSE);
  fclose(lic_fptr);

  return EXIT_SUCCESS;

	cleanup_readme:
  remove(readme_path);
	cleanup_main:
  remove(main_path);
  return EXIT_FAILURE;
}

int option_default(const char *dir_name) {
  return generic_project_maker(dir_name, DEFAULT);
}

int option_full(const char *dir_name) {
  char bin_path[MAX_PATH_LEN];
  char lib_path[MAX_PATH_LEN];

  if (generic_project_maker(dir_name, FULL) == EXIT_FAILURE) return EXIT_FAILURE;

  if (file_path_maker(dir_name, bin_path, "bin") == EXIT_FAILURE) return EXIT_FAILURE;
  if (file_path_maker(dir_name, lib_path, "lib") == EXIT_FAILURE) return EXIT_FAILURE;

  if (directory_maker(bin_path) == EXIT_FAILURE) return EXIT_FAILURE;
  if (directory_maker(lib_path) == EXIT_FAILURE) goto cleanup_bin;

  return EXIT_SUCCESS;

	cleanup_bin:
  rmdir(bin_path);
  return EXIT_FAILURE;
}
// ============================================================= PRIMARY ==
