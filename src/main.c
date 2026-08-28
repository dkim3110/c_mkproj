#include "mkproj.h"

static char *g_flags_list[] = {
  "--bare",
  "--default",
  "--full",
  "--help",
  "--name",
};

// == USAGE ===============================================================
static void print_usage(char *program) {
  fprintf(stderr,
    "\n"
    "USAGE:\n"
    "  %s [flag] --name [project name]\n"
    "  %s --name [project name] [flag]\n"
    "\n"
    "OPTIONS:\n"
    "   --name\n"
    "               Defines the name of directory; mandatory\n"
    "   --help\n"
    "               Prints this help message; overrides all arguments\n"
    "   --bare\n"
    "               Creates project in bare mode:\n"
    "                   [project name]\n"
    "                   |--LICENSE.md\n"
    "                   |--main.c\n"
    "                   `--README.md\n"
    "\n"
    "   --full\n"
    "               Creates project in full mode:\n"
    "                   [project name]\n"
    "                   |--bin\n"
    "                   |--build\n"
    "                   |--include\n"
    "                   |--lib\n"
    "                   |--src\n"
    "                   |  `--main.c\n"
    "                   |--LICENSE.md\n"
    "                   |--Makefile\n"
    "                   |--README.md\n"
    "\n"
    "   --default, [no flags]\n"
    "               Creates project in default mode:\n"
    "                   [project name]\n"
    "                   |--build\n"
    "                   |--include\n"
    "                   |--src\n"
    "                   |  `--main.c\n"
    "                   |--LICENSE.md\n"
    "                   |--Makefile\n"
    "                   |--README.md\n"
    "\n"
    , program, program
  );
}
// =============================================================== USAGE ==

// == PARSE ARGS ==========================================================
static int has_flag(int argc, char **argv, const char *flag) {
  for (int n = 1; n < argc; n++) {
    if (strncmp(argv[n], "--", 2) != 0) continue;
    if (strcmp(argv[n], flag) == 0) {
      return 1;
    }
  }

  return 0;
}

static int has_valid_flag(int argc, char **argv, int *flag_count) {
  int flag_list_size = sizeof(g_flags_list) / sizeof(g_flags_list[0]);
  for (int n = 1; n < argc; n++) {
    if (strncmp(argv[n], "--", 2) != 0) continue;

    int is_known = 0;
    for (int m = 0; m < flag_list_size; m++) {
      if (strcmp(argv[n], g_flags_list[m]) == 0) {
        is_known = 1;
        (*flag_count)++;
        break;
      }
    }

    if (!is_known) return 0;
  }

  return 1;
}

static const char *find_arg(int argc, char **argv, const char *flag) {
  for (int n = 1; n < argc - 1; n++) {
    if (strcmp(argv[n], flag) == 0) {
      if (strncmp(argv[n + 1], "--", 2) != 0) return argv[n + 1];
    }
  }

  return NULL;
}
// ========================================================== PARSE ARGS ==

// == MAIN ================================================================
int main(int argc, char *argv[]) {
  int result, flag_count = 0;

  if (argc < 2) {
    fprintf(stderr, "-fatal: no arguments given\n");
    return EXIT_FAILURE;
  }

  if (!has_valid_flag(argc, argv, &flag_count)) {
    fprintf(stderr, "-fatal: unknown flag\n");
    return EXIT_FAILURE;
  }

  // --help
  if (has_flag(argc, argv, "--help")) {
    print_usage(argv[0]);
    return EXIT_SUCCESS;
  }

  if ((flag_count - has_flag(argc, argv, "--name")) > 1) {
    fprintf(stderr, "-fatal: conflicting flags\n");
    return EXIT_FAILURE;
  }

  const char *dir_name = find_arg(argc, argv, "--name");

  if (!dir_name) {
    fprintf(stderr, "-fatal: unable to determine name\n");
    return EXIT_FAILURE;
  }

  int main_dir_failed = MAKE_DIR(dir_name);

  if (main_dir_failed) {
    fprintf(stderr, "-fatal: failed to create directory: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  /* -- flag check ------------------------------------------------------ */

  // --bare
  if (has_flag(argc, argv, "--bare")) {
    result = option_bare(dir_name);
    if (result == EXIT_FAILURE) rmdir(dir_name);
    return result;
  }

  // --full
  if (has_flag(argc, argv, "--full")) {
    result = option_full(dir_name);
    if (result == EXIT_FAILURE) rmdir(dir_name);
    return result;
  }

  // --default
  result = option_default(dir_name);
  if (result == EXIT_FAILURE) rmdir(dir_name);
  return result;
  /* ------------------------------------------------------ flag check -- */
}
// ================================================================ MAIN ==
