#include "mkproj.h"

#define EXIT_HELP (2)

static flag_t g_flags_list[] = {
  {"--bare", BARE},
  {"--default", DEFAULT},
  {"--full", FULL},
  {"--help", UNKNOWN}
};

// == HELPER ==============================================================
static void print_help_message(char *program) {
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

static int parse_args(int argc, char **argv, config_t *config) {
  for (int n = 1; n < argc; n++) {
    if (strncmp(argv[n], "--help", 6) == 0) return EXIT_HELP;

    if (argv[n][0] != '-') {
      if (config->parent_directory) {
        fprintf(stderr, "-fatal: too many arguments\n");
        return EXIT_FAILURE;
      }

      config->parent_directory = argv[n];
    }

    if (argv[n][0] == '-') {
      /* -- verify flag ------------------------------------------------ */
      int flag_list_size = sizeof(g_flags_list) / sizeof(g_flags_list[0]);
      int is_valid = 0;

      for (int m = 0; m < flag_list_size; m++) {
        if (strcmp(argv[n], g_flags_list[m].name) == 0) {
          is_valid = 1;
          config->flag = g_flags_list[m].flag;
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
    }
  }

  return (config->parent_directory) ? EXIT_SUCCESS : EXIT_FAILURE;
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
    print_help_message(argv[0]);
    return EXIT_SUCCESS;
  }

  if ((arg_check == EXIT_FAILURE)) {
    if (config.flag == UNKNOWN) fprintf(stderr, "-fatal: unknown flag\n");
    else if (config.flag_count > 1) fprintf(stderr, "-fatal: conflicting flags\n");
    else if (!config.parent_directory) fprintf(stderr, "-fatal: unable to determine name\n");

    return EXIT_FAILURE;
  }

  if (MAKE_DIR(config.parent_directory)) {
    fprintf(stderr, "-fatal: failed to create directory: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  /* -- check flag ----------------------------------------------------- */
  switch (config.flag) {
    case BARE:
      if (option_bare(config.parent_directory) == EXIT_FAILURE) goto cleanup_parent_directory;
      break;
    case FULL:
      if (option_full(config.parent_directory) == EXIT_FAILURE) goto cleanup_parent_directory;
      break;
    case DEFAULT:
    default:
      if (option_default(config.parent_directory) == EXIT_FAILURE) goto cleanup_parent_directory;
      break;
  }
  /* ----------------------------------------------------- check flag -- */

  return EXIT_SUCCESS;

  cleanup_parent_directory:
  rmdir(config.parent_directory);
  return EXIT_FAILURE;
}
// ================================================================ MAIN ==
