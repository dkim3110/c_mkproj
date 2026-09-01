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
		fprintf(stderr, "-fatal: failed to create directory '%s': ", file_path);
		perror("");
	 	fprintf(stderr, "\n");
	}
	return (check == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} /* directory_maker() */

static FILE *file_opener(const char *file_path) {
	FILE *fptr;
	errno_t check = fopen_s(&fptr, file_path, "w");
	if (check != 0) {
		fprintf(stderr, "-fatal: failed to create file '%s': ", file_path);
	 	perror("");
	 	fprintf(stderr, "\n");
		return NULL;
	}

	return fptr;
} /* file_opener() */

static int file_closer(FILE **fptr, const char *file_path) {
	if (fclose(*fptr) != 0) {
		fprintf(stderr, "-fatal: failed to close file '%s': ", file_path);
		perror("");
	 	fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	*fptr = NULL;
	return EXIT_SUCCESS;
} /* file_closer() */

static int file_maker(project_paths_t *path, project_flag_t flag) {
	FILE *main_c_fptr = file_opener(path->main_c);
	if (!main_c_fptr) return EXIT_FAILURE;
	mkproj_write_main_c(main_c_fptr);
	if (file_closer(&main_c_fptr, path->main_c) == EXIT_FAILURE) goto cleanup_main;

	FILE *makefile_fptr = file_opener(path->makefile);
	if (!makefile_fptr) goto cleanup_main;
	mkproj_write_makefile(makefile_fptr, flag);
	if (file_closer(&makefile_fptr, path->makefile) == EXIT_FAILURE) goto cleanup_makefile;

	FILE *readme_fptr = file_opener(path->readme);
	if (!readme_fptr) goto cleanup_makefile;
	mkproj_write_readme(readme_fptr);
	if (file_closer(&readme_fptr, path->readme) == EXIT_FAILURE) goto cleanup_readme;

	return EXIT_SUCCESS;

cleanup_readme:
	remove(path->readme);
cleanup_makefile:
	remove(path->makefile);
cleanup_main:
	remove(path->main_c);
	return EXIT_FAILURE;
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
			if (file_path_maker(path.root, path.bin, "bin") == EXIT_FAILURE) return EXIT_FAILURE;
			if (directory_maker(path.bin) == EXIT_FAILURE) return EXIT_FAILURE;

			if (file_path_maker(path.root, path.lib, "lib") == EXIT_FAILURE) goto cleanup_bin;
			if (directory_maker(path.lib) == EXIT_FAILURE) goto cleanup_bin;
			/* fallthrough */
		case DEFAULT:
			if (file_path_maker(path.root, path.build, "build") == EXIT_FAILURE) goto cleanup_lib;
			if (directory_maker(path.build) == EXIT_FAILURE) goto cleanup_lib;

			if (file_path_maker(path.root, path.include, "include") == EXIT_FAILURE) goto cleanup_build;
			if (directory_maker(path.include) == EXIT_FAILURE) goto cleanup_build;

			if (file_path_maker(path.root, path.src, "src") == EXIT_FAILURE) goto cleanup_include;
			if (directory_maker(path.src) == EXIT_FAILURE) goto cleanup_include;
			/* fallthrough */
		case BARE:
			if (file_path_maker((flag == BARE) ? path.root : path.src, path.main_c, "main.c") == EXIT_FAILURE) {
				if (flag != BARE) goto cleanup_src;
				return EXIT_FAILURE;
			}

			if (file_path_maker(path.root, path.makefile, "Makefile") == EXIT_FAILURE) {
				if (flag != BARE) goto cleanup_src;
				return EXIT_FAILURE;
			}

			if (file_path_maker(path.root, path.readme, "README.md") == EXIT_FAILURE) {
				if (flag != BARE) goto cleanup_src;
				return EXIT_FAILURE;
			}

			if (file_maker(&path, flag) == EXIT_FAILURE) {
				if (flag != BARE) goto cleanup_src;
				return EXIT_FAILURE;
			}
			break;
		case UNKNOWN:
		default:
			fprintf(stderr, "-fatal: failed to generate project\n");
			return EXIT_FAILURE;
			break;
	}

	return EXIT_SUCCESS;

cleanup_src:
	REMOVE_DIR(path.src);
cleanup_include:
	REMOVE_DIR(path.include);
cleanup_build:
	REMOVE_DIR(path.build);
cleanup_lib:
	if (flag == FULL) REMOVE_DIR(path.lib);
cleanup_bin:
	if (flag == FULL) REMOVE_DIR(path.bin);
	return EXIT_FAILURE;
} /* mkproj_generate_project() */
// ============================================================= PRIMARY ==
