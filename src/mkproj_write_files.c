#include "mkproj.h"

#include <stdio.h>
#include <stdlib.h>

const char MAKEFILE_BARE[] = {
#embed "../assets/Makefile/bare"
	, '\0'};

const char MAKEFILE_DEFAULT[] = {
#embed "../assets/Makefile/default"
	, '\0'};

const char MAKEFILE_FULL[] = {
#embed "../assets/Makefile/full"
	, '\0'};

const char CLANGD_CONTENTS[] = {
#embed "../assets/clangd_template"
	, '\0'};

const char GITIGNORE_CONTENTS[] = {
#embed "../assets/gitignore_template"
	, '\0'};

const char README_CONTENTS[] = {
#embed "../assets/README_template"
	, '\0'};

const char MAIN_C_CONTENTS[] = {
#embed "../assets/main_c_template"
	, '\0'};

const char TEST_C_CONTENTS[] = {
#embed "../assets/test_c_template"
	, '\0'};

const char HEADER_CONTENTS[] = {
#embed "../assets/main_h_template"
	, '\0'};

// == HELPER ==================================================================
static const char *handle_makefile(project_flag_t flag) {
	switch (flag) {
		case BARE:		return MAKEFILE_BARE;
		case PLUS:		/* fallthrough */
		case FULL:		return MAKEFILE_FULL;
		case DEFAULT: /* fallthrough */
		default:			return MAKEFILE_DEFAULT;
	}

	return NULL;
} /* handle_makefile() */
// ================================================================== HELPER ==

// == PRIMARY =================================================================
int mkproj_write_file(FILE *fptr, project_flag_t flag, file_maker_mode_t mode) {
	const char *contents = NULL;

	switch (mode) {
		case MAKEFILE:	contents = handle_makefile(flag); break;
		case TEST_C:		contents = TEST_C_CONTENTS; break;
		case MAIN_C:		contents = MAIN_C_CONTENTS; break;
		case README:		contents = README_CONTENTS; break;
		case GITIGNORE: contents = GITIGNORE_CONTENTS; break;
		case CLANGD:		contents = CLANGD_CONTENTS; break;
		case HEADER:		contents = HEADER_CONTENTS; break;
		default:				contents = NULL; break;
	}

	if ((!contents) || (fputs(contents, fptr) == EOF)) {
		fputs("-fatal: could not write to file\n", stderr);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
} /* mkproj_write_file() */
// ================================================================= PRIMARY ==
