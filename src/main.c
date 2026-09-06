#include "mkproj.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_HELP (2)

static flag_t g_flags_list[] = {
	{"-b", "--bare", BARE},
	{"-d", "--default", DEFAULT},
	{"-f", "--full", FULL},
	{"-h", "--help", UNKNOWN},
	{"-p", "--plus", PLUS}
};

// == HELPER ==============================================================
static void print_help_message(char *program) {
	fprintf(stderr,
		"\n"
		"USAGE:\n"
		" %s [flag] project_name\n"
		" %s project_name [flag]\n"
		"\n"
		"OPTIONS:\n"
		, program, program
	);
	fprintf(stderr,
		" -h, --help\tPrints this help message; overrides all arguments\n"
		"\n"
	);
	fprintf(stderr,
		" -b, --bare""                          "" -d, --default, [no flags]\n"
		" Creates project in bare mode:""       "" Creates project in default mode:\n"
		"   project_name""                      ""   project_name\n"
		"   |--main.c""                         ""   |--build\n"
		"   |--main.h""                         ""   |--include\n"
		"   |--Makefile""                       ""   |  `--main.h\n"
		"   `--README.md""                      ""   |--src\n"
		"  ""                                   ""   |  `--main.c\n"
		"  ""                                   ""   |--Makefile\n"
		"  ""                                   ""   `--README.md\n"
		"\n"
		" -p, --plus""                          "" -f, --full\n"
		" Creates project in plus mode:""       "" Creates project in full mode:\n"
		"   project_name""                      ""   project_name\n"
		"   |--bin""                            ""   |--bin\n"
		"   |--build""                          ""   |--build\n"
		"   |--include""                        ""   |--data\n"
		"   |  `--main.h""                      ""   |  `--raw\n"
		"   |--lib""                            ""   |  `--interim\n"
		"   |--src""                            ""   |  `--input\n"
		"   |  `--main.c""                      ""   |  `--output\n"
		"   |--tests""                          ""   |--docs\n"
		"   |  `--test_main.c""                 ""   |--include\n"
		"   |--Makefile""                       ""   |  `--main.h\n"
		"   ` --README.md""                     ""   |--lib\n"
		"                ""                     ""   |--src\n"
		"                ""                     ""   |  `--main.c\n"
		"                ""                     ""   |--tests\n"
		"                ""                     ""   |  `--test_main.c\n"
		"                ""                     ""   |--.gitignore\n"
		"                ""                     ""   |--Makefile\n"
		"                ""                     ""   `--README.md\n"
		"\n"
	);
} /* print_help_message() */

static int parse_args(int argc, char *argv[], config_t *config) {
	for (int n = 1; n < argc; n++) {
		if ((strcmp(argv[n], "-h") == 0) ||
				(strcmp(argv[n], "--help") == 0)) return EXIT_HELP;
	}

	for (int n = 1; n < argc; n++) {
		if (argv[n][0] != '-') {
			if (config->root) {
				fprintf(stderr, "-fatal: too many arguments\n");
				return EXIT_FAILURE;
			}

			if (strlen(argv[n]) >= MAX_PATH_LEN) {
				fprintf(stderr, "-fatal: name too long\n");
				return EXIT_FAILURE;
			}

			config->root = argv[n];
		} else {
			/* -- verify flag ------------------------------------------------ */
			int flag_list_size = sizeof(g_flags_list) / sizeof(g_flags_list[0]);
			int is_valid = 0;

			for (int m = 0; m < flag_list_size; m++) {
				if ((strcmp(argv[n], g_flags_list[m].short_name) == 0) ||
						(strcmp(argv[n], g_flags_list[m].full_name) == 0)) {
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

	return (config->root) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* parse_args() */
// ============================================================== HELPER ==

// == MAIN ================================================================
int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "-fatal: no arguments given\n");
		return EXIT_FAILURE;
	}

	config_t config = {0, DEFAULT, NULL};
	int arg_check = parse_args(argc, argv, &config);

	if (arg_check == EXIT_HELP) {
		print_help_message(argv[0]);
		return EXIT_SUCCESS;
	}

	if (arg_check == EXIT_FAILURE) {
		if (config.flag == UNKNOWN) fprintf(stderr, "-fatal: unknown flag\n");
		else if (config.flag_count > 1) fprintf(stderr, "-fatal: conflicting flags\n");
		else if (!config.root) fprintf(stderr, "-fatal: unable to determine name\n");

		return EXIT_FAILURE;
	}

	if (MAKE_DIR(config.root)) {
		perror("-fatal: failed to create directory");
		return EXIT_FAILURE;
	}

	if (mkproj_generate_project(config.root, config.flag) == EXIT_FAILURE) {
		REMOVE_DIR(config.root);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
} /* main() */
// ================================================================ MAIN ==
