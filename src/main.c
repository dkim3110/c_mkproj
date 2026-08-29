#include "mkproj.h"

#define EXIT_HELP (2)

static char *g_flags_list[] = {
  "--bare",
  "--default",
  "--full",
  "--help",
};

// == USAGE ===============================================================
static void print_usage(char *program) {
  fprintf(stderr,
    "\n"
    "USAGE:\n"
    "  %s [flag] [project name]\n"
    "  %s [project name] [flag]\n"
    "\n"
    "OPTIONS:\n"
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

// == HELPER ==============================================================
static int parse_args(int argc, char **argv, config_t *config) {
  for (int n = 1; n < argc; n++) {
    if (argv[n][0] != '-') {
      if (config->dir_name) {
        fprintf(stderr, "-fatal: too many arguments\n");
        return EXIT_FAILURE;
      }

      config->dir_name = argv[n];
    }

    if (argv[n][0] == '-') {
      /* -- verify flag ------------------------------------------------ */
      int flag_list_size = sizeof(g_flags_list) / sizeof(g_flags_list[0]);
      int is_valid = 0;

      for (int m = 0; m < flag_list_size; m++) {
        if (strcmp(argv[n], g_flags_list[m]) == 0) {
          is_valid = 1;
          break;
        }
      }

      if (!is_valid) {
        config->flag = UNKNOWN;
        return EXIT_FAILURE;
      }
      /* ------------------------------------------------ verify flag -- */

      config->flag_count++;
      if (config->flag_count > 1) return EXIT_FAILURE;

      if (strncmp(argv[n], "--full", 6) == 0) config->flag = FULL;
      if (strncmp(argv[n], "--bare", 6) == 0) config->flag = BARE;
      if (strncmp(argv[n], "--help", 6) == 0) return EXIT_HELP;
    }
  }

  return (config->dir_name) ? EXIT_SUCCESS : EXIT_FAILURE;
}
// ============================================================== HELPER ==

// == MAIN ================================================================
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "-fatal: no arguments given\n");
    return EXIT_FAILURE;
  }

  config_t config = {0, DEFAULT, NULL};
  int arg_check = parse_args(argc, argv, &config);

  // --help
  if (arg_check == EXIT_HELP) {
    print_usage(argv[0]);
    return EXIT_SUCCESS;
  }

  if ((arg_check == EXIT_FAILURE)) {
    if (config.flag == UNKNOWN) fprintf(stderr, "-fatal: unknown flag\n");
    else if (config.flag_count > 1) fprintf(stderr, "-fatal: conflicting flags\n");
    else if (!config.dir_name) fprintf(stderr, "-fatal: unable to determine name\n");

    return EXIT_FAILURE;
  }

  int main_dir_failed = MAKE_DIR(config.dir_name);

  if (main_dir_failed) {
    fprintf(stderr, "-fatal: failed to create directory: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  /* -- check flag ----------------------------------------------------- */
  switch (config.flag) {
    case BARE:
      if (option_bare(config.dir_name) == EXIT_FAILURE) goto cleanup_dir_name;
      break;
    case FULL:
      if (option_full(config.dir_name) == EXIT_FAILURE) goto cleanup_dir_name;
      break;
    case DEFAULT:
    default:
      if (option_default(config.dir_name) == EXIT_FAILURE) goto cleanup_dir_name;
      break;
  }
  /* ----------------------------------------------------- check flag -- */

  return EXIT_SUCCESS;

  cleanup_dir_name:
  rmdir(config.dir_name);
  return EXIT_FAILURE;
}
// ================================================================ MAIN ==
