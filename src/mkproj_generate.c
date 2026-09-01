#define _CRT_SECURE_NO_WARNINGS
#include "mkproj.h"

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

static int directory_maker(char *file_path) {
	int check = MAKE_DIR(file_path);
	if (check != 0) {
		fprintf(stderr, "-fatal: failed to create directory ");
		perror(file_path);
	 	fprintf(stderr, "\n");
	}
	return (check == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* directory_maker() */

static FILE *file_opener(const char *file_path) {
	FILE *fptr = fopen(file_path, "w");
	if (!fptr) {
		fprintf(stderr, "-fatal: failed to create file ");
	 	perror(file_path);
	 	fprintf(stderr, "\n");
		return NULL;
	}

	return fptr;
} /* file_opener() */

static int file_closer(FILE **fptr, const char *file_path) {
	if (fclose(*fptr) != 0) {
		fprintf(stderr, "-fatal: failed to close file ");
		perror(file_path);
	 	fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	*fptr = NULL;
	return EXIT_SUCCESS;
} /* file_closer() */

static int file_maker(char *file_path, project_flag_t flag, file_maker_mode_t mode) {
	FILE *fptr = file_opener(file_path);
	if ((!fptr) || (mkproj_write_file(fptr, flag, mode) == EXIT_FAILURE) ||
			(file_closer(&fptr, file_path) == EXIT_FAILURE)) return EXIT_FAILURE;

	return EXIT_SUCCESS;
} /* file_maker() */
// ============================================================== HELPER ==

// == PRIMARY =============================================================
int mkproj_generate_project(const char *root, project_flag_t flag) {
	project_paths_t path = {0};

	int check = snprintf(path.root, MAX_PATH_LEN, "%s", root);
	if(check >= MAX_PATH_LEN) {
		fprintf(stderr, "-fatal: name too long\n");
		return EXIT_FAILURE;
	}

	if (check < 0) {
		fprintf(stderr, "-fatal: encoding error\n");
		return EXIT_FAILURE;
	}

	switch (flag) {
		case FULL:
			if (file_path_maker(path.root, path.docs, "docs") == EXIT_FAILURE) return EXIT_FAILURE;
			if (directory_maker(path.docs) == EXIT_FAILURE) return EXIT_FAILURE;

			if (file_path_maker(path.root, path.data, "data") == EXIT_FAILURE) goto cleanup_docs;
			if (directory_maker(path.data) == EXIT_FAILURE) goto cleanup_docs;

			if (file_path_maker(path.data, path.raw, "raw") == EXIT_FAILURE) goto cleanup_data;
			if (directory_maker(path.raw) == EXIT_FAILURE) goto cleanup_data;

			if (file_path_maker(path.data, path.interim, "interim") == EXIT_FAILURE) goto cleanup_raw;
			if (directory_maker(path.interim) == EXIT_FAILURE) goto cleanup_raw;

			if (file_path_maker(path.data, path.input, "input") == EXIT_FAILURE) goto cleanup_interim;
			if (directory_maker(path.input) == EXIT_FAILURE) goto cleanup_interim;

			if (file_path_maker(path.data, path.output, "output") == EXIT_FAILURE) goto cleanup_input;
			if (directory_maker(path.output) == EXIT_FAILURE) goto cleanup_input;

			if (file_path_maker(path.root, path.gitignore, ".gitignore") == EXIT_FAILURE) goto cleanup_output;
			if (file_maker(path.gitignore, flag, GITIGNORE) == EXIT_FAILURE) goto cleanup_output;
			/* fallthrough */
		case PLUS:
			if (file_path_maker(path.root, path.bin, "bin") == EXIT_FAILURE) goto cleanup_gitignore;
			if (directory_maker(path.bin) == EXIT_FAILURE) goto cleanup_gitignore;

			if (file_path_maker(path.root, path.lib, "lib") == EXIT_FAILURE) goto cleanup_bin;
			if (directory_maker(path.lib) == EXIT_FAILURE) goto cleanup_bin;

			if (file_path_maker(path.root, path.tests, "tests") == EXIT_FAILURE) goto cleanup_lib;
			if (directory_maker(path.tests) == EXIT_FAILURE) goto cleanup_lib;

			if (file_path_maker(path.tests, path.test_main_c, "test_main.c") == EXIT_FAILURE) goto cleanup_tests;
			if (file_maker(path.test_main_c, flag, TEST_MAIN_C) == EXIT_FAILURE) goto cleanup_tests;
			/* fallthrough */
		case DEFAULT:
			if (file_path_maker(path.root, path.build, "build") == EXIT_FAILURE) goto cleanup_test_main_c;
			if (directory_maker(path.build) == EXIT_FAILURE) goto cleanup_test_main_c;

			if (file_path_maker(path.root, path.include, "include") == EXIT_FAILURE) goto cleanup_build;
			if (directory_maker(path.include) == EXIT_FAILURE) goto cleanup_build;

			if (file_path_maker(path.root, path.src, "src") == EXIT_FAILURE) goto cleanup_include;
			if (directory_maker(path.src) == EXIT_FAILURE) goto cleanup_include;
			/* fallthrough */
		case BARE:
			if ((file_path_maker((flag == BARE) ? path.root : path.src, path.main_c, "main.c") == EXIT_FAILURE) ||
					(file_path_maker(path.root, path.makefile, "Makefile") == EXIT_FAILURE) ||
					(file_path_maker(path.root, path.readme, "README.md") == EXIT_FAILURE)) {
				if (flag != BARE) goto cleanup_src;
				return EXIT_FAILURE;
			}

			if (file_maker(path.main_c, flag, MAIN_C) == EXIT_FAILURE) {
				if (flag != BARE) goto cleanup_src;
				return EXIT_FAILURE;
			}

			if (file_maker(path.readme, flag, README) == EXIT_FAILURE) goto cleanup_main_c;
			if (file_maker(path.makefile, flag, MAKEFILE) == EXIT_FAILURE) goto cleanup_readme;
			break;
		case UNKNOWN:
		default:
			fprintf(stderr, "-fatal: failed to generate project\n");
			return EXIT_FAILURE;
			break;
	}

	return EXIT_SUCCESS;

cleanup_readme:
	if (path.readme[0] != '\0') remove(path.readme);
cleanup_main_c:
	if (path.main_c[0] != '\0') remove(path.main_c);
cleanup_src:
	if (path.src[0] != '\0') REMOVE_DIR(path.src);
cleanup_include:
	if (path.include[0] != '\0') REMOVE_DIR(path.include);
cleanup_build:
	if (path.build[0] != '\0') REMOVE_DIR(path.build);
cleanup_test_main_c:
	if (path.test_main_c[0] != '\0') remove(path.test_main_c);
cleanup_tests:
	if (path.tests[0] != '\0') REMOVE_DIR(path.tests);
cleanup_lib:
	if (path.lib[0] != '\0') REMOVE_DIR(path.lib);
cleanup_bin:
	if (path.bin[0] != '\0') REMOVE_DIR(path.bin);
cleanup_gitignore:
	if (path.gitignore[0] != '\0') remove(path.gitignore);
cleanup_output:
	if (path.output[0] != '\0') REMOVE_DIR(path.output);
cleanup_input:
	if (path.input[0] != '\0') REMOVE_DIR(path.input);
cleanup_interim:
	if (path.interim[0] != '\0') REMOVE_DIR(path.interim);
cleanup_raw:
	if (path.raw[0] != '\0') REMOVE_DIR(path.raw);
cleanup_data:
	if (path.data[0] != '\0') REMOVE_DIR(path.data);
cleanup_docs:
	if (path.docs[0] != '\0') REMOVE_DIR(path.docs);
	return EXIT_FAILURE;
} /* mkproj_generate_project() */
// ============================================================= PRIMARY ==
