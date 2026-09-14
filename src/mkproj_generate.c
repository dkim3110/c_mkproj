#define _CRT_SECURE_NO_WARNINGS
#include "mkproj.h"

#include <stdio.h>
#include <stdlib.h>

// == HELPER ==============================================================
static int file_path_maker(const char *parent, char *child, const char *child_name) {
  int check = snprintf(child, MAX_PATH_LEN, "%s/%s", parent, child_name);
  if (check >= MAX_PATH_LEN) {
    fputs("-fatal: name too long\n", stderr);
    return EXIT_FAILURE;
  }

  if (check < 0) {
    fputs("-fatal: encoding error\n", stderr);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
} /* file_path_maker() */

static int directory_maker(char *file_path) {
  int check = MAKE_DIR(file_path);
  if (check != 0) {
    fprintf(stderr, "-fatal: failed to create directory ");
    perror(file_path);
  }
  return (check == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* directory_maker() */

static FILE *file_opener(const char *file_path) {
  FILE *fptr = fopen(file_path, "w");
  if (!fptr) {
    fprintf(stderr, "-fatal: failed to create file ");
    perror(file_path);
    return NULL;
  }

  return fptr;
} /* file_opener() */

static int file_closer(FILE **fptr, const char *file_path) {
  if (fclose(*fptr) != 0) {
    fprintf(stderr, "-fatal: failed to close file ");
    perror(file_path);
    return EXIT_FAILURE;
  }

  *fptr = NULL;
  return EXIT_SUCCESS;
} /* file_closer() */

static int file_maker(char *file_path, project_flag_t flag, file_maker_mode_t mode) {
  FILE *fptr = file_opener(file_path);
  if ((!fptr) || (mkproj_write_file(fptr, flag, mode) == EXIT_FAILURE) ||
      (file_closer(&fptr, file_path) == EXIT_FAILURE))
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
} /* file_maker() */
// ============================================================== HELPER ==

// == PRIMARY =============================================================
int mkproj_generate_project(const char *root, project_flag_t requested_flag) {
  char current_path[MAX_PATH_LEN];
  int layout_size = sizeof(PROJECT_LAYOUT) / sizeof(PROJECT_LAYOUT[0]);
  int target_mask = (1 << requested_flag);
  int failed_at_index = -1;

  for (int n = 0; n < layout_size; n++) {
    if (!(PROJECT_LAYOUT[n].flags & target_mask))
      continue;

    if (file_path_maker(root, current_path, PROJECT_LAYOUT[n].rel_path) ==
        EXIT_FAILURE) {
      failed_at_index = n;
      break;
    }

    if (PROJECT_LAYOUT[n].is_dir) {
      if (directory_maker(current_path) == EXIT_FAILURE) {
        failed_at_index = n;
        break;
      }
    } else {
      if (file_maker(current_path, requested_flag, PROJECT_LAYOUT[n].mode) ==
          EXIT_FAILURE) {
        failed_at_index = n;
        break;
      }
    }
  }

  if (failed_at_index != -1) {
    for (int n = failed_at_index - 1; n >= 0; n--) {
      if (!(PROJECT_LAYOUT[n].flags & target_mask))
        continue;

      file_path_maker(root, current_path, PROJECT_LAYOUT[n].rel_path);

      if (PROJECT_LAYOUT[n].is_dir)
        REMOVE_DIR(current_path);
      else
        remove(current_path);
    }

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
} /* mkproj_generate_project() */
// ============================================================= PRIMARY ==
