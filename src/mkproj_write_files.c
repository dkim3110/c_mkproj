#include "mkproj.h"
#include "mkproj_file_contents.h"

#include <stdio.h>
#include <stdlib.h>

// == HELPER ==============================================================
static const char *handle_makefile(project_flag_t flag) {
	switch (flag) {
		case BARE:
			return MAKEFILE_BARE;
		case PLUS:
			/* fallthrough */
		case FULL:
			return MAKEFILE_FULL;
		case DEFAULT:
		default:
			return MAKEFILE_DEFAULT;
	}

	return NULL;
} /* handle_makefile() */
// ============================================================== HELPER ==

// == PRIMARY =============================================================
int mkproj_write_file(FILE *fptr, project_flag_t flag, file_maker_mode_t mode) {
	const char *contents = NULL;

	switch (mode) {
		case MAKEFILE:
			contents = handle_makefile(flag);
			break;
		case TEST_MAIN_C:
			contents = TEST_MAIN_C_CONTENTS;
			break;
		case MAIN_C:
			contents = MAIN_C_CONTENTS;
			break;
		case README:
			contents = README_CONTENTS;
			break;
		case GITIGNORE:
			contents = GITIGNORE_CONTENTS;
			break;
		case HEADER:
			contents = HEADER_CONTENTS;
			break;
		default:
			contents = "";
			break;
	}

	if ((!contents) || (fputs(contents, fptr) == EOF)) {
		perror("-fatal: could not write to file");

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}/* mkproj_write_file() */
// ============================================================= PRIMARY ==
