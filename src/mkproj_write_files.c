#include "mkproj.h"
#include "mkproj_file_contents.h"

#include <stdio.h>

// == HELPER ==============================================================
static char *handle_makefile(project_flag_t flag) {
	switch (flag) {
		case BARE:
			return MAKEFILE_BARE;
			break;
		case PLUS:
			/* fallthrough */
		case FULL:
			return MAKEFILE_FULL;
			break;
		case DEFAULT:
		default:
			return MAKEFILE_DEFAULT;
			break;
	}
} /* handle_makefile() */
// ============================================================== HELPER ==

// == PRIMARY =============================================================
void mkproj_write_file(FILE *fptr, project_flag_t flag, file_maker_mode_t mode) {
	char *contents = NULL;

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
		default:
			contents = "";
			break;
	}

	if (fputs(contents, fptr) == EOF) fprintf(stderr, "-error: could not write to file\n");
}/* mkproj_write_file() */
// ============================================================= PRIMARY ==
