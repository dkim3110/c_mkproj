#include "mkproj.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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
} /* file_path_maker() */

static int generate_paths(project_paths_t *path, const char *root, project_mode_t mode) {
  if (snprintf(path->root, MAX_PATH_LEN, "%s", root) >= MAX_PATH_LEN) return EXIT_FAILURE;

  switch (mode) {
    case FULL:
      if (file_path_maker(path->root, path->bin, "bin") == EXIT_FAILURE) return EXIT_FAILURE;
      if (file_path_maker(path->root, path->lib, "lib") == EXIT_FAILURE) return EXIT_FAILURE;
    case DEFAULT:
      if (file_path_maker(path->root, path->build, "build") == EXIT_FAILURE) return EXIT_FAILURE;
      if (file_path_maker(path->root, path->include, "include") == EXIT_FAILURE) return EXIT_FAILURE;
      if (file_path_maker(path->root, path->src, "src") == EXIT_FAILURE) return EXIT_FAILURE;

      if (file_path_maker(path->root, path->makefile, "Makefile") == EXIT_FAILURE) return EXIT_FAILURE;
    case BARE:
      if (file_path_maker((mode == BARE) ? path->root : path->src, path->main_c, "main.c") == EXIT_FAILURE) {
        return EXIT_FAILURE;
      }

      if (file_path_maker(path->root, path->readme, "README.md") == EXIT_FAILURE) return EXIT_FAILURE;
      if (file_path_maker(path->root, path->lic, "LICENSE.md") == EXIT_FAILURE) return EXIT_FAILURE;
      break;
    case UNKNOWN:
    default:
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
} /* generate_paths() */

static int directory_maker(char *file_path) {
  int check = MAKE_DIR(file_path);
  if (check != 0) fprintf(stderr, "-fatal: failed to create directory %s: %s\n", file_path, strerror(errno));
  return (check == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* directory_maker() */

static FILE *file_opener(const char *file_path) {
  FILE *fptr = fopen(file_path, "w");
  if (!fptr) {
    fprintf(stderr, "-fatal: failed to create file %s: %s\n", file_path, strerror(errno));
    return NULL;
  }

  return fptr;
} /* file_opener() */

static int file_closer(FILE **fptr, const char *file_path) {
  if (fclose(*fptr) != 0) {
    fprintf(stderr, "-fatal: failed to close file %s: %s\n", file_path, strerror(errno));
    return EXIT_FAILURE;
  }

  *fptr = NULL;
  return EXIT_SUCCESS;
} /* file_closer() */

static int file_maker(project_paths_t *path, project_mode_t mode) {
  FILE *main_c_fptr = file_opener(path->main_c);
  if (!main_c_fptr) return EXIT_FAILURE;
  mkproj_write_main_c(main_c_fptr);
  if (file_closer(&main_c_fptr, path->main_c) == EXIT_FAILURE) goto cleanup_main;

  if (mode != BARE) {
    FILE *makefile_fptr = file_opener(path->makefile);
    if (!makefile_fptr) goto cleanup_main;
    mkproj_write_makefile(makefile_fptr, mode);
    if (file_closer(&makefile_fptr, path->makefile) == EXIT_FAILURE) goto cleanup_makefile;
  }

  FILE *readme_fptr = file_opener(path->readme);
  if (!readme_fptr) goto cleanup_makefile;
  mkproj_write_md(readme_fptr, README);
  if (file_closer(&readme_fptr, path->readme) == EXIT_FAILURE) goto cleanup_readme;

  FILE *lic_fptr = file_opener(path->lic);
  if (!lic_fptr) goto cleanup_readme;
  mkproj_write_md(lic_fptr, LICENSE);
  if (file_closer(&lic_fptr, path->lic) == EXIT_FAILURE) goto cleanup_lic;

  return EXIT_SUCCESS;

  cleanup_lic:
  remove(path->lic);
  cleanup_readme:
  remove(path->readme);
	cleanup_makefile:
  if (mode != BARE) remove(path->makefile);
	cleanup_main:
	remove(path->main_c);
  return EXIT_FAILURE;
} /* file_maker() */

static int defaukt_project(project_paths_t *path, project_mode_t mode) {
  if (mode != BARE) {
    if (directory_maker(path->build) == EXIT_FAILURE) return EXIT_FAILURE;
    if (directory_maker(path->include) == EXIT_FAILURE) goto cleanup_build;
    if (directory_maker(path->src) == EXIT_FAILURE) goto cleanup_include;
  }

  if (file_maker(path, mode) == EXIT_FAILURE) {
    if (mode != BARE) goto cleanup_src;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

  cleanup_src:
  REMOVE_DIR(path->src);
  cleanup_include:
  REMOVE_DIR(path->include);
  cleanup_build:
  REMOVE_DIR(path->build);
  return EXIT_FAILURE;
} /* defaukt_project() */
// ============================================================== HELPER ==

// == PRIMARY =============================================================
int mkproj_option_default(const char *root) {
  project_paths_t path = {0};

  if (generate_paths(&path, root, DEFAULT) == EXIT_FAILURE) return EXIT_FAILURE;
  if (defaukt_project(&path, DEFAULT) == EXIT_FAILURE) return EXIT_FAILURE;

  return EXIT_SUCCESS;
} /* mkproj_option_default() */

int mkproj_option_bare(const char *root) {
  project_paths_t path = {0};

  if (generate_paths(&path, root, BARE) == EXIT_FAILURE) return EXIT_FAILURE;
  if (defaukt_project(&path, BARE) == EXIT_FAILURE) return EXIT_FAILURE;

  return EXIT_SUCCESS;
} /* mkproj_option_bare() */

int mkproj_option_full(const char *root) {
  project_paths_t path = {0};

  if (generate_paths(&path, root, FULL) == EXIT_FAILURE) return EXIT_FAILURE;
  if (directory_maker(path.bin) == EXIT_FAILURE) return EXIT_FAILURE;
  if (directory_maker(path.lib) == EXIT_FAILURE) goto cleanup_bin;

  if (defaukt_project(&path, FULL) == EXIT_FAILURE) goto cleanup_lib;

  return EXIT_SUCCESS;

  cleanup_lib:
  REMOVE_DIR(path.lib);
  cleanup_bin:
  REMOVE_DIR(path.bin);
  return EXIT_FAILURE;
} /* mkproj_option_full() */
// ============================================================= PRIMARY ==
